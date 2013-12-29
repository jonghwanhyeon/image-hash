from ctypes import *

import configuration

_image_hash = cdll.LoadLibrary(configuration.IMAGE_HASH_LIBRARY_PATH)

def calculate(filename):
    hash = c_ulonglong()

    if not _image_hash.image_hash_calculate(filename.encode(), byref(hash)):
        raise Exception('failed to calculate image hash')

    return hash.value;

def compare(hash1, hash2):  
    return _image_hash.image_hash_compare(c_ulonglong(hash1), c_ulonglong(hash2))