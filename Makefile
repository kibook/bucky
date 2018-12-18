SOURCE=config.c bucky.c
OUTPUT=bucky.cgi
CFLAGS=-Wall -Wextra -pedantic-errors

ifeq ($(DEBUG),1)
	CFLAGS+=-g
else
	CFLAGS+=-O3
endif

all: $(OUTPUT)

$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OUTPUT)
