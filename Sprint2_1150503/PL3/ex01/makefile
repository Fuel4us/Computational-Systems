ex01_writer: writer.o
	gcc -Wall -g writer.o -o ex01_writer -lrt

ex01_reader: reader.o
	gcc -Wall -g reader.o -o ex01_reader -lrt

writer.o: writer.c
	gcc -Wall -g -c writer.c -o writer.o -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c -o reader.o -lrt

run1: ex01_writer
	./ex01_writer

run2: ex01_reader
	./ex01_reader

clean:
	rm *.o ex01_writer ex01_reader
