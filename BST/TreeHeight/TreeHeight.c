
#include <stdlib.h>

#include "tree.h"

int TreeHeight(Tree t) {
    if (t == NULL) return -1;

    int lheight = TreeHeight (t->left);
    int rheight = TreeHeight (t->right);

    return 1 + (lheight > rheight ? lheight : rheight);
}

