![run-tests](../../workflows/run-tests/badge.svg)

## 2020 Project 1

Εκφώνηση: https://k08.chatzi.org/projects/project1/


### Προσωπικά στοιχεία

__Όνομα__: Δημάκης Αντώνιος

__Α.Μ.__: sdi1900047


### Documentation

Κώδικας άσκησης 1:<br>
line_read.c<br>
Κώδικας άσκησης 2:<br>
line_read_map.c<br>
Κώδικας άσκησης 3:<br>
exer_3.c<br>
Κώδικας άσκησης 4:<br>
pq_sort.c, pq_sort_test<br>
Κώδικας άσκησης 5:<br>
life.c, life_test.c<br>
Κώδικας άσκησης 6:<br>
life.c, life_gif.c<br>
Η υλοποίηση της κατάστασης έχει γίνει με ADTSet που περιέχει τα ζωντανά κελιά.<br>
Στην υλοποίηση του LifeCell η συντεταγμένη y αυξάνεται προς τα πάνω. Επομένως και στα arguments του life_gif το \<bottom\> πρέπει να είναι μικρότερο από \<top\>.<br>
Ένα state που δημιουργείται από ένα rle τοποθετείται στο τέταρτο τεταρτημόριο, αρχίζοντας από το LifeCell (0,0).<br>
Ένα state που αποθηκεύεται σε rle αγνοεί τον «νεκρό χώρο» πάνω και αριστερά, ώστε να αποθηκευτεί μόνο το ίδιο το pattern. Ένα glider δηλαδή αποθηκεύεται ως «bob$2bo$3o!» (αν είναι σε αυτό το βήμα) και όχι π.χ. ως «685$384bob$385bo$383bo!».<br>
Έχουν φτιαχθεί στο life.c και κάποιες συναρτήσεις που επιτρέπουν επαναληπτικές μεθοδους μέσα σε μία κατάσταση (διάσχιση των ζωντανών κελιών).<br>
Υπάρχουν στο life_test δύο συναρτήσεις που δεν ζητούνται, και δεν χρειάζονται βαθμολόγηση. Απλά αφήνονται μήπως διευκολύνουν την εξέταση της εργασίας.