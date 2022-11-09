CC = gcc
TARGET = parser_201820682.out
OBJS = main.o lex.yy.o data_set.o utils.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -g -o $@ $^ -lfl

.c.o:
	$(CC) -g -c -o $@ $<

lex.yy.c:
	flex lex.l

clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f $(TARGET)