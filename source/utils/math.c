#include "malloc.h"

size_t	_ceilling_unsigned(size_t x, size_t rounding) {

	size_t	division = x / rounding;
	size_t	modulo = x % rounding;
	size_t	result;

	result = division;
	if (modulo)
		result++;

	return (result);
}

size_t	_padded(size_t size) {
	return (_ceilling_unsigned(size, sizeof(size_t)) * sizeof(size_t));
}

size_t	min(size_t a, size_t b) {
	if (a <= b) {
		return a;
	}
	return b;
}
