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


// ----------------------------------------------------------------------------
/// \brief Copy the src list to a new dst list. If dst is not an empty list, its
/// content is destroyed before the copy operation. No memory used by the nodes
/// or the data of src is reused for dst. The nodes and data of dst are
/// allocated in this copy function (and freed on destroy).
/// \param dst Pointer to the list to copy to.
/// \param src Pointer to the list to copy.
/// \param data_size The size of one data slot.
// ----------------------------------------------------------------------------
void linkedlist_copy(linkedlist_t *dst, linkedlist_t *src,
                     unsigned int const data_size);


//  ----------------------------------------------------------------------------
/// \brief  Run the callback function passed as parameter on the data of all
/// nodes in the list passed as parameter. The callback may modify the data,
/// since the data itself is held by the client module.
/// \param  list The list to run the callback on.
/// \param  callback Function pointer to the function to run on data.
//  ----------------------------------------------------------------------------
void linkedlist_run_for_all(linkedlist_t *list,
                            void (*callback)(void * const data));


//  ----------------------------------------------------------------------------
/// \brief  Copy the list from position (0 is head) to its end, into
/// sublist. New nodes are created, there are no nodes being pointed to twice.
/// \param  list The list to copy from.
/// \param  sublist The list to copy to. Overwritten if not empty.
/// \param  position Where to start copying from in list.
/// \param  data_size The size in bytes of one data object.
//  ----------------------------------------------------------------------------
void linkedlist_sublist_copy(linkedlist_t * const sublist,
                             linkedlist_t * const list,
                             unsigned int const position,
                             unsigned int const data_size);

#endif // LINKEDLIST_H_INCLUDED
