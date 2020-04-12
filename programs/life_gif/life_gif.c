#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "gif.h"
#include "life.h"

int main(int argc, char *argv[]) {
	// το πλήθος των ορισμάτων πρέπει να είναι 10 (+1 το πρόγραμμα)
	if (argc != 11) {
		fprintf(stderr, "Invalid amount of arguments\n");
		return 1;
	}
	// δημιουργία αρχικής κατάστασης από το δοσμένο rle και
	// εισαγωγή των άλλων παραμέτρων από την γραμμή εντολών
    LifeState state = life_create_from_rle(argv[1]);
    int top = atoi(argv[2]), left = atoi(argv[3]), bottom = atoi(argv[4]), right = atoi(argv[5]);
	int frames = atoi(argv[6]), speed = atoi(argv[8]), delay = atoi(argv[9]);
    float zoom = atof(argv[7]);
    char *gif_name = argv[10];
	// στρογγυλό ακέραιο zoom για την περίπτωση που είναι >= 1
	int zoom_in = round(zoom);
	// στρογγυλό ακέραιο αντίστροφο του zoom για την περίπτωση που είναι < 1
	int zoom_out = round(1/zoom);
	// πίνακας που θα αποθηκεύει το πλήθος των ζωντανών κελιών
	// σε κάθε pixel για την περίπτωση που zoom < 1
	uint live_cells[(right - left + 1)/zoom_out][(top - bottom + 1)/zoom_out];
	// αρχικοποίηση σε 0
	if (zoom < 1) {
		for (uint i = 0 ; i < (right - left + 1)/zoom_out ; i++) {
			for (uint j = 0 ; j < (top - bottom + 1)/zoom_out ; j++) {
				live_cells[i][j] = 0;
			}
		}
	}
	// το δεξί σύνορο πρέπει να είναι πιο μεγάλο από το
	// αριστερό, και το πάνω πιο μεγάλο από το κάτω
	if ((right < left) || (top < bottom)) {
		fprintf(stderr, "Invalid borders\n");
        return 1;
	}
	// πρέπει να παραχθεί τουλάχιστον ένα frame
    if (frames < 1) {
        fprintf(stderr, "Invalid frames value\n");
        return 1;
    }
	// δεν μπορεί το zoom να είναι αρνητικό
    if (zoom <= 0) {
        fprintf(stderr, "Invalid zoom value\n");
        return 1;
    }
	// δεν μπορεί ανάμεσα σε κάθε frame να
	// μεσολαβούν λιγότερες από μία εξελίξεις
    if (speed < 1) {
        fprintf(stderr, "Invalid speed value\n");
        return 1;
    }
	// η βιβλιοθήκη με το gif αποθηκεύει την καθυστέρηση σε εκατοστά του δευτερολέπτου
	// με μορφή int, άρα δεν μπορεί να αποθκεύσει καθυστερήσεις κάτων των 10ms
	if (delay < 10) {
        fprintf(stderr, "Lower delays than 10ms not supported by gif library\n");
        return 1;
    }
	// δημιουργία και αποθήκευση όλων των καταστάσεων που θα χρειαστούν σε λίστα,
	// με τόσες εξελίξεις όσες απαιτούν οι παράμετροι frames και speed, εκτός αν
	// προκύψει επανάληψη κατάστασης, οπότε σταματάει η εξέλιξη και
	// αποθηκεύεται αυτός ο κόμβος της λίστας
	ListNode loop;
	List statelist = life_evolve_many(state, (frames - 1)*speed + 1, &loop);
	LifeCell cell;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη,
	// των οποίων το μέγεθος καθορίζεται από το zoom
	GIF* gif;
	Bitmap* bitmap;
	if (zoom >= 1) {
		gif = gif_create(zoom_in*(right - left + 1), zoom_in*(top - bottom + 1));
		bitmap = bm_create(zoom_in*(right - left + 1), zoom_in*(top - bottom + 1));
	}
	else {
		gif = gif_create((right - left + 1)/zoom_out, (top - bottom + 1)/zoom_out);
		bitmap = bm_create((right - left + 1)/zoom_out, (top - bottom + 1)/zoom_out);
	}

	// Default καθυστέρηση μεταξύ των frames, σε εκατοστά του δευτερολέπτου
	gif->default_delay = delay/10;
	
	// Ορίζουμε τα χρώματα που χρησιμοποιούνται στο GIF (αλλιώς παίρνει αυτά που υπάρχουν στο πρώτο frame)
	unsigned int palette[] = { 0xFF000000, 0xFFFFFFFF }; // black, white
	gif_set_palette(gif, palette, 2);

	// Ξεκινόντας από τον πρώτο κόμβο της λίστας
	ListNode node = list_first(statelist);
	// σε μια επανάληψη που θα επαναληφθεί τόσες φορές όσες οι συνολικές έξελίξεις που έχουν ορισθεί
	for (uint i = 0 ; i < (frames - 1)*speed + 1 ; i++) {
		// αν η συγκεκριμένη πρέπει να αναπαρασταθεί σε frame
		if ((i % speed) == 0) {
			// αποθήκευση της κατάστασης από τον τρέχοντα κόμβο
			state = list_node_value(statelist, node);
			// άσπρισμα ολόκληρου του bitmap
			bm_set_color(bitmap, bm_atoi("white"));
			bm_clear(bitmap);
			// βάλε την "γραφίδα" στο μαύρο
			bm_set_color(bitmap, bm_atoi("black"));
			// αν γίνεται μεγέθυνση (ή τίποτα)
			if (zoom >= 1) {
				// για κάθε κόμβο της κατάστασης (κάθε κόμβος της περιέχει ένα ζωντανό κελί)
		        for (StateNode node = state_first(state) ; node != STATE_EOF ; node = state_next(state, node)) {
					// αποθηκεύεται το κελί
					cell = state_node_cell(state, node);
					// ζωγραφίζεται ένα μάυρο τετράγωνο με αρχή το σημείο
					// ((cell.x - left)*zoom_in, (cell.y - bottom)*zoom_in) και τέλος
					// το ((cell.x - left)*zoom_in + zoom_in - 1, (cell.y - bottom)*zoom_in + zoom_in - 1))
					// αν το κελί βρίσκεται στα όρια
					if ((cell.y < bottom)) {
						break;
					}
					if ((cell.x >= left) && (cell.x <= right) && (cell.y <= top)) {
						bm_fillrect(bitmap,
						(cell.x - left)*zoom_in,
						(cell.y - bottom)*zoom_in,
						(cell.x - left)*zoom_in + zoom_in - 1,
						(cell.y - bottom)*zoom_in + zoom_in - 1);
					}
				}
			}
			// αν γίνεται σμίκρυνση
			else {
				// για κάθε κόμβο της κατάστασης
				for (StateNode node = state_first(state) ; node != STATE_EOF ; node = state_next(state, node)) {
					// αποθηκεύεται το κελί
					cell = state_node_cell(state, node);
					// αν το κελί βρίσκεται εντός των ορίων
					if (cell.y < bottom) {
						break;
					}
					if ((cell.x >= left) && (cell.x - left + 1) <= (((right - left + 1)/zoom_out)*zoom_out) && ((cell.y - bottom + 1) <= ((top - bottom + 1)/zoom_out)*zoom_out)) {
						// προστίθεται ένα ζωντανό κελί στο pixel στο οποίο ανήκει
						live_cells[(cell.x - left)/zoom_out][(cell.y - bottom)/zoom_out]++;
						// αν η πλειονότητα των κελιών στο pixel είναι ζωντανά
						if (live_cells[(cell.x - left)/zoom_out][(cell.y - bottom)/zoom_out] > zoom_out*zoom_out/2) {
							// ζωγραφίζεται μάυρο
							bm_putpixel(bitmap, (cell.x - left)/zoom_out, (cell.y - bottom)/zoom_out);
						}
					}
				}
			}
			// Αντιστρέφουμε το bitmap γιατί στον κατακόρυφο άξονα "γράφει" από πάνω προς τα κάτω
			bm_flip_vertical(bitmap);
			// Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
			gif_add_frame(gif, bitmap);
		}
		// αν τελειώνει η λίστα
        if (list_next(statelist, node) == LIST_EOF) {
            // πήγαινε στον κόμβο όπου συνεχίζει η επανάληψη των καταστάσεων
			// (θα υπάρχει πάντα επανάληψη αν τελιώσει η λίστα πριν τελιώσει το εξωτερικό loop)
            node = loop;
        }
		// αλλιώς πήγαινε στον επόμενο κόμβο
        else {
            node = list_next(statelist, node);
        }
    }
	// Αποδέσμευση λίστας
	list_destroy(statelist);
	// Αποθήκευση σε αρχείο
	gif_save(gif, gif_name);
	// Αποδέσμευση bitmap και gif
	bm_free(bitmap);
	gif_free(gif);
    return 0;
}