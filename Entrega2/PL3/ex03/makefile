ex01_writer: writer.o
	gcc -Wall -g writer.o -o ex03_writer -lrt

ex01_reader: reader.o
	gcc -Wall -g reader.o -o ex03_reader -lrt

writer.o: writer.c
	gcc -Wall -g -c writer.c -o writer.o -lrt

reader.o: reader.c
	gcc -Wall -g -c reader.c -o reader.o -lrt

run1: ex03_writer
	./ex03_writer

run2: ex03_reader
	./ex03_reader

clean:
	rm *.o ex03_writer ex03_reader
