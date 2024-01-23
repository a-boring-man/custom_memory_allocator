# custom_memory_allocator

## Goals
This project is about implementing a memory management system in C that only uses the function ```nmap()``` to allocate memory. The goal is to find a good heuristic between too many system calls and efficient memory usage or fewer system calls but more wasted memory. This library reimplements the ```malloc```, ```calloc```, ```realloc```, ```free``` functions plus some useful functions like : ```show_alloc_mem``` that shows free and allocated parts of memory and ```show_alloc_mem_ex``` that does the same but prints out the allocated memory in hexadecimal. To use the last two functions, you should include this library in your project and compile with it. Lastly, I made a ```free_everything``` function that does what the name implies.

## how to compile and use the library
To create the library, use the ```make``` command, no warning should be displayed and two .so file should be created. Once created, you have two choices for how to use the library: either recompile it with your C file to be able to use ```show_alloc_mem``` and ```show_alloc_mem_ex``` or you can use the ```LD_PRELOAD``` variable to tell your computer to use this library instead of the standard one.

### simple linkage
Simply launch your binary preceding with the following command : ```LD_PRELOAD=absolute/path/to/the/library/libft_malloc.so``` use ```MYLD_PRELOAD``` if you are on Mac instead. Example : ```LD_PRELOAD=some/path/libft_malloc.so ./my_binary```.

### recompiling
To be able to recompile a binary using this library, you should first export the following environmental variable : ```export LD_LIBRARY_PATH=relative/path/to/the/library```, then use it like you would use any other library by adding it to your .c file and compile it like this : ```gcc mycfile.c libft_malloc.so```

### shortcoming
You could do a ```source LD_PRELOAD=path/to/lib/libft_malloc.so``` to always use the library for any command that you will do in this shell in the future, but since, for some unfounded reason, using this library cause some segmentation fault when using clang compilator and probably other stuff I would advise against it

## bonus features
Bonus features are subtle changes to how the memory allocator works and are activated during the compilation of the library. To enable said features export the coresponding variable name and set it to anything you want except for the FREE_DELAY variable that should be set to an integer value, here is an example : ```export DEBUG=anythingcangohere```. upon activating a bonus features you need to recompile the project by using the command ```make re```, warning should pop up informing you of the features you have activated.
here is a list of features :
    ```DEBUG``` : allocate extra memory before and after the requested space and set it to the hexadecimal value of "ee" for debug purpose. Use it with ```show_alloc_mem_ex``` to quickly find a "write one extra" type of error.

    ```LOG``` : will creat a ```log``` file registering every call to ```free```, ```malloc```, ```realloc```, or ```calloc``` function and the associated pointer adress, it can be usefull for debuging or following the flow of your program, keep in mind that the file never empty itself.

    ```MUTEX``` : will allow the allocation to use CPU multithreading otherwise a multithreading use of this library is not safe.

    ```POISON_FREE``` : set free space to the hex value of "cc" for debuging purpose.

    ```CHECK_FREE``` : setting this feature will in fact unset the feature in the code making ```free``` and ```realloc``` way faster but removing segfault protection in case of a invalide pointer given.

    ```COALESCING``` : defragment free block, result in a better memory usage but slitly slower ```free``` speed.

    ```BEST_FIT``` : change the algorithm to make better use of available memory but at the cost of slower allocation.

    ```PRINTF``` : for debuging purpose only, print certain message on fd 2.