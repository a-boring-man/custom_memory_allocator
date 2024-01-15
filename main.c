# include "./include/malloc.h"
# include "./include/ft_printf.h"

int main() {
    size_t *test;

    ft_printf("---------------------- STARTING TEST -------------------\n");
    ft_printf("printing memory prior of doing anything\n");
    show_alloc_mem();
    show_alloc_mem_ex();
    ft_printf("\n");

    ft_printf("calling one malloc to see i we need to populated them\n");
    test = malloc(4 * sizeof(size_t));
    ft_printf("printing memory\n");
    show_alloc_mem();
    show_alloc_mem_ex();
    ft_printf("\n");

    ft_printf("----------------testing free----------------\n");
    free(test);
    ft_printf("printing memory\n");
    show_alloc_mem();
    show_alloc_mem_ex();
    ft_printf("\n");

    ft_printf("----------------testing different size of malloc----------------\n");
    for (int i = 49; i < 100000; i = i * 1.5) {
        char *lol = malloc(i);
        lol[0] = 'a';
        ft_printf("lol i is %d\n", i);
    }
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");

    ft_printf("----------------testing 100 allocation for each----------------\n");
    for (int i = 49; i < 9000; i = i * 1.5) {
        for (int j = 0; j < 100; j++) {
            char *lol = malloc(i);
            lol[0] = 'a';
        }
    }
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("----------------testing free everything----------------\n");
    free_everything();
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");

    ft_printf("----------------testing 100 allocation for each  size and freeing just after----------------\n");
    for (int i = 49; i < 9000; i = i * 1.5) {
        for (int j = 0; j < 100; j++) {
            char *lol = malloc(i);
            lol[0] = 'a';
            free(lol);
        }
    }
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
}