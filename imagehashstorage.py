from ctypes import *

import configuration

_image_hash = cdll.LoadLibrary(configuration.IMAGE_HASH_LIBRARY_PATH)
_image_hash.image_hash_storage_create.restype = c_void_p
_image_hash.image_hash_storage_load.restype = c_void_p

class ImageHashStorage:
    def __init__(self):
        self.image_hash = _image_hash # prevent `_image_hash` from being freed by garbage collector
        self._storage = None # will be instantiated lazily

    def __del__(self):
        if self.storage:
            self.image_hash.image_hash_storage_free(self.storage)

    @property
    def storage(self):
        if not self._storage:
            self._storage = c_void_p(self.image_hash.image_hash_storage_create())
            if not self._storage:
                raise Exception('failed to create image hash storage', self.image_hash.image_hash_storage_get_last_error())

        return self._storage

    @storage.setter
    def storage(self, value):
        if self._storage:
            self.image_hash.image_hash_storage_free(self._storage)
            self._storage = None

        self._storage = c_void_p(value)

    def store(self, filename):
        if not self.image_hash.image_hash_storage_store(self.storage, filename.encode()):
            raise Exception('failed to store image hash storage', self.image_hash.image_hash_storage_get_last_error())

    def load(self, filename):
        self.storage = self.image_hash.image_hash_storage_load(filename.encode())
        if not self.storage:
            raise Exception('failed to load image hash storage', self.image_hash.image_hash_storage_get_last_error())

    def add(self, id, hash):
        if not self.image_hash.image_hash_storage_add(self.storage, id.encode(), c_ulonglong(hash)):
            raise Exception('failed to add image hash to storage', self.image_hash.image_hash_storage_get_last_error())

    def query(self, hash, radius=22.0):
        neighbor_id = c_char_p()
        neighbor_hash = c_ulonglong()

        if not self.image_hash.image_hash_storage_query(self.storage, c_ulonglong(hash), c_float(radius), byref(neighbor_id), byref(neighbor_hash)):
            raise Exception('failed to query image hash from storage', self.image_hash.image_hash_storage_get_last_error())

        if not neighbor_hash.value:
            return None

        return {
            'id': neighbor_id.value.decode(),
            'hash': neighbor_hash.value
        }

    def display(self):
        self.image_hash.image_hash_storage_print(self.storage)