#include "./strings.h"

#include <stdlib.h>
#include <string.h>

char **str_to_owned(const char **string_arr, int count) {
    char **owned = calloc(sizeof(char *), count);

    for (int i = 0; i < count; i++) {
        owned[i] = calloc(sizeof (char), strlen(string_arr[i]));

        strcpy(owned[i], string_arr[i]);
    }

    return owned;
}

char **str_append(const char *to_append, const char **string_arr, int count) {
    const char **new_string_arr = realloc(string_arr, count + 1);

    new_string_arr[count - 1] = calloc(sizeof (char), strlen(to_append));

    strcpy(new_string_arr[count - 1], to_append);

    return new_string_arr;
}

void str_free_all(const char **string_arr, int count) {
    if (string_arr == NULL) {
        return;
    }

    for (int i = 0; i < count; i++) {
        free(string_arr[i]);
    }

    free(string_arr);
}