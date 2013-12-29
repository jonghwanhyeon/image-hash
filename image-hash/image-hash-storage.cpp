#include "image-hash-storage.h"

float image_hash_storage_comparator(MVPDP *point1, MVPDP *point2);

MVPDP *image_hash_storage_create_point(const char *id, unsigned long long hash);
void image_hash_storage_free_point(MVPDP *point);

MVPError error;

float image_hash_storage_comparator(MVPDP *point1, MVPDP *point2) {
	unsigned long long *hash1 = (unsigned long long *)point1->data;
	unsigned long long *hash2 = (unsigned long long *)point2->data;

	return image_hash_compare(*hash1, *hash2);
}

MVPTree *image_hash_storage_create() {
	return mvptree_alloc(NULL, image_hash_storage_comparator, MVPTREE_BRANCH_FACTOR, MVPTREE_PATH_LENGTH, MVPTREE_LEAF_CAPACITY);
}

void image_hash_storage_free(MVPTree *tree) {
	mvptree_clear(tree, free);
}

int image_hash_storage_store(MVPTree *tree, const char *filename) {
	error = mvptree_write(tree, filename, 0644);
	if (error != MVP_SUCCESS) return 0;

	return 1;
}

MVPTree *image_hash_storage_load(const char *filename) {
	return mvptree_read(filename, image_hash_storage_comparator, MVPTREE_BRANCH_FACTOR, MVPTREE_PATH_LENGTH, MVPTREE_LEAF_CAPACITY, &error);
}

int image_hash_storage_add(MVPTree *tree, const char *id, unsigned long long hash) {
	MVPDP *point = image_hash_storage_create_point(id, hash);

	error = mvptree_add(tree, &point, 1);
	if (error != MVP_SUCCESS) {
		image_hash_storage_free_point(point);
		return 0;		
	}

	return 1;
}

int image_hash_storage_query(MVPTree *tree, unsigned long long hash, char **neighbor_id, unsigned long long *neighbor_hash) {
	*neighbor_id = NULL;
	*neighbor_hash = 0;

	MVPDP *point = image_hash_storage_create_point("query-point", hash);

	unsigned int number_of_points;
	MVPDP **points = mvptree_retrieve(tree, point, 1, MVPTREE_THRESHOLD, &number_of_points, &error);
	if ((error != MVP_SUCCESS) && (error != MVP_EMPTYTREE) && (error != MVP_KNEARESTCAP)) {
		image_hash_storage_free_point(point);
		return 0;
	}

	if (number_of_points >= 1) {
		*neighbor_id = points[0]->id;
		memcpy(neighbor_hash, points[0]->data, sizeof(unsigned long long));
	}
	
	image_hash_storage_free_point(point);
	free(points);	

	return 1;
}

MVPDP *image_hash_storage_create_point(const char *id, unsigned long long hash) {
	MVPDP *point = dp_alloc(MVP_UINT64ARRAY);
	point->id = strdup(id);
	point->data = malloc(sizeof(unsigned long long));
	memcpy(point->data, &hash, sizeof(unsigned long long));
	point->datalen = 1;

	return point;
}

void image_hash_storage_free_point(MVPDP *point) {
	dp_free(point, free);
}

int image_hash_storage_get_last_error() {
	return error;
}

void image_hash_storage_print(MVPTree *tree) {
	mvptree_print(stdout, tree);
}