#include <stdlib.h>
#include <string.h>

#include "slice.h"

const char** str_append(const char** list, const char* string, int count) {
    list = realloc(list, sizeof(const char*) * (count + 1));

    strcpy(list[count], string);

    return list;
}