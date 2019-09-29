// DLList.h - Interface to doubly-linked list ADT
// Written by John Shepherd, March 2013
// Last modified, August 2014

#ifndef DLLIST_H
#define DLLIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// External view of DLList
// Implementation given in DLList.c
// Implements a DLList of strings (i.e. items are strings)

typedef struct DLListRep *DLList;

/** Create a new, empty DLList. */
DLList newDLList (void);

/** Release all resources associated with a DLList. */
void freeDLList (DLList);

/** Create an DLList by reading items from a file.
 * Assume that the file is open for reading. */
DLList getDLList (FILE *);

/** Display a DLList to file, one item per line.
 * Assume that the file is open for writing. */
void putDLList (FILE *, DLList);

/** Check internal consistency of a DLList (for testing). */
bool validDLList (DLList);

/** Return the item at current position. */
char *DLListCurrent (DLList);

/** Move current position (+ve forward, -ve backward).
 * Return false if we reach the end of list during the move. */
bool DLListMove (DLList, int);

/** move to specified position in list
 * i'th node, assuming first node has i==1 */
bool DLListMoveTo (DLList, int);

/** insert an item before current item
 * new item becomes current item */
void DLListBefore (DLList, char *);

/** insert an item after current item
 * new item becomes current item */
void DLListAfter (DLList, char *);

/** delete current item
 * new item becomes item following current
 * if current was last, current becomes new last
 * if current was only item, current becomes null */
void DLListDelete (DLList);

/** return number of elements in a list */
size_t DLListLength (DLList);

/** is the list empty? */
bool DLListIsEmpty (DLList);

#endif
