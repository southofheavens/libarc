#include <sorting/counting_sort.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static size_t 
int_key(const void *ptr)
{
    return (size_t)(*(const int *)ptr);
}

typedef struct 
{
    char name[32];
    size_t age;
} Person;

static size_t
person_age_key(const void *ptr)
{
    const Person *person = ptr;
    return person->age;
}

static void
test_empty_array(void)
{
    counting_sort(NULL, 0, sizeof(int), int_key);
}

static void
test_single_element(void)
{
    int actual[] = {42};
    int expected[] = {42};

    enum { ARRAY_SIZE = 1 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_sorted_array(void)
{
    int actual[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3, 4, 5};

    enum { ARRAY_SIZE = 5 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_reverse_sorted_array(void)
{
    int actual[] = {5, 4, 3, 2, 1};
    int expected[] = {1, 2, 3, 4, 5};

    enum { ARRAY_SIZE = 5 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_duplicate_keys(void)
{
    int actual[] = {4, 2, 2, 5, 1, 4, 3, 2};
    int expected[] = {1, 2, 2, 2, 3, 4, 4, 5};

    enum { ARRAY_SIZE = 8 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_zero_keys(void)
{
    int actual[] = {0, 0, 0, 0};
    int expected[] = {0, 0, 0, 0};

    enum { ARRAY_SIZE = 4 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_large_key_range(void)
{
    int actual[] = {1000, 500, 999, 0, 1000, 1};
    int expected[] = {0, 1, 500, 999, 1000, 1000};

    enum { ARRAY_SIZE = 6 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_structs(void)
{
    Person actual[] = 
    {
        {"Alex", 25},
        {"John", 19},
        {"Kate", 30},
        {"Mike", 21}
    };

    Person expected[] = 
    {
        {"John", 19},
        {"Mike", 21},
        {"Alex", 25},
        {"Kate", 30}
    };

    enum { ARRAY_SIZE = 4 };

    counting_sort(actual, ARRAY_SIZE, sizeof(Person), person_age_key);

    size_t i;
    for (i = 0; i < ARRAY_SIZE; ++i) 
    {
        assert(actual[i].age == expected[i].age);
        assert(strcmp(actual[i].name, expected[i].name) == 0);
    }
}

static void
test_stability(void)
{
    Person actual[] = 
    {
        {"Alex", 25},
        {"John", 20},
        {"Kate", 25},
        {"Mike", 20},
        {"Anna", 25}
    };

    Person expected[] = {
        {"John", 20},
        {"Mike", 20},
        {"Alex", 25},
        {"Kate", 25},
        {"Anna", 25}
    };

    enum { ARRAY_SIZE = 5 };

    counting_sort(actual, ARRAY_SIZE, sizeof(Person), person_age_key);

    size_t i;
    for (i = 0; i < ARRAY_SIZE; ++i) 
    {
        assert(actual[i].age == expected[i].age);
        assert(strcmp(actual[i].name, expected[i].name) == 0);
    }
}

static void
test_two_elements(void)
{
    int actual[] = {2, 1};
    int expected[] = {1, 2};

    enum { ARRAY_SIZE = 2 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_all_equal_elements(void)
{
    int actual[] = {7, 7, 7, 7, 7, 7};
    int expected[] = {7, 7, 7, 7, 7, 7};

    enum { ARRAY_SIZE = 6 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_alternating_keys(void)
{
    int actual[] = {1, 5, 1, 5, 1, 5, 1, 5};
    int expected[] = {1, 1, 1, 1, 5, 5, 5, 5};

    enum { ARRAY_SIZE = 8 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_sparse_keys(void)
{
    int actual[] = {10000, 1, 5000, 100, 9999, 2};
    int expected[] = {1, 2, 100, 5000, 9999, 10000};

    enum { ARRAY_SIZE = 6 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

static void
test_repeated_min_and_max(void)
{
    int actual[] = {9, 0, 9, 0, 9, 0, 9, 0};
    int expected[] = {0, 0, 0, 0, 9, 9, 9, 9};

    enum { ARRAY_SIZE = 8 };

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

typedef struct 
{
    size_t key;
    int id;
} Item;

static size_t
item_key(const void *ptr)
{
    return ((const Item *)ptr)->key;
}

static void
test_stability_with_ids(void)
{
    Item actual[] =
    {
        {3, 10},
        {1, 20},
        {3, 30},
        {2, 40},
        {1, 50},
        {3, 60},
        {2, 70}
    };

    Item expected[] = 
    {
        {1, 20},
        {1, 50},
        {2, 40},
        {2, 70},
        {3, 10},
        {3, 30},
        {3, 60}
    };

    enum { ARRAY_SIZE = 7 };

    counting_sort(actual, ARRAY_SIZE, sizeof(Item), item_key);

    size_t i;
    for (i = 0; i < ARRAY_SIZE; ++i) 
    {
        assert(actual[i].key == expected[i].key);
        assert(actual[i].id == expected[i].id);
    }
}

typedef struct 
{
    unsigned char key;
    unsigned char payload[127];
} LargeItem;

static size_t
large_item_key(const void *ptr)
{
    return ((const LargeItem *)ptr)->key;
}

static void
test_large_objects(void)
{
    LargeItem actual[] = 
    {
        {3, {0}},
        {1, {1}},
        {2, {2}},
        {1, {3}}
    };

    LargeItem expected[] = 
    {
        {1, {1}},
        {1, {3}},
        {2, {2}},
        {3, {0}}
    };

    enum { ARRAY_SIZE = 4 };

    counting_sort(actual, ARRAY_SIZE, sizeof(LargeItem), large_item_key);

    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        assert(actual[i].key == expected[i].key);
        assert(memcmp(actual[i].payload, expected[i].payload, sizeof(actual[i].payload)) == 0);
    }
}

static void
test_single_distinct_key(void)
{
    Person actual[] =
    {
        {"Alex", 42},
        {"John", 42},
        {"Kate", 42},
        {"Mike", 42}
    };

    Person expected[] =
    {
        {"Alex", 42},
        {"John", 42},
        {"Kate", 42},
        {"Mike", 42}
    };

    enum { ARRAY_SIZE = 4 };

    counting_sort(actual, ARRAY_SIZE, sizeof(Person), person_age_key);

    for (size_t i = 0; i < ARRAY_SIZE; ++i)
    {
        assert(strcmp(actual[i].name, expected[i].name) == 0);
        assert(actual[i].age == expected[i].age);
    }
}

static int 
int_compar(const void *lhs, const void *rhs)
{
    int a = *(const int *)lhs;
    int b = *(const int *)rhs;

    return (a > b) - (a < b);
}

static void 
test_huge_array(void)
{
    enum { ARRAY_SIZE = 1000000 };

    int *actual = malloc(ARRAY_SIZE * sizeof(int));
    int *expected = malloc(ARRAY_SIZE * sizeof(int));

    assert(actual != NULL);
    assert(expected != NULL);

    uint32_t seed = 123456789;

    size_t i;
    for (i = 0; i < ARRAY_SIZE; ++i) 
    {
        seed = seed * 1664525u + 1013904223u;
        actual[i] = (int)(seed % 10000);
    }

    memcpy(expected, actual, ARRAY_SIZE * sizeof(int));

    qsort(expected, ARRAY_SIZE, sizeof(int), int_compar);

    counting_sort(actual, ARRAY_SIZE, sizeof(int), int_key);

    assert(memcmp(actual, expected, ARRAY_SIZE * sizeof(int)) == 0);

    free(actual);
    free(expected);
}

static size_t
size_t_key(const void *ptr)
{
    return *(const size_t *)ptr;
}

static void
test_size_t_array(void)
{
    size_t actual[] =
    {
        1000000, 1, 500000, 0, 999999
    };

    size_t expected[] =
    {
        0, 1, 500000, 999999, 1000000
    };

    enum { ARRAY_SIZE = 5 };

    counting_sort(actual, ARRAY_SIZE, sizeof(size_t), size_t_key);

    assert(memcmp(actual, expected, sizeof(actual)) == 0);
}

int
main(void)
{
    test_empty_array();
    test_single_element();
    test_sorted_array();
    test_reverse_sorted_array();
    test_duplicate_keys();
    test_zero_keys();
    test_large_key_range();
    test_structs();
    test_stability();
    test_two_elements();
    test_all_equal_elements();
    test_alternating_keys();
    test_sparse_keys();
    test_repeated_min_and_max();
    test_stability_with_ids();
    test_large_objects();
    test_single_distinct_key();
    test_huge_array();
    test_size_t_array();

    puts("All counting_sort tests passed!");
    return 0;
}
