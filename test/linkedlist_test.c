/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/

// Module under test.
#include "../linkedlist.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>


//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************
static int cumulative_sum;


//******************************************************************************
// Function prototypes
//******************************************************************************
// Helper functions.
static void cumulative_add_reset(void);
static void cumulative_add(void * const a);

// Test functions.
static void test_linkedlist_init(void);
static void test_linkedlist_run_for_all(void);


//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_linkedlist_init();
    test_linkedlist_run_for_all();
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_linkedlist_init(void)
{
    // Create a list a check that it is really created.
    // Destroy it and check that it is really destroyed.
    int data;
    linkedlist_t list = empty_list;
    list = linkedlist_add(list, &data);

    assert(list != empty_list);

    list = linkedlist_destroy(list);

    assert(list == empty_list);
}


// Helper function to be used as a callback. Its prototype is defined, which
// explains the use of a static variable of module scope.
static void cumulative_add(void * const a)
{
    cumulative_sum += *(int *) a;
}

// Helper function.
static void cumulative_add_reset(void)
{
    cumulative_sum = 0;
}

static void test_linkedlist_run_for_all(void)
{
    // Create a list and add all its elements together.
    linkedlist_t list = empty_list;
    int data[] = {1, 2, 3, 4, 5};
    int data_sum = 0;

    for (int index = 0; index < sizeof data / sizeof data[0]; index++) {
        list = linkedlist_add(list, &data[index]);
        data_sum += data[index];
    }
    cumulative_add_reset();
    linkedlist_run_for_all(list, cumulative_add);
    assert(cumulative_sum == data_sum);

    list = linkedlist_destroy(list);
}
