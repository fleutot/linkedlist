/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Ostervall
----------------------------------------------------------------------------*/

// Module under test.
#include "../linkedlist.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <malloc.h>

//******************************************************************************
// Module macros
//******************************************************************************
// Number of elements in array x.
#define NB_ELEMENTS(x) (sizeof (x) / sizeof (x[0]))


//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************
static int read_array[20];
static unsigned int read_array_current_index;


//******************************************************************************
// Function prototypes
//******************************************************************************
// Helper functions.
static void display(void const * const a);
static void list_populate(linkedlist_t * const list,
                          const int data[],
                          const unsigned int number_of_elements);
static void list_read_to_array_reset(void);
static void list_read_to_array(void const * const a);
static bool int_arrays_equal(int const * const a,
                             int const * const b,
                             const unsigned int size);

// Test functions.
static void test_linkedlist_init(void);
static void test_linkedlist_run_for_all(void);
static void test_linkedlist_copy(void);
static void test_linkedlist_copy_overwrite(void);
static void test_linkedlist_sublist_copy(void);
static void test_linkedlist_cross(void);
static void test_linkedlist_cross_at_0(void);
static void test_linkedlist_data_handle_get(void);
static void test_linkedlist_cross_long(void);


//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_linkedlist_init();
    test_linkedlist_run_for_all();
    test_linkedlist_copy();
    test_linkedlist_copy_overwrite();
    test_linkedlist_sublist_copy();
    test_linkedlist_cross();
    test_linkedlist_cross_at_0();
    test_linkedlist_cross_long();
    test_linkedlist_data_handle_get();
    printf("All tests passed.\n");
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_linkedlist_init(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    linkedlist_t *list_p = linkedlist_create();

    assert(list_p != NULL);

    linkedlist_destroy(list_p);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_run_for_all(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    int data[] = {1, 2, 3, 4, 5};

    linkedlist_t *list = linkedlist_create();
    list_populate(list, data, NB_ELEMENTS(data));

    list_read_to_array_reset();
    linkedlist_run_for_all(list, list_read_to_array);

    assert(int_arrays_equal(data, read_array, NB_ELEMENTS(data)));

    linkedlist_destroy(list);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_copy(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    int data[] = {1, 2, 3, 4, 5};

    linkedlist_t *src = linkedlist_create();
    list_populate(src, data, NB_ELEMENTS(data));

    linkedlist_t *dst = linkedlist_create();
    linkedlist_copy(dst, src, sizeof data[0]);

    list_read_to_array_reset();
    linkedlist_run_for_all(dst, list_read_to_array);

    assert(int_arrays_equal(data, read_array, NB_ELEMENTS(data)));

    assert(linkedlist_size_get(src) == linkedlist_size_get(dst));

    linkedlist_destroy(src);
    linkedlist_destroy(dst);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_copy_overwrite(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {11, 12, 13, 14, 15};

    linkedlist_t *src1 = linkedlist_create();
    list_populate(src1, data1, NB_ELEMENTS(data1));

    linkedlist_t *src2 = linkedlist_create();
    list_populate(src2, data2, NB_ELEMENTS(data2));

    linkedlist_t *dst = linkedlist_create();
    linkedlist_copy(dst, src1, sizeof data1[0]);

    linkedlist_copy(dst, src2, sizeof data2[0]);

    list_read_to_array_reset();
    linkedlist_run_for_all(dst, list_read_to_array);

    assert(int_arrays_equal(data2, read_array, NB_ELEMENTS(data2)));

    assert(linkedlist_size_get(src2) == linkedlist_size_get(dst));

    linkedlist_destroy(src1);
    linkedlist_destroy(src2);
    linkedlist_destroy(dst);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_sublist_copy(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    const unsigned int node_index = 2;
    int data[] = {1, 2, 3, 4, 5};
    int result[] = {3, 4, 5};

    linkedlist_t *list = linkedlist_create();
    list_populate(list, data, NB_ELEMENTS(data));

    linkedlist_t *sublist = linkedlist_create();
    linkedlist_sublist_copy(sublist, list, node_index, sizeof data[0]);

    list_read_to_array_reset();
    linkedlist_run_for_all(sublist, list_read_to_array);

    assert(int_arrays_equal(result, read_array, NB_ELEMENTS(result)));

    assert(linkedlist_size_get(list) - node_index
           == linkedlist_size_get(sublist));

    linkedlist_destroy(list);
    linkedlist_destroy(sublist);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_cross(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    const int data_a[] = {1, 2, 3, 4, 5};
    const int data_b[] = {11, 12, 13, 14, 15, 16, 17};
    const unsigned int pos_a = 4;
    const unsigned int pos_b = 2;
    const int result_a[] = {1, 2, 3, 4, 13, 14, 15, 16, 17};
    const int result_b[] = {11, 12, 5};

    // Create the lists, cross them and test.
    linkedlist_t *list_a = linkedlist_create();
    linkedlist_t *list_b = linkedlist_create();

    list_populate(list_a, data_a, NB_ELEMENTS(data_a));
    list_populate(list_b, data_b, NB_ELEMENTS(data_b));

    linkedlist_cross(list_a, pos_a, list_b, pos_b);

    list_read_to_array_reset();
    linkedlist_run_for_all(list_a, list_read_to_array);
    assert(int_arrays_equal(result_a, read_array, NB_ELEMENTS(result_a)));
    assert(linkedlist_size_get(list_a) == NB_ELEMENTS(result_a));

    list_read_to_array_reset();
    linkedlist_run_for_all(list_b, list_read_to_array);
    assert(int_arrays_equal(result_b, read_array, NB_ELEMENTS(result_b)));
    assert(linkedlist_size_get(list_b) == NB_ELEMENTS(result_b));

    linkedlist_destroy(list_a);
    linkedlist_destroy(list_b);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_cross_at_0(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    const int data_a[] = {1, 2, 3, 4};
    const int data_b[] = {11, 12, 13, 14};
    unsigned int pos_a = 0;
    unsigned int pos_b = 2;
    const int result_a[] = {13, 14};
    const int result_b[] = {11, 12, 1, 2, 3, 4};

    // Create the lists, cross them and test.
    linkedlist_t *list_a = linkedlist_create();
    linkedlist_t *list_b = linkedlist_create();

    list_populate(list_a, data_a, NB_ELEMENTS(data_a));
    list_populate(list_b, data_b, NB_ELEMENTS(data_b));

    linkedlist_cross(list_a, pos_a, list_b, pos_b);

    list_read_to_array_reset();
    linkedlist_run_for_all(list_a, list_read_to_array);
    assert(int_arrays_equal(result_a, read_array, NB_ELEMENTS(result_a)));
    assert(linkedlist_size_get(list_a) == NB_ELEMENTS(result_a));

    list_read_to_array_reset();
    linkedlist_run_for_all(list_b, list_read_to_array);
    assert(int_arrays_equal(result_b, read_array, NB_ELEMENTS(result_b)));
    assert(linkedlist_size_get(list_b) == NB_ELEMENTS(result_b));

    linkedlist_destroy(list_a);
    linkedlist_destroy(list_b);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_cross_long(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    const int data_a[LINKEDLIST_MAX_SIZE - 10] = {0};
    const int data_b[20] = {0};
    const int pos_a = 1;
    const int pos_b = 19;

    linkedlist_t *list_a = linkedlist_create();
    linkedlist_t *list_b = linkedlist_create();

    list_populate(list_a, data_a, NB_ELEMENTS(data_a));
    list_populate(list_b, data_b, NB_ELEMENTS(data_b));

    linkedlist_cross(list_a, pos_a, list_b, pos_b);

    assert(linkedlist_size_get(list_a) == 2);
    assert(linkedlist_size_get(list_b) == LINKEDLIST_MAX_SIZE);

    linkedlist_destroy(list_a);
    linkedlist_destroy(list_b);
    fprintf(stdout, "OK\n");
}


static void test_linkedlist_data_handle_get(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    const int data[] = {11, 12, 13, 14, 15};
    const unsigned int pos = 3;

    linkedlist_t *list = linkedlist_create();
    list_populate(list, data, NB_ELEMENTS(data));

    int *result_ptr = (int *) linkedlist_data_handle_get(list, pos);

    assert(result_ptr != NULL);
    assert(*result_ptr == data[pos]);

    // Test the wrap around functionality.
    const unsigned int other_pos = 2;
    result_ptr = (int *) linkedlist_data_handle_get(
        list,
        other_pos + NB_ELEMENTS(data)
        );

    assert(result_ptr != NULL);
    assert(*result_ptr == data[other_pos]);

    linkedlist_destroy(list);
    fprintf(stdout, "OK\n");
}


//------------------------------------------------------------------------------
// Helper functions
//------------------------------------------------------------------------------
static void list_read_to_array_reset(void)
{
    for (unsigned int i = 0; i < NB_ELEMENTS(read_array); i++) {
        read_array[i] = 0;
    }
    read_array_current_index = 0;
}

// This function is meant to be used as a parameter of linkedlist_run_for_all,
// hence the parameter format.
static void list_read_to_array(void const * const a)
{
    read_array[read_array_current_index] = * (int *) a;
    read_array_current_index++;

    if (read_array_current_index >= NB_ELEMENTS(read_array)) {
        fprintf(stderr, "list_read_to_array: index out of range.\n");
        read_array_current_index = 0;
    }
}


// This function is meant to be used as a parameter of linkedlist_run_for_all,
// hence the parameter format.
static void display(void const * const a)
{
    printf("d: %d\n", *(int *) a);
}


static void list_populate(linkedlist_t * const list,
                          const int data[],
                          const unsigned int number_of_elements)
{
    for (unsigned int i = 0; i < number_of_elements; i++) {
        int *new_data_object = malloc(sizeof(int));
        if (new_data_object == NULL) {
            fprintf(stderr, "%s: new_data_object is NULL.\n", __func__);
            linkedlist_destroy(list);
            return;
        }
        *new_data_object = data[i];
        linkedlist_add(list, new_data_object);
    }
}

static bool int_arrays_equal(int const * const a,
                             int const * const b,
                             const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}
