# paths
LIB = ../../lib
INCLUDE = ../../include

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = 

# Αρχεία .o
OBJS = line_read.o

# Το εκτελέσιμο πρόγραμμα
EXEC = line_read

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS = < ../../misc/words/words-100000

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)
