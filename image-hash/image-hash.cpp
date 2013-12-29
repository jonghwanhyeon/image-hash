#include "image-hash.h"

int image_hash_calculate(const char *filename, unsigned long long *hash) {
	if (ph_dct_imagehash(filename, *hash) < 0) return 0;

	return 1;
}

int image_hash_compare(unsigned long long hash1, unsigned long long hash2) {
	return ph_hamming_distance(hash1, hash2);
}