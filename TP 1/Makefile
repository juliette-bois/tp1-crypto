default_target: all

SOURCEDIR=sources
INCLUDESDIR=includes
OBJECTSDIR=objects

CC=gcc
LDFLAGS=-L/usr/local/opt/openssl@1.1/lib
CPPFLAGS=-I/usr/local/opt/openssl@1.1/include -I$(INCLUDESDIR)

OBJS=$(OBJECTSDIR)/config.o $(OBJECTSDIR)/crypto.o

all: $(OBJS)
	$(CC) -o main $(SOURCEDIR)/main.c $(OBJS) $(LDFLAGS) $(CPPFLAGS) -lcrypto -lssl

$(OBJECTSDIR)/%.o: $(SOURCEDIR)/%.c $(INCLUDESDIR)/%.h
	$(CC) -o $@ -c $< -I$(INCLUDESDIR)

clean:
	rm -f $(OBJECTSDIR)/*.o