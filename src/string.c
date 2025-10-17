#include "string.h"

size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return *(const unsigned char*)a - *(const unsigned char*)b;
}

int strncmp(const char *a, const char *b, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (a[i] != b[i] || !a[i] || !b[i])
            return (unsigned char)a[i] - (unsigned char)b[i];
    }
    return 0;
}

void *memset(void *dst, int val, size_t n) {
    unsigned char *p = dst;
    while (n--) *p++ = (unsigned char)val;
    return dst;
}
