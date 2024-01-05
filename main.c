# include "./include/malloc.h"

int main() {
    size_t *test;
    size_t *test1;
    size_t *test2;

    test = malloc(2 * sizeof(size_t));
    //show_alloc_mem();
    //debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    test1 = malloc(2 * sizeof(size_t));
    //show_alloc_mem();
    //debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    test2 = malloc(2 * sizeof(size_t));
    //show_alloc_mem();
    //debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    test[0] = 42;
    test[1] = 67;
    test1[0] = 8942;
    test1[1] = 8942;
    test2[0] = 8942;
    test2[1] = 23;
    show_alloc_mem();
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    free(test1);
    show_alloc_mem();
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    void *new_pointer = realloc(test, 1 * sizeof(size_t));
    ft_printf("new pointer is now : %p\n", new_pointer);
    debug_hexa((size_t *)new_pointer - 7, 40);
    show_alloc_mem();
    ft_printf("first_free\n");
    free(test2);
    //free(test);
    show_alloc_mem();
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    ft_printf("second and third_free\n");
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
}