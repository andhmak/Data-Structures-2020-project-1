#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "gif.h"
#include "life.h"

int main(int argc, char *argv[]) {
    LifeState state = life_create_from_rle(argv[1]);
    int top = atoi(argv[2]), left = atoi(argv[3]), bottom = atoi(argv[4]), right = atoi(argv[5]), frames = atoi(argv[6]), speed = atoi(argv[8]), delay = atoi(argv[9]);
    float zoom = atof(argv[7]);
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
	// Μεγέθη
	int size = 128;
	int cell_size = 5;

	// Δημιουργία ενός GIF και ενός bitmap στη μνήμη
	GIF* gif = gif_create(size, size);
	Bitmap* bitmap = bm_create(size, size);

	// Default καθυστέρηση μεταξύ των frames, σε εκατοστά του δευτερολέπτου
	gif->default_delay = 10;

	// Δημιουργούμε ενα animation με ένα "cell" το οποίο μετακινείται από τη δεξιά-πάνω
	// γωνία προς την κάτω-αριστερά. Το cell μετακινείται ένα pixel τη φορά, οπότε το animation
	// θα έχει τόσα frames όσα το μέθεθος της εικόνας.

	for(int i = 0; i < size; i++) {
		// Σε κάθε frame, πρώτα μαυρίζουμε ολόκληρο το bitmap
		bm_set_color(bitmap, bm_atoi("black"));
		bm_clear(bitmap);

		// Και μετά ζωγραφίζουμε ένα άσπρο τετράγωνο με αρχή το
		// σημείο (i,i) και τέλος το (i+cell_size, i+cell_size)
		bm_set_color(bitmap, bm_atoi("white"));
		bm_fillrect(bitmap, i, i, i+cell_size, i+cell_size);
   
		// Τέλος προσθέτουμε το bitmap σαν frame στο GIF (τα περιεχόμενα αντιγράφονται)
		gif_add_frame(gif, bitmap);
	}

	// Αποθήκευση σε αρχείο
	gif_save(gif, "example.gif");

	// Αποδέσμευση μνήμης
	bm_free(bitmap);
	gif_free(gif);
    return 0;
}