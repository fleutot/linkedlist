/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/

// Module under test.
#include "../linkedlist.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

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
static void display(void * const a);

// Test functions.
static void test_linkedlist_init(void);
static void test_linkedlist_run_for_all(void);
static void test_linkedlist_copy(void);


//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_linkedlist_init();
    test_linkedlist_run_for_all();
    test_linkedlist_copy();
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_linkedlist_init(void)
{
    linkedlist_t *list_p = linkedlist_create();

    assert(list_p != NULL);

    linkedlist_destroy(list_p);
}


// Helper function to be used as a callback. Its prototype much match that of
// the callback parameter of linkedlist_run_for_all(), which explains the use of
// a static variable of module scope.
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
    linkedlist_t *list_p = linkedlist_create();
    int data[] = {1, 2, 3, 4, 5};
    int data_sum = 0;

    for (int index = 0; index < sizeof data / sizeof data[0]; index++) {
        linkedlist_add(list_p, &data[index]);
        data_sum += data[index];    // For later comparison.
    }
    cumulative_add_reset();
    linkedlist_run_for_all(list_p, cumulative_add);

    assert(cumulative_sum == data_sum);

    linkedlist_destroy(list_p);
}


static void test_linkedlist_copy(void)
{
    int data[] = {1, 2, 3, 4, 5};
    int data_sum = 0;

    linkedlist_t *src = linkedlist_create();
    for (int index = 0; index < sizeof data / sizeof data[0]; index++) {
        linkedlist_add(src, &data[index]);
        data_sum += data[index];    // For later comparison.
    }

    linkedlist_t *dst = linkedlist_create();
    linkedlist_copy(dst, src, sizeof data[0]);

    cumulative_add_reset();
    linkedlist_run_for_all(dst, cumulative_add);

    assert(cumulative_sum == data_sum);

    linkedlist_destroy(src);
    linkedlist_destroy(dst);
}


static void display(void * const a)
{
    printf("d: %d\n", *(int *) a);
}
