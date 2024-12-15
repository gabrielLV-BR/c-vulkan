#if !defined(__strings_h__)
#define __strings_h__

char **str_to_owned(const char * const* string_arr, int count);

char **str_append(const char *to_append, const char * const* string_arr, int count);

void str_free_all(const char * const* string_arr, int count);

#endif // __strings_h__
