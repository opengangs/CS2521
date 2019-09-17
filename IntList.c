// IntList.c - Lists of integers
// Written by John Shepherd, July 2008

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>

#include "IntList.h"

// data structures representing IntList
struct IntListNode {
	int data;           /**< value of this list item */
	struct IntListNode *next;
	                    /**< pointer to node containing next element */
};

struct IntListRep {
	int size;           /**< number of elements in list */
	struct IntListNode *first;
	                    /**< node containing first value */
	struct IntListNode *last;
	                    /**< node containing last value */
};

/** Create a new, empty IntList. */
IntList newIntList (void)
{
	struct IntListRep *L = malloc (sizeof *L);
	if (L == NULL) err (EX_OSERR, "couldn't allocate IntList");
	L->size = 0;
	L->first = NULL;
	L->last = NULL;
	return L;
}

/** Release all resources associated with an IntList. */
void freeIntList (IntList L)
{
	if (L == NULL) return;

	for (struct IntListNode *curr = L->first, *next;
		 curr != NULL; curr = next) {
		next = curr->next;
		free (curr);
	}

	free (L);
}

/** Create an IntList by reading values from a file.
 * Assume that the file is open for reading. */
IntList getIntList (FILE *inf)
{
	IntList L = newIntList ();

	int v;
	while (fscanf (inf, "%d", &v) != EOF)
		IntListInsert (L, v);

	return L;
}

/** Display IntList as one integer per line on `stdout`. */
void showIntList (IntList L)
{
	IntListPrint (stdout, L);
}

/** create a new IntListNode with value v
 * (this function is local to this ADT) */
static struct IntListNode *newIntListNode (int v)
{
	struct IntListNode *n = malloc (sizeof *n);
	if (n == NULL) err (EX_OSERR, "couldn't allocate IntList node");
	n->data = v;
	n->next = NULL;
	return n;
}

/** Apppend one integer to the end of an IntList. */
void IntListInsert (IntList L, int v)
{
	assert (L != NULL);

	struct IntListNode *n = newIntListNode (v);
	if (L->first == NULL)
		L->first = L->last = n;
	else {
		L->last->next = n;
		L->last = n;
	}
	L->size++;
}

/** Insert an integer into correct place in a sorted IntList. */
void IntListInsertInOrder (IntList L, int v)
{

    assert (L != NULL); // assume L is not NULL
    
    struct IntListNode *new = newIntListNode (v);
    
    if (L->size == 0) {
        // no elements.
        L->first = L->last = new;
        
        L->size++;
        return;
    }
    
    // assume there is at least one item.
    if (v >= L->last->data) {
        // insert at tail.
        L->last->next = new;
        L->last = new;
        L->last->next = NULL;
        
        L->size++;
        return;
    }
    
    if (v <= L->first->data) {
        // insert at head.
        new->next = L->first;
        L->first = new;
        
        L->size++;
        return;
    }
    
    else {
        // insert somewhere in the middle.
        struct IntListNode *curr = L->first;
        
        while (curr->next != NULL) {
            if (v <= curr->next->data) {
                // found it.
                new->next = curr->next;
                curr->next = new;
                
                L->size++;
                return;
            }
            
            curr = curr->next;
        }
    }
}

/** Delete first occurrence of `v` from an IntList.
 * If `v` does not occur in IntList, no effect */
void IntListDelete (IntList L, int v)
{
	assert (L != NULL);

	// find where v occurs in list
	struct IntListNode *prev = NULL;
	struct IntListNode *curr = L->first;
	while (curr != NULL && curr->data != v) {
		prev = curr;
		curr = curr->next;
	}

	// not found; give up
	if (curr == NULL)
		return;

	// unlink curr
	if (prev == NULL)
		L->first = curr->next;
	else
		prev->next = curr->next;
	if (L->last == curr)
		L->last = prev;
	L->size--;

	// drop curr
	free (curr);
}

/** Return number of elements in an IntList. */
int IntListLength (IntList L)
{
	assert (L != NULL);
	return L->size;
}

/** Make a copy of an IntList.
 * New list should look identical to the original list. */
IntList IntListCopy (IntList L)
{
	assert (L != NULL);
	struct IntListRep *Lnew = newIntList ();
	for (struct IntListNode *curr = L->first;
		 curr != NULL; curr = curr->next)
		IntListInsert (Lnew, curr->data);
	return Lnew;
}

/** Make a sorted copy of an IntList. */
IntList IntListSortedCopy (IntList L)
{
	assert (L != NULL);
	struct IntListRep *Lnew = newIntList ();
	for (struct IntListNode *curr = L->first;
		 curr != NULL; curr = curr->next)
		IntListInsertInOrder (Lnew, curr->data);
	return Lnew;
}

/** Check whether an IntList is sorted in ascending order.
 * Returns `false` if list is not sorted, `true` if it is. */
bool IntListIsSorted (IntList L)
{
	assert (L != NULL);

	// trivial cases, 0 or 1 items
	if (L->size < 2)
		return true;

	// scan list, looking for out-of-order pair
	for (struct IntListNode *curr = L->first;
		 curr->next != NULL; curr = curr->next)
		if (curr->next->data < curr->data)
			return false;

	// nothing out-of-order, must be sorted
	return true;
}

/** Check internal consistency of an IntList. */
bool IntListOK (IntList L)
{
	if (L == NULL)
		return true;

	if (L->size == 0)
		return (L->first == NULL && L->last == NULL);

	// scan to (but not past) last node
	struct IntListNode *p = L->first;
	int count = 1; // at least one node
	while (p->next != NULL) {
		count++;
		p = p->next;
	}

	return (count == L->size && p == L->last);
}

/** Display an IntList as one integer per line to a file.
 * Assume that the file is open for writing. */
void IntListPrint (FILE *outf, IntList L)
{
	assert (L != NULL);
	for (struct IntListNode *curr = L->first;
		 curr != NULL; curr = curr->next)
		fprintf (outf, "%d\n", curr->data);
}
