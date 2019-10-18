// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/

DLList getPeaks(DLList L){

    DLList peaksL = newDLList();
    peaksL->nitems = 0;
    assert (peaksL != NULL);
    
    if (L->nitems == 0 || L->nitems == 1 || L->nitems == 2) return peaksL;
    
    DLListNode *cur = L->first->next;
    while (cur->next != NULL) {
    
        // find peak node.
        if (cur->value > cur->prev->value && cur->value > cur->next->value) {
            // found a peak node.
            
            // create a new DLList called peakL and return.
            DLListNode *insert = newDLListNode (cur->value);
            
            if (peaksL->nitems == 0) {
                peaksL->first = peaksL->last = peaksL->curr = insert;
            }
            
            else {
                peaksL->last->next = insert;
                insert->prev = peaksL->last;
                peaksL->last = insert;
            }
            peaksL->nitems++;
        }
        
        cur = cur->next;
    }
    
    
    return peaksL;

}
