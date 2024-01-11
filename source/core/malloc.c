#include "malloc.h"

void	*malloc(size_t size) {
	pthread_mutex_lock(&mutex);
	if (size == 0) {
	pthread_mutex_unlock(&mutex);
		return NULL;
	}
	void	*return_ptr;
	ft_printf("my_malloc %u\n red_zone_size = %d\n", size, RED_ZONE_SIZE);
	t_zone	*zone = choose_the_right_page(size);
	//show_alloc_mem();
	return_ptr = first_fit(size, zone);
	//show_alloc_mem();
	
	int fd = open("./log", O_APPEND | O_WRONLY);
	ft_dprintf(fd, "malloc : -%p-\n", return_ptr);
	close(fd);
	pthread_mutex_unlock(&mutex);
	//ft_printf("after mutex\n");
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