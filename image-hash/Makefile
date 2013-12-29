all:
	clang -O3 -c image-hash.cpp -o image-hash.o
	clang -O3 -c image-hash-storage.cpp -o image-hash-storage.o
	clang -O3 -dynamiclib image-hash.o image-hash-storage.o -lpHash -lmvptree -o image-hash.dylib

clean:
	rm -rf *.o
	rm -rf *.dylib