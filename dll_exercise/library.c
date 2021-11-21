#include "library.h"

/*
 * Inserts node pointed to by New to the right of the node pointed to by Prev.
 * Caller is responsible for ensuring that 'New' will live at least as long as the list.
 * Prev and New must not be NULL.
 * New must not be a member of any list (i.e. New->left and New->right must be NULL)
 */
void insert (NODE *Prev, NODE *New) {
    if (!Prev || !New) {
        return;
    }
    if (New->left || New->right) {
        return;
    }

    NODE *OldRight = Prev->right; // Might be NULL
    Prev->right = New;
    New->left = Prev;
    New->right = OldRight;
    if (OldRight) OldRight->left = New;
}
