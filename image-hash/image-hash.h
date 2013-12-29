#ifndef image_hash_image_hash_h
#define image_hash_image_hash_h

#include <pHash.h>

extern "C" {

int image_hash_calculate(const char *filename, unsigned long long *hash);
int image_hash_compare(unsigned long long hash1, unsigned long long hash2);

}

#endif
