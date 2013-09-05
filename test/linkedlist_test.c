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
// Module macros
//******************************************************************************
// Number of elements in array x.
#define NUMBER_OF_ELEMENTS(x) (sizeof (x) / sizeof (x[0]))


//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************
static int cumulative_sum;
static int read_array[20];
static unsigned int read_array_current_index;


//******************************************************************************
// Function prototypes
//******************************************************************************
// Helper functions.
static void cumulative_add_reset(void);
static void cumulative_add(void const * const a);
static void display(void const * const a);
static void list_populate(linkedlist_t * const list,
                          const int data[],
                          const unsigned int number_of_elements);
static void list_read_to_array_reset(void);
static void list_read_to_array(void const * const a);


// Test functions.
static void test_linkedlist_init(void);
static void test_linkedlist_run_for_all(void);
static void test_linkedlist_copy(void);
static void test_linkedlist_sublist_copy(void);
static void test_linkedlist_cross(void);
static void test_linkedlist_cross_at_0(void);


//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_linkedlist_init();
    test_linkedlist_run_for_all();
    test_linkedlist_copy();
    test_linkedlist_sublist_copy();
    test_linkedlist_cross();
    test_linkedlist_cross_at_0();
    printf("All tests passed.\n");
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
static void cumulative_add(void const * const a)
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

    for (int index = 0; index < NUMBER_OF_ELEMENTS(data); index++) {
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
    for (int index = 0; index < NUMBER_OF_ELEMENTS(data); index++) {
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


static void test_linkedlist_sublist_copy(void)
{
    const unsigned int node_index = 2;
    int data[] = {1, 2, 3, 4, 5};
    int data_sum = 0;

    linkedlist_t *list = linkedlist_create();
    list_populate(list, data, NUMBER_OF_ELEMENTS(data));

    for (int index = node_index; index < NUMBER_OF_ELEMENTS(data); index++) {
        data_sum += data[index];
    }

    linkedlist_t *sublist = linkedlist_create();
    linkedlist_sublist_copy(sublist, list, node_index, sizeof data[0]);

    cumulative_add_reset();
    linkedlist_run_for_all(sublist, cumulative_add);

    assert(cumulative_sum == data_sum);

    linkedlist_destroy(list);
    linkedlist_destroy(sublist);
}


static void test_linkedlist_cross(void)
{
    const int data_a[] = {1, 2, 3, 4, 5};
    const int data_b[] = {11, 12, 13, 14, 15, 16, 17};
    const unsigned int pos_a = 4;
    const unsigned int pos_b = 2;
    // After crossing the lists at the positions above, the results must be:
    // data_a[] == {1, 2, 3, 4, 13, 14, 15, 16, 17}
    // data_b[] == {11, 12, 5}

    int data_sum_a = 0;
    int data_sum_b = 0;

    // Generate the test sums.
    unsigned int index;
    for (index = 0; index < pos_a; index++) {
        data_sum_a += data_a[index];
    }
    for (index = pos_b; index < NUMBER_OF_ELEMENTS(data_b); index++) {
        data_sum_a += data_b[index];
    }

    for (index = 0; index < pos_b; index++) {
        data_sum_b += data_b[index];
    }
    for (index = pos_a ; index < NUMBER_OF_ELEMENTS(data_a); index++) {
        data_sum_b += data_a[index];
    }

    // Create the lists, cross them and test.
    linkedlist_t *list_a = linkedlist_create();
    linkedlist_t *list_b = linkedlist_create();

    list_populate(list_a, data_a, NUMBER_OF_ELEMENTS(data_a));
    list_populate(list_b, data_b, NUMBER_OF_ELEMENTS(data_b));

    linkedlist_cross(list_a, pos_a, list_b, pos_b);

    cumulative_add_reset();
    linkedlist_run_for_all(list_a, cumulative_add);
    assert(cumulative_sum == data_sum_a);

    cumulative_add_reset();
    linkedlist_run_for_all(list_b, cumulative_add);
    assert(cumulative_sum == data_sum_b);

    linkedlist_destroy(list_a);
    linkedlist_destroy(list_b);
}


static void test_linkedlist_cross_at_0(void)
{
    const int data_a[] = {1, 2, 3, 4};
    const int data_b[] = {11, 12, 13, 14};
    unsigned int pos_a = 0;
    unsigned int pos_b = 2;
    const int result_a[] = {13, 14};
    const int result_b[] = {11, 12, 1, 2, 3, 4};

    // Create the lists, cross them and test.
    linkedlist_t *list_a = linkedlist_create();
    linkedlist_t *list_b = linkedlist_create();

    list_populate(list_a, data_a, NUMBER_OF_ELEMENTS(data_a));
    list_populate(list_b, data_b, NUMBER_OF_ELEMENTS(data_b));

    linkedlist_cross(list_a, pos_a, list_b, pos_b);

    list_read_to_array_reset();
    linkedlist_run_for_all(list_a, list_read_to_array);

    for (unsigned int i = 0; i < NUMBER_OF_ELEMENTS(result_a); i++) {
        assert(read_array[i] == result_a[i]);
    }

    list_read_to_array_reset();
    linkedlist_run_for_all(list_b, list_read_to_array);

    for (unsigned int i = 0; i < NUMBER_OF_ELEMENTS(result_b); i++) {
        assert(read_array[i] == result_b[i]);
    }

    linkedlist_destroy(list_a);
    linkedlist_destroy(list_b);
}

static void list_read_to_array_reset(void)
{
    for (unsigned int i = 0; i < NUMBER_OF_ELEMENTS(read_array); i++) {
        read_array[i] = 0;
    }
    read_array_current_index = 0;
}


static void list_read_to_array(void const * const a)
{
    read_array[read_array_current_index] = * (int *) a;
    read_array_current_index++;

    if (read_array_current_index >= NUMBER_OF_ELEMENTS(read_array)) {
        fprintf(stderr, "list_read_to_array: index out of range.\n");
        read_array_current_index = 0;
    }
}


static void display(void const * const a)
{
    printf("d: %d\n", *(int *) a);
}


static void list_populate(linkedlist_t * const list,
                          const int data[],
                          const unsigned int number_of_elements)
{
    for (unsigned int index = 0; index < number_of_elements; index++) {
        linkedlist_add(list, &data[index]);
    }
}
