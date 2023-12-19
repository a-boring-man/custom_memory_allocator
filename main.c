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
    ft_printf("first_free\n");
    free(test);
    show_alloc_mem();
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    ft_printf("second_free\n");
    free(test1);
    show_alloc_mem();
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
    free(test2);
    show_alloc_mem();
    debug_hexa(test - 5 - RED_ZONE_SIZE, 40);
}