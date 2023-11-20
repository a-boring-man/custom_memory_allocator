#include "malloc.h"

void	*malloc(size_t size) {
	void	*return_ptr;
	//dprintf(1, "my_malloc %ld", size);
	t_zone	*zone = choose_the_right_page(size);
	return_ptr = first_fit(size, zone);
	//void	*free_space = first_fit(zone, size);
	//void	*free_space = next_fit(zone, size);
	//void	*free_space = best_fit(zone, size);
	//if (free_space == NULL) {
	//	return create_page(zone, size);
	//}
	(void)zone;
	(void)size;
	
	// int pff = write(1, "salut\n", 6);
	// (void)pff;
	return (NULL);
}