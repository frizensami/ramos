#include "prog_mm.h"
#include "../../../mm/mm.h"

#define MALLOC_HELP_TEXT "Usage: malloc <number of bytes to allocate>\n"
#define FREE_HELP_TEXT "Usage: free <address to free (e.g. 0x123)> \n"

int malloc_command_handler(char* args) {
    // printf("Hello from malloc! Args: %s\n", args);
    int num_bytes = -1;

    if (*args == '\0') {
        printf(MALLOC_HELP_TEXT);
        return 0;
    }

    if ((num_bytes = atoi(args)) < INT_MAX && num_bytes > 0) {
        void* ptr = malloc(num_bytes);
        if (ptr == NULL) {
            printf("malloc could not allocate %d bytes!\n", num_bytes);
            return 1;
        } else {
            printf("malloc allocated %d bytes starting from 0x%08X\n", num_bytes, ptr);
            return 0;
        }
    } else {
        printf("Invalid number of bytes requested from malloc\n");
        return 1;
    }
}

int free_command_handler(char* args) {
    printf("Hello from free! Args: %s\n", args);

    uint32_t freeptr = 0;

    if (*args == '\0') {
        printf(FREE_HELP_TEXT);
        return 0;
    }

    if ((freeptr = hex_str_to_int(args)) < INT_MAX) {
        void* resptr = free(freeptr);
        if (resptr == NULL) {
            printf("free could not free object at 0x%08X!\n", freeptr);
            return 1;
        } else {
            printf("free'd object at 0x%08X\n", freeptr);
            return 0;
        }
    } else {
        printf("Invalid ptr passed to free\n");
        return 1;
    }
    return 0;
}

int heapinfo_command_handler(char* args) {
    print_heap_info();
    return 0;
}