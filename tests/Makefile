# paths
LIB = ../lib
INCLUDE = ../include
MODULES = ../modules

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = pq_sort_test.o $(MODULES)/pq_sort.o $(LIB)/k08.a

# Το εκτελέσιμο πρόγραμμα
EXEC = pq_sort_test

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS =

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)

# Για να φτιάξουμε το k08.a, τρέχουμε το make στο lib directory.
$(LIB)/k08.a:
	$(MAKE) -C $(LIB) k08.a
