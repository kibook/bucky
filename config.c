#include "config.h"

/* MIME-types associated with certain Gopher item types */
const char *mime_type(char type)
{
	switch (type) {
		case GOPHER_ITEM_PLAIN_TEXT: return "text/plain";
		case GOPHER_ITEM_JPEG: return "image/jpeg";
		case GOPHER_ITEM_PNG: return "image/png";
		case GOPHER_ITEM_BINARY:
		case GOPHER_ITEM_DOS:
		case GOPHER_ITEM_UUE:
		case GOPHER_ITEM_BINHEX: return "application/octet-stream";
		case GOPHER_ITEM_GIF: return "image/gif";
		case GOPHER_ITEM_WAV: return "audio/x-wav";
		case GOPHER_ITEM_HTML: return "text/html";
	}

	return "text/plain";
}

/* Return the HTTP request path to the item type tag icon */
const char *gopher_item_icon(char type)
{
	switch (type) {
		case GOPHER_ITEM_PLAIN_TEXT: return "/gophericon/icn0.png";
		case GOPHER_ITEM_DIRECTORY: return "/gophericon/icn1.png";
		case GOPHER_ITEM_ERROR: return "/gophericon/icn3.png";
		case GOPHER_ITEM_BINHEX: return "/gophericon/icn4.png";
		case GOPHER_ITEM_DOS: return "/gophericon/icn5.png";
		case GOPHER_ITEM_SEARCH: return "/gophericon/icn7.png";
		case GOPHER_ITEM_TELNET: return "/gophericon/icn8.png";
		case GOPHER_ITEM_BINARY: return "/gophericon/icn9.png";
		case GOPHER_ITEM_JPEG: return "/gophericon/icnI.png";
		case GOPHER_ITEM_WAV: return "/gophericon/icns.png";
		case GOPHER_ITEM_PNG: return "/gophericon/icnp.png";
		case GOPHER_ITEM_HTML: return "/gophericon/icnh.png";
	}

	return "/gophericon/icn.png";
}
