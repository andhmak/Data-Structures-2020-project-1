/////////////////////////////////////////////////////////////////////////
//
// Παράδειγμα δημιουργίας ενός GIF χρησιμοποιώντας τη βιβλιοθήκη libbmp
//
///////////////////////////////////////////////////////////////////////// 

#include "bmp.h"
#include "gif.h"

int main(int argc, char *argv[]) {
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
}
