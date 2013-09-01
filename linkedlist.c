/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/
#include "linkedlist.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct node_t {
    void *data; // Generic data type pointer.
    struct node_t *next;
} node_t;


struct linkedlist {
    node_t *head;
};

//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************

//******************************************************************************
// Function prototypes
//******************************************************************************
static void nodes_recursive_destroy(node_t *node_p);
static void nodes_run_for_all(node_t *node_p,
                              void (*callback)(void * const data));


//******************************************************************************
// Function definitions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Create a new empty list, with node pointer pointing to NULL.
/// \return Pointer to the new list.
//  ----------------------------------------------------------------------------
linkedlist_t *linkedlist_create(void)
{
    linkedlist_t *new_list_p = malloc(sizeof (linkedlist_t));
    new_list_p->head = NULL;
    return new_list_p;
}


//  ----------------------------------------------------------------------------
/// \brief  Add a new node at the end of a linked list, with content
/// data. Allocate memory for the new node, walk through the list to its end,
/// and link to the new node. If the list was empty before being added to, the
/// new start of list is the new node itseld.
//  ----------------------------------------------------------------------------
void linkedlist_add(linkedlist_t *dst, void * const data)
{
    if (dst == NULL) {
        // The list needs to be created first.
        return;
    }

    // Create a whole new node.
    node_t *new_node_p = malloc(sizeof (node_t));
    *new_node_p = (node_t) {
        .data = data,
        .next = NULL
    };

    if (dst->head == NULL) {
        // NULL head means this list was empy.
        dst->head = new_node_p;
    } else {
        // Walk to the last node.
        node_t *current_node = (node_t *) dst->head;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        // current_node is now the last node of dst.
        current_node->next = new_node_p;
    }
}


//  ----------------------------------------------------------------------------
/// \brief  Free the first node of the list passed as parameter, then call
/// itself on the rest of the list until there are no more nodes to free.
//  ----------------------------------------------------------------------------
void linkedlist_destroy(linkedlist_t *list)
{
    nodes_recursive_destroy(list->head);
    free(list);
}


//  ----------------------------------------------------------------------------
/// \brief  Run the callback function on the data of the first node, then call
/// itself on the rest of the list until there are no more nodes.
//  ----------------------------------------------------------------------------
void linkedlist_run_for_all(linkedlist_t *list,
                            void (*callback) (void * const data))
{
    nodes_run_for_all(list->head, callback);
}


//******************************************************************************
// Internal functions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Free the node passed as parameter, and run itself on the next node
/// in the list.
/// \param  Pointer to the node to free.
//  ----------------------------------------------------------------------------
static void nodes_recursive_destroy(node_t *node_p)
{
    if (node_p == NULL) {
        return;
    } else {
        node_t *rest_of_nodes = (*node_p).next;
        free(node_p);
        nodes_recursive_destroy(rest_of_nodes);
    }
}


//  ----------------------------------------------------------------------------
/// \brief  Run the callback on the data field of the node passed as parameter,
/// and run itself on the next node in the list.
/// \param  node_p Pointer to the node that has data to run the callback on.
/// \param  callback The function to run on the node's data.
//  ----------------------------------------------------------------------------
static void nodes_run_for_all(node_t *node_p,
                              void (*callback)(void * const data))
{
    if (node_p == NULL) {
        return;
    } else {
        callback(node_p->data);
        nodes_run_for_all(node_p->next, callback);
    }
}
