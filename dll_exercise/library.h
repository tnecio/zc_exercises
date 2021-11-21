#ifndef DLL_EXERCISE_LIBRARY_H
#define DLL_EXERCISE_LIBRARY_H

typedef struct node {
    int data;
    struct node * right, * left;
} NODE;

/*
 * Inserts node pointed to by New to the right of the node pointed to by Prev.
 * Prev and New must not be NULL.
 * New must not be a member of any list (i.e. New->left and New->right must be NULL)
 */
void insert (NODE *Prev, NODE *New);

#endif //DLL_EXERCISE_LIBRARY_H
