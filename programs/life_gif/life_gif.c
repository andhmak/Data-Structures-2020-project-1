#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "gif.h"
#include "life.h"

//ideas: don't calculate anything outside one cell out of borders, pass argument in life_evolve_many to do so, break up grid to find areas with loops
//<------make zoom limit 0.67 instead of 1 //doesn't seem to help, both with many frames and high speed (many evoluctions)


int main(int argc, char *argv[]) {
    LifeState state = life_create_from_rle(argv[1]);
    int top = atoi(argv[2]), left = atoi(argv[3]), bottom = atoi(argv[4]), right = atoi(argv[5]), frames = atoi(argv[6]), speed = atoi(argv[8]), delay = atoi(argv[9]);
    float zoom = atof(argv[7]);
	int int_zoom = round(zoom);
	int zoom_out = round(1/zoom);
	uint live_cells[(right - left + 1)/zoom_out][(top - bottom + 1)/zoom_out];  //<----maybe malloc (calloc) so not used in zoom>=1 case (no significant difference time-wise in either case)
	if (zoom < 1) {
		for (uint i = 0 ; i < (right - left + 1)/zoom_out ; i++) {             //<-----maybe put an if on that loop so it doesn't run when zoom >= 1
			for (uint j = 0 ; j < (top - bottom + 1)/zoom_out ; j++) {
				live_cells[i][j] = 0;
			}
		}
	}
    char *gif_name = argv[10];
	if ((right < left) || (top < bottom)) {
		fprintf(stderr, "Invalid borders\n");
        return 1;
	}
    if (frames < 1) {
        fprintf(stderr, "Invalid frames value\n");
        return 1;
    }
    if (zoom <= 0) {
        fprintf(stderr, "Invalid zoom value\n");
        return 1;
    }
    if (speed < 1) {
        fprintf(stderr, "Invalid speed value\n");
        return 1;
    }
	if (delay < 10) {
        fprintf(stderr, "Lower delays than 10ms not supported by gif library\n");
        return 1;
    }
	ListNode loop;
	List list = life_evolve_many(state, (frames - 1)*speed + 1, &loop);
	LifeCell cell;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif;
	Bitmap* bitmap;
	if (zoom >= 1) {
		gif = gif_create(int_zoom*(right - left + 1), int_zoom*(top - bottom + 1));
		bitmap = bm_create(int_zoom*(right - left + 1), int_zoom*(top - bottom + 1));
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

	// Δημιουργούμε ενα animation με ένα "cell" το οποίο μετακινείται από τη δεξιά-πάνω
	// γωνία προς την κάτω-αριστερά. Το cell μετακινείται ένα pixel τη φορά, οπότε το animation
	// θα έχει τόσα frames όσα το μέθεθος της εικόνας.
	ListNode node = list_first(list);
	for (uint i = 0 ; i < (frames - 1)*speed + 1 ; i++) {
		if ((i % speed) == 0) {
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
					if ((cell.y < bottom)) { //<----or cell.y = bottom && cell.x > right (try again later, no significant difference either way)
						break;
					}
//					printf("x: %d, y: %d\n", cell.x, cell.y);
					if ((cell.x >= left) && (cell.x <= right) && (cell.y <= top)) {
						bm_fillrect(bitmap,
						(cell.x - left)*int_zoom,
						(cell.y - bottom)*int_zoom,
						(cell.x - left)*int_zoom + int_zoom - 1,
						(cell.y - bottom)*int_zoom + int_zoom - 1);
					}
				}
			}
			else {
				for (StateNode node = state_first(state) ; node != STATE_EOF ; node = state_next(state, node)) {
					cell = state_node_cell(state, node);
					// Και μετά ζωγραφίζουμε ένα μάυρο τετράγωνο με αρχή το
					// σημείο (i,i) και τέλος το (i+cell_size, i+cell_size)
					if (cell.y < bottom) { //<----or cell.y = bottom && cell.x > right
						break;
					}
//					printf("zoom_out: %d\n",zoom_out);
//					printf("cell.x: %d\n", cell.x);
//					printf("cell.y: %d\n", cell.y);
//					printf("left: %d\n", left);
//					printf("right: %d\n", right);
//					printf("top: %d\n", top);
//					printf("bottom: %d\n", bottom);
//					printf("truth: %d\n", (cell.x >= left) && (cell.x < (right)) && (cell.y < (top)));
					//<---maybe don't check if more than half already filled
					//<---the "if" for zoom can be moved fartehr in or out, it's a balance between cleaner code and speed (as the if will be evaluated less times)
					if ((cell.x >= left) && ((cell.x - left)/zoom_out <= (right - left)/zoom_out) && ((cell.y - bottom)/zoom_out <= (top - bottom)/zoom_out)) {
						live_cells[(cell.x - left)/zoom_out][(cell.y - bottom)/zoom_out]++;
						if (live_cells[(cell.x - left)/zoom_out][(cell.y - bottom)/zoom_out] > zoom_out*zoom_out/2) {
							bm_putpixel(bitmap, (cell.x - left)/zoom_out, (cell.y - bottom)/zoom_out);
						}
					}
				}
			}
			// Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
			bm_flip_vertical(bitmap);
			gif_add_frame(gif, bitmap);
		}
        if (list_next(list, node) == LIST_EOF) {
            if (loop) {
                node = loop;
            }
//            else {
//				printf("This shouldn't happen");
//				return 2;
//                break;
//            }
        }
        else if (list_next(list, node) != LIST_EOF) {
            node = list_next(list, node);
        }
    }
	list_destroy(list);
	// Αποθήκευση σε αρχείο
	gif_save(gif, gif_name);
	// Αποδέσμευση μνήμης
	bm_free(bitmap);
	gif_free(gif);
    return 0;
}

//<----maybe also remember frames if loop, not only states. no need to generate the same frames over and over. Maybe use same method with map and list as life_evolve_many.
//<----Seems most of the time goes into gif_save. Didn't expect that. Probably a good sign. Potentially a very bad sign.