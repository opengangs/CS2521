#include "BSTree.h"

#include <stdlib.h>

BSTree BSTreeGetSmallest(BSTree t) {
	
	if (t == NULL) return NULL;

	// find the smallest on the left subtree.
	if (t->left != NULL) {
		t = BSTreeGetSmallest (t->left);
	}

	return t;
}
