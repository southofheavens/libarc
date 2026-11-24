#include <sorting/counting_sort.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static size_t 
lookup_maximum
(
    void * base,
    size_t nmemb,
    size_t size,
    size_t (*key)(const void *)
)
{
    size_t i, maximum = 0;
    for (i = 0; i < nmemb; ++i) 
    {
        size_t current_key = key((void *)((uint8_t *)base + size * i));
        if (current_key > maximum) {
            maximum = current_key;
        }
    }
    return maximum;
}

static size_t *
make_a_count
(
    size_t maximum,
    void * base,
    size_t nmemb,
    size_t size,
    size_t (*key)(const void *)
)
{
    size_t *count = (size_t *)malloc(sizeof(size_t) * (maximum + 1));
    if (count == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < maximum + 1; ++i) {
        count[i] = 0;
    }

    for (i = 0; i < nmemb; ++i)
    {
        void *curr_elem = (void *)((uint8_t *)base + size * i);
        count[key(curr_elem)]++;
    }

    return count;
}

static void
accumulate_counts
(
    size_t *count,
    size_t maximum
)
{
    size_t i;
    for (i = 1; i < maximum + 1; ++i) {
        count[i] += count[i - 1];
    }
}

void 
counting_sort
(
    void * base, 
    size_t nmemb, 
    size_t size, 
    size_t (*key)(const void *)
)
{
    if (base == NULL || key == NULL) {
        return;
    }

    size_t maximum = lookup_maximum(base, nmemb, size, key);
    size_t *count = make_a_count(maximum, base, nmemb, size, key);
    if (count == NULL) {
        return;
    }
    accumulate_counts(count, maximum);

    void *result = malloc(nmemb * size);
    if (result == NULL) {
        return;
    }

    size_t i; 
    for (i = nmemb; i > 0; --i)
    {
        size_t index = i - 1;
        void *current_elem = (void *)((uint8_t *)base + index * size);
        size_t current_key = key(current_elem);
        size_t current_elem_count = count[current_key]; // <-

        memcpy((void *)((uint8_t *)result + size * (current_elem_count - 1)), current_elem, size);
        (count[current_key])--;
    }

    memcpy(base, result, nmemb * size);
    free(count);
    free(result);
}
