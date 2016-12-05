build:
	$(CC) $(CARGS) config.c bucky.c -o bucky.cgi

clean:
	rm -rf bucky.cgi
