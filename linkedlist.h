/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/

#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

struct node;
typedef struct node node_t;
typedef node_t *linkedlist_t;

extern node_t * empty_list;

//  ----------------------------------------------------------------------------
/// \brief  Create and link a new node at the end of the destination list.
/// \param  dst Destination list.
/// \param  data Pointer to the data content of the new node.
/// \return The updated list. This is useful if dst was empty before calling
/// this function.
//  ----------------------------------------------------------------------------
linkedlist_t linkedlist_add(linkedlist_t dst, void * const data);


//  ----------------------------------------------------------------------------
/// \brief  Destroy the list passed as parameter. Only the list is destroyed,
/// the data pointed to by each node needs to be destroyed separately (probably
/// before destroying the list itself).
/// \param  list The list to destroy.
//  ----------------------------------------------------------------------------
linkedlist_t linkedlist_destroy(linkedlist_t list);

#endif // LINKEDLIST_H_INCLUDED
