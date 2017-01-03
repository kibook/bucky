#ifndef GOPHER_H
#define GOPHER_H

/* Gopher item types */
#define GOPHER_ITEM_PLAIN_TEXT '0'
#define GOPHER_ITEM_DIRECTORY '1'
#define GOPHER_ITEM_CSO '2'
#define GOPHER_ITEM_ERROR '3'
#define GOPHER_ITEM_BINHEX '4'
#define GOPHER_ITEM_DOS '5'
#define GOPHER_ITEM_UUE '6'
#define GOPHER_ITEM_SEARCH '7'
#define GOPHER_ITEM_TELNET '8'
#define GOPHER_ITEM_BINARY '9'
#define GOPHER_ITEM_REDUNDANT '+'
#define GOPHER_ITEM_TN3270 'T'
#define GOPHER_ITEM_GIF 'g'
#define GOPHER_ITEM_JPEG 'I'
#define GOPHER_ITEM_PNG 'p'
#define GOPHER_ITEM_WAV 's'
#define GOPHER_ITEM_INFO 'i'
#define GOPHER_ITEM_HTML 'h'
#define GOPHER_ITEM_PDF 'd'
/* These are actually from Gopher+ */
#define GOPHER_ITEM_MOVIE ';'
#define GOPHER_ITEM_PICTURE ':'
#define GOPHER_ITEM_SOUND '<'
/* These are from Overbite */
#define GOPHER_ITEM_CSS 'c'
#define GOPHER_ITEM_XML 'x'

#define DEFAULT_GOPHER_PORT "70"

/* Limits */
#define DISPLAY_MAXLEN 128
#define SELECTOR_MAXLEN 256
#define HOST_MAXLEN 128
#define PORT_MAXLEN 6

#endif
