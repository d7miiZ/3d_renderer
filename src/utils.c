#include <stdlib.h>
#include <string.h>

#include <utils.h>

// Very naive swap function, fix later
void swap(void *a, void *b, size_t size) {
    void *temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}