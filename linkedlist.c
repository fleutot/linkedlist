/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/
#include "linkedlist.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t {
    bool allocated_here;    // flag to free memory at destroy.
    void const *data;       // Generic data type pointer.
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
                              void (*callback)(void const * const data));
static node_t *nodes_recursive_copy(node_t *src,
                                    unsigned int const data_size);


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
void linkedlist_add(linkedlist_t *dst, void const * const data)
{
    if (dst == NULL) {
        fprintf(stderr,
                "linkedlist_add: the list needs to be created first.\n");
        return;
    }

    // Create a whole new node.
    node_t *new_node_p = malloc(sizeof (node_t));
    *new_node_p = (node_t) {
        .allocated_here = false,
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
                            void (*callback) (void const * const data))
{
    if (list != NULL) {
        nodes_run_for_all(list->head, callback);
    } else {
        fprintf(stderr, "linkedlist_run_for_all: list is NULL.\n");
    }
}


//  ----------------------------------------------------------------------------
/// \brief  Destroy a possibly non-empty dst list and fill it with a copy of
/// src. Both nodes and data are copied to new locations, no sharing of memory
/// between src and dst.
//  ----------------------------------------------------------------------------
void linkedlist_copy(linkedlist_t *dst, linkedlist_t *src,
                     unsigned int const data_size)
{
    if (dst->head != NULL) {
        linkedlist_destroy(dst);
    }
    dst->head = nodes_recursive_copy(src->head, data_size);
}


//  ----------------------------------------------------------------------------
/// \brief  Destroy a possibly non-empty sublist and fill it with a copy of list
/// from position to its end.
//  ----------------------------------------------------------------------------
void linkedlist_sublist_copy(linkedlist_t * const sublist,
                             linkedlist_t * const list,
                             unsigned int const position,
                             unsigned int const data_size)
{
    if (list == NULL || sublist == NULL) {
        fprintf(stderr, "linkedlist_node_get: list or sublist is NULL.\n");
        return;
    }

    node_t *walker = list->head;

    for (int index = 0; index < position; index++) {
        walker = walker->next;
    }

    if (sublist->head != NULL) {
        linkedlist_destroy(sublist);
    }

    sublist->head = nodes_recursive_copy(walker, data_size);
}


//  ----------------------------------------------------------------------------
/// \brief  Cross lists by changing the .next pointer of the node before
/// position. Special case when trying to cross from index 0 is taken care of.
//  ----------------------------------------------------------------------------
void linkedlist_cross(linkedlist_t * const list_a,
                      unsigned int const pos_a,
                      linkedlist_t * const list_b,
                      unsigned int const pos_b)
{
    if (list_a == NULL || list_b == NULL) {
        fprintf(stderr, "linkedlist_cross: one of the input lists is NULL.\n");
        return;
    }

    node_t *walker_a = list_a->head;
    // -1 because its the node *pointing to* pos that needs changing.
    // index needs to be signed, comparison value may be negative.
    for (int index = 0; index < (int) pos_a - 1; index++) {
        walker_a = walker_a->next;
        if (walker_a == NULL) {
            fprintf(stderr, "linkedlist_cross: list_a reached unexpected termination.\n");
            return;
        }
    }

    node_t *walker_b = list_b->head;
    for (int index = 0; index < (int) pos_b - 1; index++) {
        walker_b = walker_b->next;
        if (walker_b == NULL) {
            fprintf(stderr, "linkedlist_cross: list_b reached unexpected termination.\n");
            return;
        }
    }

    node_t *start_of_new_part_for_list_b = walker_a->next;

    if (pos_a == 0) {
        start_of_new_part_for_list_b = list_a->head;
        list_a->head = walker_b->next;
    } else {
        walker_a->next = walker_b->next;
    }

    if (pos_b == 0) {
        list_b->head = walker_a->next;
    } else {
        walker_b->next = start_of_new_part_for_list_b;
    }
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
        if (node_p->allocated_here == true) {
            free((void *) node_p->data);
        }
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
                              void (*callback)(void const * const data))
{
    if (node_p == NULL) {
        return;
    } else {
        callback(node_p->data);
        nodes_run_for_all(node_p->next, callback);
    }
}


//  ----------------------------------------------------------------------------
/// \brief  Allocate memory for new data (content copied from src) and a new
/// node. Call itself to copy the next node of src (and populate the current
/// .next with the returned value).
/// \param  src Node to copy from.
/// \param  data_size Data block size.
/// \return Pointer to the newly created node.
//  ----------------------------------------------------------------------------
node_t *nodes_recursive_copy(node_t * const src,
                             unsigned int const data_size)
{
    if (src == NULL) {
        return NULL;
    }

    // Allocate a new data object and copy.
    void *new_data = malloc(data_size);
    if (new_data != NULL) {
        memcpy(new_data, src->data, data_size);
    }

    // Create a whole new node.
    node_t *new_node_p = malloc(sizeof (node_t));
    *new_node_p = (node_t) {
        .allocated_here = true, // for freeing memory at destroy.
        .data = new_data,
        .next = nodes_recursive_copy(src->next, data_size)
    };

    return new_node_p;
}
