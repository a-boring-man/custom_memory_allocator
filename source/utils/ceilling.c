
#include "malloc.h"

size_t ceilling_unsigned(size_t x, size_t rounding) {

	size_t	division = x / rounding;
	size_t	modulo = x % rounding;
	size_t	result;

	result = division;
	if (modulo)
		result++;

	return (result);
}

size_t	padded(size_t size) {
	return (ceilling_unsigned(size, sizeof(size_t)) * sizeof(size_t));
}