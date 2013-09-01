/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/
#include "linkedlist.h"

#include <stddef.h>
#include <stdlib.h>

struct node {
    void *data; // Generic data type pointer.
    struct node *next;
};

//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************
node_t *empty_list = NULL;


//******************************************************************************
// Function prototypes
//******************************************************************************

//******************************************************************************
// Function definitions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Add a new node at the end of a linked list, with content
/// data. Allocate memory for the new node, walk through the list to its end,
/// and link to the new node. If the list was empty before being added to, the
/// new start of list is the new node itseld.
//  ----------------------------------------------------------------------------
linkedlist_t linkedlist_add(linkedlist_t dst, void * const data)
{
    // Create a whole new node.
    node_t *new_node = malloc(sizeof (node_t));
    *new_node = (node_t) {
        .data = data,
        .next = NULL
    };

    if (dst == NULL) {
        // NULL destination means this is a whole new list.
        return (linkedlist_t) new_node;
    } else {
        node_t *current_node = (node_t *) dst;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        // current_node is now the last node of dst.
        current_node->next = new_node;
        return dst;
    }
}


//  ----------------------------------------------------------------------------
/// \brief  Free the first node of the list passed as parameter, then call
/// itself on the rest of the list until there are no more nodes to free.
//  ----------------------------------------------------------------------------
linkedlist_t linkedlist_destroy(linkedlist_t list)
{
    if (list == NULL) {
        return NULL;
    } else {
        linkedlist_t rest_of_list = (*list).next;
        free( (node_t *) list);
        return linkedlist_destroy(rest_of_list);
    }
}

//******************************************************************************
// Internal functions
//******************************************************************************
