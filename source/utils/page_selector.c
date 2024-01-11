#include "malloc.h"

t_zone	*choose_the_right_page(size_t size) {
	for (int grimoire_page = 0; grimoire_page < 10; grimoire_page++) {
		if (size <= grimoire[grimoire_page].max_size)
			return &grimoire[grimoire_page];
	}
	return &grimoire[10];
}