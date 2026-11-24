#pragma once

#include <stddef.h>

/**
 * Sort NMEMB elements of BASE, of SIZE bytes each,
 * using KEY to extract a sorting key from each element.
 */
void 
counting_sort
(
    void * base, 
    size_t nmemb, 
    size_t size, 
    size_t (*key)(const void *)
);
