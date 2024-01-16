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

    ft_printf("----------------testing different size_t of malloc----------------\n");
    for (int i = 49; i < 100000; i = i * 1.5) {
        char *lol = malloc(i);
        lol[0] = 'a';
        ft_printf("lol i is %d\n", i);
    }
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("----------------testing different size_t of malloc----------------\n");
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

    ft_printf("----------------testing 100 allocation for each  size_t and freeing just after----------------\n");
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
    
    ft_printf("----------------testing freeing NULL pointer----------------\n");
    free(NULL);
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("----------------testing 1000 allocation for each  size----------------\n");
    for (int i = 49; i < 10000; i = i * 1.5) {
        for (int j = 0; j < 1000; j++) {
            char *lol = malloc(i);
            lol[0] = 'a';
            //free(lol);
        }
    }
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("---------------- free everything----------------\n");
    free_everything();
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");

    ft_printf("----------------testing coalesing----------------\n");
    size_t *test1 = malloc(4);
    test1[0] = 42;
    size_t *test2 = malloc(4);
    test2[0] = 42;
    size_t *test3 = malloc(4);
    test3[0] = 42;
    size_t *test4 = malloc(84);
    test4[0] = 42;
    size_t *test5 = malloc(84);
    test5[0] = 42;
    size_t *test6 = malloc(84);
    test6[0] = 42;
    size_t *test7 = malloc(180);
    test7[0] = 42;
    size_t *test8 = malloc(180);
    test8[0] = 42;
    size_t *test9 = malloc(180);
    test9[0] = 42;
    size_t *test10 = malloc(560);
    test10[0] = 42;
    size_t *test11 = malloc(560);
    test11[0] = 42;
    size_t *test12 = malloc(560);
    test12[0] = 42;
    size_t *test13 = malloc(1200);
    test13[0] = 42;
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");

    ft_printf("messing thing up\n");

    free(test1);
    free(test3);

    free(test4);
    free(test5);

    free(test7);
    free(test8);
    free(test9);
    
    free(test12);
    free(test11);

    free(test13);

    ft_printf("seeing the result\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("---------------- free everything----------------\n");
    free_everything();
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("---------------- testing realloc increassing size with nothing else ----------------\n");
    char *rea = realloc(NULL, 25);
    rea[0] = 'a';
    rea[16] = '6';
    show_alloc_mem_ex();
    for (int i = 49; i < 10000; i = i * 1.2) {
        rea = realloc(rea, i);
        ft_printf("i : %d\n", i);
        show_alloc_mem_ex();
    }
    ft_printf("\n");
    
    ft_printf("---------------- testing realloc decreasing size with nothing else ----------------\n");
    for (int i = 10000; i > 25; i = i / 1.2) {
        rea = realloc(rea, i);
        ft_printf("i : %d\n", i);
        show_alloc_mem_ex();
    }
    ft_printf("\n");
    
    ft_printf("---------------- testing realloc staying in the same size range ----------------\n");
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 25, padded(25) - 25, padded(25) + 16);
    realloc(rea, 25);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 20, padded(20) - 20, padded(20) + 16);
    realloc(rea, 20);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 15, padded(15) - 15, padded(15) + 16);
    realloc(rea, 15);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 10, padded(10) - 10, padded(10) + 16);
    realloc(rea, 10);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 30, padded(30) - 30, padded(30) + 16);
    realloc(rea, 30);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 35, padded(35) - 35, padded(35) + 16);
    realloc(rea, 35);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 40, padded(40) - 40, padded(40) + 16);
    realloc(rea, 40);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 45, padded(45) - 45, padded(45) + 16);
    realloc(rea, 45);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 50, padded(50) - 50, padded(50) + 16);
    realloc(rea, 50);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 55, padded(55) - 55, padded(55) + 16);
    realloc(rea, 55);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 60, padded(60) - 60, padded(60) + 16);
    realloc(rea, 60);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 5, padded(5) - 5, padded(5) + 16);
    realloc(rea, 5);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 42, padded(42) - 42, padded(42) + 16);
    realloc(rea, 42);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 2, padded(2) - 2, padded(2) + 16);
    realloc(rea, 2);
        show_alloc_mem_ex();
    ft_printf("\n");
    
    ft_printf("---------------- testing realloc staying in the same size range but with allocated block behind----------------\n");
    free(rea);
    rea = realloc(NULL, 32);
    rea[0] = 'a';
    rea[16] = '6';
    realloc(NULL, 64);
    show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 25, padded(25) - 25, padded(25) + 16);
    rea = realloc(rea, 25);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 20, padded(20) - 20, padded(20) + 16);
    rea = realloc(rea, 20);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 15, padded(15) - 15, padded(15) + 16);
    rea = realloc(rea, 15);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 10, padded(10) - 10, padded(10) + 16);
    rea = realloc(rea, 10);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 30, padded(30) - 30, padded(30) + 16);
    rea = realloc(rea, 30);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 35, padded(35) - 35, padded(35) + 16);
    rea = realloc(rea, 35);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 40, padded(40) - 40, padded(40) + 16);
    rea = realloc(rea, 40);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 45, padded(45) - 45, padded(45) + 16);
    rea = realloc(rea, 45);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 50, padded(50) - 50, padded(50) + 16);
    rea = realloc(rea, 50);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 55, padded(55) - 55, padded(55) + 16);
    rea = realloc(rea, 55);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 60, padded(60) - 60, padded(60) + 16);
    rea = realloc(rea, 60);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 5, padded(5) - 5, padded(5) + 16);
    rea = realloc(rea, 5);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 42, padded(42) - 42, padded(42) + 16);
    rea = realloc(rea, 42);
        show_alloc_mem_ex();
        ft_printf("reallocing for %d with padding %d so a total size of %d\n", 2, padded(2) - 2, padded(2) + 16);
    rea = realloc(rea, 2);
        show_alloc_mem_ex();
    ft_printf("\n");
    
    ft_printf("---------------- free everything----------------\n");
    free_everything();
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
    
    ft_printf("---------------- testing calloc ----------------\n");
    show_alloc_mem_ex();
    for (int i = 49; i < 10000; i = i * 1.5) {
        calloc(1, i);
        ft_printf("i : %d\n", i);
        show_alloc_mem_ex();
    }
    ft_printf("\n");
    
    ft_printf("---------------- free everything----------------\n");
    free_everything();
    ft_printf("printing memory\n");
    show_alloc_mem();
    ft_printf("\n");
}