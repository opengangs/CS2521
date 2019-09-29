// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "DLList.h"

int main (void)
{

	DLList myList = getDLList (stdin);
	assert (validDLList (myList));
	
	DLList emptyListDLListBefore = newDLList ();
	DLList emptyListDLListAfter = newDLList ();
	DLList ListDLListDelete = newDLList ();
	
	// create a linked list for DLListDelete.
	DLListBefore (ListDLListDelete, "4");
	DLListBefore (ListDLListDelete, "3");
	DLListBefore (ListDLListDelete, "2");
	DLListBefore (ListDLListDelete, "1");
	DLListBefore (ListDLListDelete, "0");

	// TODO: more tests needed here
	printf ("Testing in progress...\n");
	printf ("Stage one: DLListBefore\n\n");
	
	printf ("Test 1.1: DLListBefore -- empty list\n");
	printf ("Number of nodes: %zu\n", DLListLength (emptyListDLListBefore));
	printf ("Inserting new node: '0'\n");
	DLListBefore (emptyListDLListBefore, "0");
	printf ("New list:\n");
	putDLList (stdout, emptyListDLListBefore);
	assert (validDLList (emptyListDLListBefore));
	printf ("\n");
	
	printf ("Test 1.2: DLListBefore -- one node\n");
	printf ("Number of nodes: %zu\n", DLListLength (emptyListDLListBefore));
	printf ("Current node: %s\n", DLListCurrent (emptyListDLListBefore));
	printf ("Inserting new node before %s: '1'\n", DLListCurrent (emptyListDLListBefore));
	DLListBefore (emptyListDLListBefore, "1");
	printf ("New lsit:\n");
	putDLList (stdout, emptyListDLListBefore);
	assert (validDLList (emptyListDLListBefore));
	printf ("\n");
	
	printf ("Test 1.3: DLListBefore -- n nodes\n");
	// assign some random number of nodes.
	
	DLListBefore (emptyListDLListBefore, "2");
	DLListMove (emptyListDLListBefore, 3);
	DLListBefore (emptyListDLListBefore, "4");
	DLListBefore (emptyListDLListBefore, "10");
	DLListMove (emptyListDLListBefore, -1);
	DLListBefore (emptyListDLListBefore, "9");
	printf ("Current state of list:\n");
	putDLList (stdout, emptyListDLListBefore);
	printf ("\nNumber of nodes: %zu\n", DLListLength (emptyListDLListBefore));
	printf ("Inserting new node before %s: '69'\n", DLListCurrent (emptyListDLListBefore));
	DLListBefore (emptyListDLListBefore, "69");
	printf ("New list:\n");
	
	putDLList (stdout, emptyListDLListBefore);
	assert (validDLList (emptyListDLListBefore));
	printf ("\n");
	
	printf ("Test 1.4: DLListBefore -- end of list\n");
	printf ("Number of nodes: %zu\n", DLListLength (emptyListDLListBefore));
	printf ("Current node: %s\n", DLListCurrent (emptyListDLListBefore));
	printf ("Inserting new node before the last node: '89'\n");
	DLListMove (emptyListDLListBefore, (int)DLListLength (emptyListDLListBefore));
	DLListBefore (emptyListDLListBefore, "89");
	printf ("New list:\n");
	
	putDLList (stdout, emptyListDLListBefore);
	assert (validDLList (emptyListDLListBefore));
	
	printf ("\nTest one -- success\n\n");
	
	// START TEST TWO
	printf ("Stage two: DLListAfter\n\n");
	
	printf ("Test 2.1: DLListAfter -- empty list\n");
	printf ("Number of nodes: %zu\n", DLListLength (emptyListDLListAfter));
	printf ("Inserting new node: '0'\n");
	DLListAfter (emptyListDLListAfter, "0");
	printf ("New list:\n");
	putDLList (stdout, emptyListDLListAfter);
	assert (validDLList (emptyListDLListAfter));
	printf ("\n");
	
	printf ("Test 2.2: DLListAfter -- one node\n");
	printf ("Number of nodes: %zu\n", DLListLength (emptyListDLListAfter));
	printf ("Current node: %s\n", DLListCurrent (emptyListDLListAfter));
	printf ("Inserting new node after %s: '1'\n", DLListCurrent (emptyListDLListAfter));
	DLListAfter (emptyListDLListAfter, "1");
	printf ("New lsit:\n");
	putDLList (stdout, emptyListDLListAfter);
	assert (validDLList (emptyListDLListAfter));
	printf ("\n");
	
	printf ("Test 2.3: DLListAfter -- n nodes\n");
	// assign some random number of nodes.
	
	DLListAfter (emptyListDLListAfter, "2");
	DLListMove (emptyListDLListAfter, 3);
	DLListAfter (emptyListDLListAfter, "4");
	DLListAfter (emptyListDLListAfter, "10");
	DLListMove (emptyListDLListAfter, -1);
	DLListAfter (emptyListDLListAfter, "9");
	printf ("Current state of list:\n");
	putDLList (stdout, emptyListDLListAfter);
	printf ("\nNumber of nodes: %zu\n", DLListLength (emptyListDLListAfter));
	printf ("Inserting new node after %s: '69'\n", DLListCurrent (emptyListDLListAfter));
	DLListAfter (emptyListDLListAfter, "69");
	printf ("New list:\n");
	
	putDLList (stdout, emptyListDLListAfter);
	assert (validDLList (emptyListDLListAfter));
	printf ("\n");
	
	printf ("Test 2.4: DLListAfter -- end of list\n");
	printf ("Number of nodes: %zu\n", DLListLength (emptyListDLListAfter));
	printf ("Current node: %s\n", DLListCurrent (emptyListDLListAfter));
	printf ("Inserting new node after the last node: '89'\n");
	DLListMove (emptyListDLListAfter, (int)DLListLength (emptyListDLListAfter));
	DLListAfter (emptyListDLListAfter, "89");
	printf ("New list:\n");
	
	putDLList (stdout, emptyListDLListAfter);
	assert (validDLList (emptyListDLListAfter));
	
	printf ("\nStage two -- success\n\n");
	
	printf ("Stage three: DLListDelete\n");
	printf ("Test 3.1: DLListDelete -- start of list\n");
	printf ("Number of nodes: %zu\n", DLListLength (ListDLListDelete));
	printf ("Before deletion...\n");
	putDLList (stdout, ListDLListDelete);
	printf ("Deleting %s...\n", DLListCurrent (ListDLListDelete));
	DLListDelete (ListDLListDelete);
	printf ("After deletion...\n");
	putDLList (stdout, ListDLListDelete);
	assert (validDLList (ListDLListDelete));
	
	printf ("Test 3.2: DLListDelete -- end of list\n");
	printf ("Number of nodes: %zu\n", DLListLength (ListDLListDelete));
	printf ("Before deletion...\n");
	putDLList (stdout, ListDLListDelete);
	
	DLListMove (ListDLListDelete, (int)DLListLength (ListDLListDelete));
	printf ("Deleting %s ...\n", DLListCurrent (ListDLListDelete));
	DLListDelete (ListDLListDelete);
	printf ("After deletion...\n");
	putDLList (stdout, ListDLListDelete);
	assert (validDLList (ListDLListDelete));
	
	printf ("Test 3.3: DLListDelete -- delete nth node\n");
	printf ("Number of nodes: %zu\n", DLListLength (ListDLListDelete));
	printf ("Before deletion...\n");
	putDLList (stdout, ListDLListDelete);
	
	DLListMove (ListDLListDelete, -1);
	printf ("Deleting %s ...\n", DLListCurrent (ListDLListDelete));
	DLListDelete (ListDLListDelete);
	printf ("After deletion...\n");
	putDLList (stdout, ListDLListDelete);
	assert (validDLList (ListDLListDelete));
	
	// delete the rest.
	printf ("Test 3.4: DLListDelete -- delete the rest\n");
	printf ("Number of nodes: %zu\n", DLListLength (ListDLListDelete));
	printf ("Before deletion...\n");
	putDLList (stdout, ListDLListDelete);
	
	printf ("Deleting %s ...\n", DLListCurrent (ListDLListDelete));
	DLListDelete (ListDLListDelete);
	printf ("After deletion...\n");
	putDLList (stdout, ListDLListDelete);
	assert (validDLList (ListDLListDelete));
	
	printf ("Number of nodes: %zu\n", DLListLength (ListDLListDelete));
	printf ("Before deletion...\n");
	putDLList (stdout, ListDLListDelete);
	
	printf ("Deleting %s ...\n", DLListCurrent (ListDLListDelete));
	DLListDelete (ListDLListDelete);
	printf ("After deletion...\n");
	putDLList (stdout, ListDLListDelete);
	assert (validDLList (ListDLListDelete));
	
	// empty list.
	printf ("ListDLListNodeDelete: no more nodes\n\n");
	printf ("Stage three -- success\n");
	printf ("Testing complete!\n");

    // Avoid memory leak.
	freeDLList (emptyListDLListBefore);
	freeDLList (emptyListDLListAfter);
	freeDLList(ListDLListDelete);
	freeDLList (myList);
	
	return EXIT_SUCCESS;
}
