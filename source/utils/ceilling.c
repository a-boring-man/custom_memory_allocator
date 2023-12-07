
#include "malloc.h"

size_t ceilling_unsigned(double x) {
	size_t	result = x;

	if (x > result) {
		result++;
	}

	return (result);
}

size_t	padded(size_t size) {
	return (ceilling_unsigned((double)size / (double)sizeof(size_t)) * sizeof(size_t));
}