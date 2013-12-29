#ifndef image_hash_image_hash_storage_h
#define image_hash_image_hash_storage_h

#include <stdio.h> /* dependency of mvptree.h */
#include <stdlib.h>
#include <string.h>

extern "C" {
#include <mvptree.h>
}

#include "image-hash.h"

#define MVPTREE_BRANCH_FACTOR 2
#define MVPTREE_PATH_LENGTH 5
#define MVPTREE_LEAF_CAPACITY 25

#define MVPTREE_THRESHOLD 24.0

extern "C" {
	
MVPTree *image_hash_storage_create();
void image_hash_storage_free(MVPTree *tree);

int image_hash_storage_store(MVPTree *tree, const char *filename);
MVPTree *image_hash_storage_load(const char *filename);

int image_hash_storage_add(MVPTree *tree, const char *id, unsigned long long hash);
int image_hash_storage_query(MVPTree *tree, unsigned long long hash, char **neighbor_id, unsigned long long *neighbor_hash);

int image_hash_storage_get_last_error();

void image_hash_storage_print(MVPTree *tree);

}

#endif
