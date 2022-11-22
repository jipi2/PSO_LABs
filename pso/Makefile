prog_shared: prog.c libmymath.so
	gcc prog.c -o prog_shared -lmymath -L .

libmymath.so: add_shared.o sub_shared.o
	gcc -shared add_shared.o sub_shared.o -o libmymath.so

add_shared.o: add.c
	gcc -fPIC -c add.c -o add_shared.o

sub_shared.o: sub.c
	gcc -fPIC -c sub.c -o sub_shared.o