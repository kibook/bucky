SOURCE=bucky.c config.c
OUTPUT=bucky.cgi
CFLAGS=-Wall -Wextra -pedantic-errors

ifeq ($(DEBUG),1)
	CFLAGS+=-g
else
	CFLAGS+=-O3
endif

all: $(OUTPUT)

$(OUTPUT): $(SOURCE) gopher.h config.h
	$(CC) $(CFLAGS) -o $@ $(SOURCE)

clean:
	rm -rf $(OUTPUT)
