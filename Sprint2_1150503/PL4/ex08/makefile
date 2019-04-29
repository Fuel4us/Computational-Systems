# Exemplo dado no moodle

# variaveis
INCLUDES =
SOURCES = main.c
OBJFILES = main.o
EXEC = exec

# regras de sufixo
.SUFFIXES : .c .o

# como transformar um .c num .o ; $< -- nome do ficheiro

${EXEC}: ${OBJFILES}
	gcc -Wall -g ${OBJFILES} -o ${EXEC} -lrt -lpthread

${OBJFILES}: ${SOURCES}
	gcc -Wall -g -c ${SOURCES} -o ${OBJFILES} -lrt -lpthread

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
