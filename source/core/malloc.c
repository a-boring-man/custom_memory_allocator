#include "malloc.h"

void	*malloc(size_t size) {
	void	*return_ptr;
	ft_printf("my_malloc %u\n red_zone_size = %d\n", size, RED_ZONE_SIZE);
	t_zone	*zone = choose_the_right_page(size);
	return_ptr = first_fit(size, zone);
	return (return_ptr);
	//void	*free_space = first_fit(zone, size);
	//void	*free_space = next_fit(zone, size);
	//void	*free_space = best_fit(zone, size);
	//if (free_space == NULL) {
	//	return create_page(zone, size);
	//}
	(void)zone;
	(void)size;
	
	return (NULL);
}