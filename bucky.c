/* Bucky: Gopher <-> HTTP proxy for Bucktooth server */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netdb.h>
#include <resolv.h>

#include "gopher.h"
#include "config.h"

#define BUCKDCOM BUCKD" -isproxy -proxy=2F%s -proxyip=%s"
#define BUCKDCOM_MAXLEN 1024

#define HTMLSTR_MAXLEN 512
#define URLSTR_MAXLEN 512

/* Copy and hex encode a string.
 * This will always require hs to be able to hold a string of at least strlen(str) * 2 chars.
 */
void hexstrcpy(char *hs, char *str)
{
	size_t len, i;

	len = strlen(str);

	for (i = 0; i < len; i++) {
		sprintf(hs + i * 2, "%.2x", str[i]);
	}
}

/* Copy a string, making it HTML-safe */
void htmlstrncpy(char *html, char *str, size_t num)
{
	size_t len = strlen(str);
	size_t i, j;

	for (i = 0, j = 0; i < len && j < num; i++, j++) {
		if (str[i] == '<') {
			strncpy(html + j, "&lt;", 4);
			j += 3;
		} else if (str[i] == '>') {
			strncpy(html + j, "&gt;", 4);
			j += 3;
		} else if (str[i] == ' ') {
			strncpy(html + j, "&nbsp;", 6);
			j += 5;
		} else {
			html[j] = str[i];
		}
	}

	html[j] = '\0';
}

/* Copy a string, making it URL-safe */
void urlstrncpy(char *url, char *str, size_t num)
{
	size_t len = strlen(str);
	size_t i, j;

	for (i = 0, j = 0; i < len && j < num; i++, j++) {
		if (str[i] == '?') {
			strncpy(url + j, "%3F", 3);
			j += 2;
		} else if (str[i] == '\t') {
			strncpy(url + j, "%09", 3);
			j += 2;
		} else {
			url[j] = str[i];
		}
	}

	url[j] = '\0';
}

/* Copy a URL-safe string, unescaping it */
void unurlstrncpy(char *str, char *url, size_t num)
{
	size_t len = strlen(url);
	size_t i, j;

	for (i = 0, j = 0; i < len && j < num; ++i, ++j) {
		if (url[i] == '+') {
			str[j] = ' ';
		} else {
			str[j] = url[i];
		}
	}

	str[j] = '\0';
}
		
/* Read up to next occurence of character */
int read_to(FILE *buckd, char str[], int n, char to)
{
	int i, c;

	for (i = 0; (c = fgetc(buckd)) != to && i < n; i++) {
		str[i] = c;
	}

	str[i] = '\0';

	return i;
}

/* Print a menu item as HTML */
void print_menu_item(char type, char *display, char *selector, char *host, char *port)
{
	char html_display[HTMLSTR_MAXLEN], url_string[URLSTR_MAXLEN];

	/* Escape naughty characters in the display string so it's HTML-friendly */
	htmlstrncpy(html_display, display, HTMLSTR_MAXLEN);

	/* Encode naughty characters in the URL */
	urlstrncpy(url_string, selector, URLSTR_MAXLEN);

	printf("<tr>");

	/* Inline resources (info, error) */
	if (type == GOPHER_ITEM_INFO) {
		printf("<td></td><td align=\"left\" valign=\"middle\">");
		printf("<tt class=\"info\">");
		if (strcmp(html_display, "") == 0) {
			printf("<br>");
		} else {
			printf("<nobr>%s</nobr>", html_display);
		}
		printf("</tt>");
	} else if (type == GOPHER_ITEM_ERROR) {
		printf("<td align=\"right\" valign=\"middle\">");
		printf("<img src=\"%s\">", gopher_item_icon(type));
		printf("</td><td align=\"left\" valign=\"middle\">");
		#ifdef TT_LINKS
			printf("<tt class=\"error\">");
		#else
			printf("<div class=\"error\">");
		#endif
		printf("<nobr>%s</nobr>", html_display);
		#ifdef TT_LINKS
			printf("</tt>");
		#else
			printf("</div>");
		#endif
	/* If the resource is external, provide a gopher:// URL */
	} else if (strcmp(host, MY_HOST) != 0) {
		printf("<td align=\"right\" valigh=\"middle\">");
		if (strcmp(port, DEFAULT_GOPHER_PORT) != 0) {
			printf("<a href=\"gopher://%s:%s/%c%s\">", host, port, type, url_string);
		} else {
			printf("<a href=\"gopher://%s/%c%s\">", host, type, url_string);
		}
		printf("<img src=\"%s\">", gopher_item_icon(type));
		printf("</a></td><td align=\"left\" valign=\"middle\">");
		#ifdef TT_LINKS
			printf("<tt class=\"res ext\">");
		#else
			printf("<div class=\"res ext\">");
		#endif
		if (strcmp(port, DEFAULT_GOPHER_PORT) != 0) {
			printf("<a href=\"gopher://%s:%s/%c%s\">", host, port, type, url_string);
		} else {
			printf("<a href=\"gopher://%s/%c%s\">", host, type, url_string);
		}
		printf("%s", html_display);
		printf("</a>");
		#ifdef TT_LINKS
			printf("</tt>");
		#else
			printf("</div>");
		#endif
	/* Local resources get a relative URI.
	 * If USE_REWRITE is enabled, this is relative to REWRITE_ROOT
	 * otherwise, it is given as a QUERY_STRING to the CGI script
	 */
	} else {
		printf("<td align=\"right\" valign=\"middle\">");
		/* Resources with special URIs */
		if (type == GOPHER_ITEM_TELNET) {
			printf("<a href=\"telnet://%s:%s\">", host, port);
		} else if (type == GOPHER_ITEM_TN3270) {
			printf("<a href=\"tn3270://%s:%s\">", host, port);
		} else if (type == GOPHER_ITEM_HTML && strncmp(url_string, "URL:", 4) == 0) {
			printf("<a href=\"%s\">", url_string + 4);
		} else {
			#ifdef USE_REWRITE
				printf("<a href=\"%s%c%s\">", REWRITE_ROOT, type, url_string);
			#else
				printf("<a href=\"?%c%s\">", type, url_string);
			#endif
		}
		printf("<img src=\"%s\">", gopher_item_icon(type));
		printf("</a></td><td align=\"left\" valign=\"middle\">");
		#ifdef TT_LINKS
			printf("<tt class=\"res\">");
		#else
			printf("<div class=\"res\">");
		#endif
		/* Resources with special URIs */
		if (type == GOPHER_ITEM_TELNET) {
			printf("<a href=\"telnet://%s:%s\">", host, port);
		} else if (type == GOPHER_ITEM_TN3270) {
			printf("<a href=\"tn3270://%s:%s\">", host, port);
		} else if (type == GOPHER_ITEM_HTML && strncmp(url_string, "URL:", 4) == 0) {
			printf("<a href=\"%s\">", url_string + 4);
		} else {
			#ifdef USE_REWRITE
				printf("<a href=\"%s%c%s\">", REWRITE_ROOT, type, url_string);
			#else
				printf("<a href=\"?%c%s\">", type, url_string);
			#endif
		}
		printf("%s", html_display);
		printf("</a>");
		#ifdef TT_LINKS
			printf("</tt>");
		#else
			printf("</div>");
		#endif
	}

	printf("</td></tr>\r\n");
}

/* Read output from buckd and parse a line of a menu.
 * Returns 1 if there are more lines to parse,
 * or 0 if the end of the menu has been reached.
 */
int handle_menu_line(FILE *buckd)
{
	char display[DISPLAY_MAXLEN];
	char selector[SELECTOR_MAXLEN];
	char host[HOST_MAXLEN];
	char port[PORT_MAXLEN];

	int c;

	char type;

	c = fgetc(buckd);
	if (c == '.' || c == EOF) {
		return 0;
	} else {
		type = c;
	}

	read_to(buckd, display, DISPLAY_MAXLEN, '\t');
	read_to(buckd, selector, SELECTOR_MAXLEN, '\t');
	read_to(buckd, host, HOST_MAXLEN, '\t');
	read_to(buckd, port, PORT_MAXLEN, '\r');

	print_menu_item(type, display, selector, host, port);

	while ((c = fgetc(buckd)) != '\n') if (c == EOF) break;

	return 1;
}

/* Optional navigation bar */
void print_top_nav(char type, char *selector)
{
	int len, c;
	int is_root = strcmp(selector, "/") == 0;

	if (is_root) return;

	len = strlen(selector);

	#ifdef TT_LINKS
		printf("<tt class=\"nav\" id=\"topnav\">\r\n");
	#else
		printf("<div class=\"nav\" id=\"topnav\">\r\n");
	#endif

	if (selector[len - 1] == '/') {
		c = len - 2;
	} else {
		c = len - 1;
	}

	while (selector[c] != '/' && c > 0) c--;

	printf("<a id=\"navup\" title=\"go up a directory\" ");
	#ifdef USE_REWRITE
		if (strncmp(selector, "/", c) == 0) {
			printf("href=\"%s\">", REWRITE_ROOT);
		} else {
			printf("href=\"%s1/%.*s\">", REWRITE_ROOT, c, selector);
		}
	#else
		printf("href=\"?1/%.*s\">", c, selector);
	#endif
	printf("<img src=\"%s\"> ", gopher_item_icon(GOPHER_ITEM_DIRECTORY));
	printf("..");
	printf("</a>\r\n");

	printf("<a id=\"navroot\" title=\"go to root of server\" ");
	#ifdef USE_REWRITE
		printf("href=\"%s\">", REWRITE_ROOT);
	#else
		printf("href=\"?\">");
	#endif
	printf("<img src=\"%s\"> ", gopher_item_icon(GOPHER_ITEM_DIRECTORY));
	printf("/");
	printf("</a>\r\n");

	#ifdef TT_LINKS
		printf("</tt>\r\n");
	#else
		printf("</div>\r\n");
	#endif

	printf("<hr>\r\n");
}

void print_bottom_nav(char type, char *selector)
{
	char url[URLSTR_MAXLEN];
	int is_root = strcmp(selector, "/") == 0;

	urlstrncpy(url, selector, URLSTR_MAXLEN);

	printf("<hr>\r\n");

	#ifdef TT_LINKS
		printf("<tt class=\"nav\" id=\"bottomnav\">\r\n");
	#else
		printf("<div class=\"nav\" id=\"bottomnav\">\r\n");
	#endif

	if (is_root) {
		printf("<a href=\"gopher://%s\" title=\"view with Gopher client\">", MY_HOST);
	} else {
		printf("<a href=\"gopher://%s/%c/%s\" title=\"view with Gopher client\">", MY_HOST, type, url);
	}

	printf("<img src=\"%s\"> ", gopher_item_icon(GOPHER_ITEM_DIRECTORY));

	if (is_root) {
		printf("gopher://%s", MY_HOST);
	} else {
		printf("gopher://%s/%c/%s", MY_HOST, type, url);
	}

	printf("</a>\r\n");

	#ifdef TT_LINKS
		printf("</tt>\r\n");
	#else
		printf("</div>\r\n");
	#endif
}

/* Textfiles (plain text, type 0) will have a final full-stop (.) line */
void handle_textfile(FILE* buckd)
{
	int c, i;
	int next[3];

	while ((c = fgetc(buckd)) != EOF) {
		if (c == '.') {
			for (i = 0; i < 3; i++) {
				next[i] = fgetc(buckd);
			}

			if (next[0] == '\r' && next[1] == '\n' && next[2] == EOF) {
				break;
			} else {
				for (i = 2; i >= 0; i--) {
					ungetc(next[i], buckd);
				}
			}
		}

		putchar(c);
	}
}

/* Files should be sent as-is with an appropriate MIME type */
void handle_file(FILE *buckd, char response_type, char *selector)
{
	int c;

	char *ext;

	printf("Content-type: ");

	/* for binary, attempt to guess the MIME type from the extension */
	if (response_type == GOPHER_ITEM_BINARY) {
		ext = strrchr(selector, '.');
		if (!ext) ext = "";
		printf("%s; charset=binary", mime_type_from_ext(ext));
	} else {
		printf("%s; charset=utf-8", mime_type(response_type));
	}

	printf("\r\n\r\n");

	if (response_type == GOPHER_ITEM_PLAIN_TEXT) {
		/* Do not show the final terminating full-stop returned by the server */
		handle_textfile(buckd);
	} else {
		while ((c = fgetc(buckd)) != EOF) {
			putchar(c);
		}
	}
}

/* Handle output from buckd based on the item type and selector requested */
void handle_buckd(FILE *buckd, char response_type, char *selector)
{
	if (response_type == GOPHER_ITEM_DIRECTORY || response_type == GOPHER_ITEM_SEARCH) {
		printf("Content-type: text/html; charset=utf-8\r\n\r\n");
		printf("<html>\r\n");
		printf("<head>\r\n");
		printf("<meta charset=\"utf-8\">\r\n");
		printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\r\n");

		#ifdef EXTERNAL_CSS
			printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"%s\">", EXTERNAL_CSS);
		#endif

		printf("<title></title>\r\n");

		printf("</head>\r\n");
		printf("<body>\r\n");

		#ifdef SHOW_NAVBARS
			print_top_nav(response_type, selector);
		#endif

		if (response_type == GOPHER_ITEM_DIRECTORY || strstr(selector, "\t") || strstr(selector, "?")) {
			printf("<table border=\"0\" cellpadding=\"1\" cellspacing=\"0\">\r\n");
			while (handle_menu_line(buckd));
			printf("</table>\r\n");
		} else {
			printf("<tt class=\"queryTitle\">Enter query:</tt>\r\n");
			#ifdef USE_REWRITE
				printf("<form action=\"/\">\r\n");
			#else
				printf("<form>\r\n");
			#endif
			printf("<input type=\"hidden\" name=\"SEARCH\" value=\"%s\">\r\n", selector);
			printf("<input type=\"text\" name=\"Q\" autofocus>\r\n");
			printf("<input type=\"submit\">\r\n");
			printf("</form>\r\n");
		}

		#ifdef SHOW_NAVBARS
			print_bottom_nav(response_type, selector);
		#endif

		printf("</body>\r\n");
		printf("</html>\r\n");
	} else {
		handle_file(buckd, response_type, selector);
	}
}		

/* Call Bucktooth daemon and return a FILE pointer to the process */
FILE *popen_buckd(char *req)
{
	char *ip = getenv("REMOTE_ADDR");
	char com[BUCKDCOM_MAXLEN];

	snprintf(com, BUCKDCOM_MAXLEN, BUCKDCOM, req, ip);

	return popen(com, "r");
}

FILE *open_socket(char *req)
{
	int sockfd;
	struct sockaddr_in addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	server = gethostbyname(MY_HOST);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(MY_PORT);

	bcopy(server->h_addr, &addr.sin_addr.s_addr, server->h_length);

	connect(sockfd, (struct sockaddr *) &addr, sizeof(addr));

	write(sockfd, req, strlen(req));
	write(sockfd, "\r\n", 2);

	return fdopen(sockfd, "r");
}

int main(void)
{
	char *query_string, *type, *selector, *search_res, *search_q;
	char search_sel[SELECTOR_MAXLEN], search_params[URLSTR_MAXLEN];
	char hs[SELECTOR_MAXLEN * 2];
	FILE *fp;

	/* QUERY_STRING will be in the form of #/SELECTOR */
	query_string = getenv("QUERY_STRING");

	/* ...EXCEPT if its a special SEARCH query (needed to use standard HTML form for queries) */
	if (strncmp(query_string, "SEARCH=", 7) == 0) {
		type = "7";
		search_res = strtok(query_string + 7, "&");
		strtok(NULL, "=");
		search_q = strtok(NULL, "");
		unurlstrncpy(search_params, search_q, URLSTR_MAXLEN);
		snprintf(search_sel, SELECTOR_MAXLEN, "%s\t%s", search_res, search_params);
		selector = search_sel;
	} else {
		type = strtok(query_string, "/");
		selector = strtok(NULL, "");
	}

	/* Default to root menu */
	if (!type) type = "1";
	if (!selector) selector = "/";

	#ifdef USE_SOCKETS
		fp = open_socket(selector);
		handle_buckd(fp, *type, selector);
		fclose(fp);
	#else
		/* buckd's -proxy flag takes a hex-encoded selector */
		hexstrcpy(hs, selector);

		fp = popen_buckd(hs);
		handle_buckd(fp, *type, selector);
		pclose(fp);
	#endif
	
	return 0;
}
