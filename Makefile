OUTPUT=bucky.cgi
CFLAGS=-Wall -Wextra -pedantic-errors

ifeq ($(DEBUG),1)
	CFLAGS+=-g
else
	CFLAGS+=-O3
endif

all: $(OUTPUT)

$(OUTPUT): bucky.c config.c gopher.h config.h
	$(CC) $(CFLAGS) -o $@ bucky.c config.c

clean:
	rm -rf $(OUTPUT)
