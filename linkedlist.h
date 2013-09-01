/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/

#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

// Do not create your own linkedlist_t variables, use the function
// linkedlist_create().
typedef struct linkedlist linkedlist_t;

//  ----------------------------------------------------------------------------
/// \brief  Create a new empty list.
/// \return Pointer to the new list.
//  ----------------------------------------------------------------------------
linkedlist_t *linkedlist_create(void);


//  ----------------------------------------------------------------------------
/// \brief  Link a new node at the end of the destination list.
/// \param  dst Destination list.
/// \param  data Pointer to the data content of the new node.
/// \return The updated list. This is useful if dst was empty before calling
/// this function.
//  ----------------------------------------------------------------------------
void linkedlist_add(linkedlist_t *dst, void * const data);


// ----------------------------------------------------------------------------
/// \brief Destroy the list passed as parameter. Only the list and its nodes are
/// destroyed, the data pointed to by each node needs to be destroyed separately
/// (probably before destroying the list itself).
/// \param list The list to destroy.
// ----------------------------------------------------------------------------
void linkedlist_destroy(linkedlist_t *list);


//  ----------------------------------------------------------------------------
/// \brief  Run the callback function passed as parameter on the data of all
/// nodes in the list passed as parameter. The callback may modify the data,
/// since the data itself is held by the client module.
/// \param  list The list to run the callback on.
/// \param  callback Function pointer to the function to run on data.
//  ----------------------------------------------------------------------------
void linkedlist_run_for_all(linkedlist_t *list,
                            void (*callback)(void * const data));


#endif // LINKEDLIST_H_INCLUDED
