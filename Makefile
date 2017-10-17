build:
	$(CC) $(CARGS) -O3 config.c bucky.c -o bucky.cgi

clean:
	rm -rf bucky.cgi
