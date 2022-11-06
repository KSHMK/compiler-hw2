CC = gcc
TARGET = parser_201820682.out
OBJS = main.o lex.yy.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ -lfl

.c.o:
	$(CC) -c -o $@ $<

lex.yy.o:
	flex lex.l
	$(CC) -c -o lex.yy.o lex.yy.c 

clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f $(TARGET)