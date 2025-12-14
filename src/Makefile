
OBJS=scanner.o parser.o ast.o interpreter.o tac.o mips.o symbol_table.o semantics.o codegen.o
INTERM=scanner.c parser.c parser.h
PROGRAM=./interpreter
CFLAGS=-g 

all: $(PROGRAM)

scanner.c: scanner.flex parser.h
	flex -o scanner.c scanner.flex

parser.c parser.h: parser.bison
	bison --defines=parser.h -o parser.c parser.bison

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)
clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM)

test: $(PROGRAM)
	$(PROGRAM) ./examples/complete_program.ada
	$(PROGRAM) ./examples/simple_program.ada


