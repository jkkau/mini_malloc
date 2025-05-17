#include "malloc.h"
#include <stdio.h>

int main() {
    char *p = (char *)mini_malloc(1000);
    for (int i = 0; i < 1000; i++) {
        p[i] = 'a';
    }
    printf("p: %s\n", p);

    mini_free(p);
}