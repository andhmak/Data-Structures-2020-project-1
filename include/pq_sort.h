///////////////////////////////////////////////////////////////////
//
// pq_sort
//
// Ταξινόμηση μέσω ουράς προτεραιότητας
//
///////////////////////////////////////////////////////////////////

#pragma once // #include το πολύ μία φορά

#include "ADTVector.h"
#include "ADTList.h"


// Ταξινομεί το Vector vec, με βάση τη διάταξη της συνάρτησης compare

void pq_sort_vector(Vector vec, CompareFunc compare);

// Ταξινομεί τη λίστα list, με βάση τη διάταξη της συνάρτησης compare

void pq_sort_list(List list, CompareFunc compare);
