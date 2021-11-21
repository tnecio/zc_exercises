#include <stddef.h>
#include <assert.h>

#include "library.h"

void test_insert_happypath() {
    // Given we have a double-linked list with 2 elements
    NODE Nodes[] = {{1, NULL, NULL},
                    {3, NULL, NULL} };
    Nodes[0].right = &(Nodes[1]);
    Nodes[1].left = &(Nodes[0]);

    // When we insert a node in the middle and the end
    // (We cannot insert in the beginning because of how 'insert' has been specified,
    // we can only insert a node to the right of an existing one, never to the left).
    NODE NewMiddle = {2, NULL, NULL};
    insert(&(Nodes[0]), &NewMiddle);
    NODE NewEnd = {4, NULL, NULL};
    insert(&(Nodes[1]), &NewEnd);

    // Then the iteration over the list in the left direction
    // should yield the expected values, iterating in the right
    // should yield the same values in reverse order
    int Expected[] = {1, 2, 3, 4};
    size_t ExpectedLen = sizeof(Expected) / sizeof(int);
    size_t i = 0;
    for (NODE *Node = &(Nodes[0]); Node != NULL; Node = Node->right) {
        assert( Node->data == Expected[i] );
        i++;
    }
    assert( i == ExpectedLen );
    for (NODE *Node = &NewEnd; Node != NULL; Node = Node->left) {
        i--;
        assert( Node->data == Expected[i] );
    }
    assert( i == 0 );
}

int main() {
    test_insert_happypath();
    return 0;
}
