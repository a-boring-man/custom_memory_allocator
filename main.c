# include "./include/malloc.h"

int main() {
    int *test;

    test = malloc(6 * sizeof(int));
    test[0] = 42;
    test[0] = 67;
    test[0] = 8942;
    test[0] = 8942;
    test[0] = 8942;
    test[0] = 23;
    show_alloc_mem();
}