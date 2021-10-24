default_target: all

SOURCEDIR=sources
INCLUDESDIR=includes
OBJECTSDIR=objects

CC=gcc
LDFLAGS=-L/usr/local/opt/openssl@1.1/lib
CPPFLAGS=-I/usr/local/opt/openssl@1.1/include -I$(INCLUDESDIR)

OBJS=$(OBJECTSDIR)/config.o $(OBJECTSDIR)/crypto.o

all: $(OBJS)
	$(CC) -o rbt $(SOURCEDIR)/main.c $(OBJS) $(LDFLAGS) $(CPPFLAGS) -lcrypto -lssl -Wreturn-stack-address

$(OBJECTSDIR)/%.o: $(SOURCEDIR)/%.c $(INCLUDESDIR)/%.h
	$(CC) -o $@ -c $< -I$(INCLUDESDIR) -Wreturn-stack-address

clean:
	rm -f $(OBJECTSDIR)/*.o