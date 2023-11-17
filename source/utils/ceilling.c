
#include "malloc.h"

size_t ceilling_unsigned(double x) {
	size_t	result = x;

	if (x > result) {
		result++;
	}

	return (result);
}