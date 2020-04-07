LIB = ../../lib
INCLUDE = ../../include
MODULES = ../../modules

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -g -I$(INCLUDE)
LDFLAGS = -lm

# Αρχεία .o
OBJS = life_gif.o $(MODULES)/life.o $(LIB)/k08.a $(LIB)/libbmp.a  #<-----------for some reason if life.o is in the end it doesn't work

# Το εκτελέσιμο πρόγραμμα
EXEC = life_gif

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS = ../../tests/life_test/glider.rle 10 -10 -10 10 20 1 1 100 example.gif

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

$(MODULES)/life.o: $(MODULES)/life.c
	$(CC) $(MODULES)/life.c -c -o $(MODULES)/life.o $(CFLAGS)

# Για να φτιάξουμε το k08.a, τρέχουμε το make στο lib directory.
$(LIB)/k08.a:
	$(MAKE) -C $(LIB) k08.a

# το ίδιο για το libbmp.a
$(LIB)/libbmp.a:
	$(MAKE) -C $(LIB) libbmp.a

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)