#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "gif.h"
#include "life.h"

//ideas: don't calculate anything outside one cell out of borders, pass argument in life_evolve_many to do so, break up grid to find areas with loops

int main(int argc, char *argv[]) {
    LifeState state = life_create_from_rle(argv[1]);
    int top = atoi(argv[2]), left = atoi(argv[3]), bottom = atoi(argv[4]), right = atoi(argv[5]), frames = atoi(argv[6]), speed = atoi(argv[8]), delay = atoi(argv[9]);
    float zoom = atof(argv[7]);
	int int_zoom = round(zoom);
    char *gif_name = argv[10];
    if (frames < 1) {
        fprintf(stderr, "Invalid frames value");
        return 1;
    }
    if (zoom <= 0) {
        fprintf(stderr, "Invalid zoom value");
        return 1;
    }
    if (speed < 1) {
        fprintf(stderr, "Invalid speed value");
        return 1;
    }
	ListNode loop;
	List list = life_evolve_many(state, frames/speed, &loop);
	LifeCell cell;
	// Μεγέθη
//	int size = 128;
//	int cell_size = 5;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif = gif_create(round(zoom*(right - left)), round(zoom*(top - bottom)));
	Bitmap* bitmap;
	if (zoom >= 1) {
		bitmap = bm_create(int_zoom*(right - left), int_zoom*(top - bottom));
	}

	// Default καθυστέρηση μεταξύ των frames, σε εκατοστά του δευτερολέπτου
	gif->default_delay = delay;

	// Δημιουργούμε ενα animation με ένα "cell" το οποίο μετακινείται από τη δεξιά-πάνω
	// γωνία προς την κάτω-αριστερά. Το cell μετακινείται ένα pixel τη φορά, οπότε το animation
	// θα έχει τόσα frames όσα το μέθεθος της εικόνας.

	for (ListNode node = list_first(list);  ; ) {
		state = list_node_value(list, node);
		// Σε κάθε frame, πρώτα ασπρίζουμε ολόκληρο το bitmap
		bm_set_color(bitmap, bm_atoi("white"));
		bm_clear(bitmap);
		bm_set_color(bitmap, bm_atoi("black"));
		if (zoom >= 1) {
	        for (StateNode node = state_first(state) ; node != STATE_EOF ; node = state_next(state, node)) {
				cell = state_node_cell(state, node);
				// Και μετά ζωγραφίζουμε ένα μάυρο τετράγωνο με αρχή το
				// σημείο (i,i) και τέλος το (i+cell_size, i+cell_size)
				if ((cell.x > left) && (cell.x < right) && (cell.y > bottom) && (cell.y < top)) {
					bm_fillrect(bitmap, cell.x - (int_zoom - 1)/2 - left, cell.y - (int_zoom - 1)/2 - bottom, cell.x + (int_zoom)/2 - left, cell.y + (int_zoom)/2 - bottom);
				}
			}
		}
		else {

		}
		// Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
		gif_add_frame(gif, bitmap);
        if (list_next(list, node) == LIST_EOF) {
            if (loop) {
                node = loop;
            }
            else {
                break;
            }
        }
        else if (list_next(list, node) != LIST_EOF){
            node = list_next(list, node);
        }
    }

	// Αποθήκευση σε αρχείο
	gif_save(gif, gif_name);

	// Αποδέσμευση μνήμης
	bm_free(bitmap);
	gif_free(gif);
    return 0;
}