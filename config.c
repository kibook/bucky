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

/* Try to guess MIME type from extension, otherwise fall back to application/octet-stream */
const char *mime_type_from_ext(char *ext)
{
	if (strcmp(ext, ".webm") == 0)
		return "video/webm";
	if (strcmp(ext, ".mp4") == 0)
		return "video/mp4";

	return "application/octet-stream";
}

/* Return the HTTP request path to the item type tag icon */
const char *gopher_item_icon(char type)
{
	switch (type) {
		case GOPHER_ITEM_PLAIN_TEXT: 	return GOPHER_ICON_ROOT"/icn0.gif";
		case GOPHER_ITEM_DIRECTORY: 	return GOPHER_ICON_ROOT"/icn1.gif";
		case GOPHER_ITEM_CSO:		return GOPHER_ICON_ROOT"/icn2.gif";
		case GOPHER_ITEM_ERROR: 	return GOPHER_ICON_ROOT"/icn3.gif";
		case GOPHER_ITEM_BINHEX: 	return GOPHER_ICON_ROOT"/icn4.gif";
		case GOPHER_ITEM_DOS: 		return GOPHER_ICON_ROOT"/icn5.gif";
		case GOPHER_ITEM_UUE:		return GOPHER_ICON_ROOT"/icn6.gif";
		case GOPHER_ITEM_SEARCH: 	return GOPHER_ICON_ROOT"/icn7.gif";
		case GOPHER_ITEM_TELNET: 	return GOPHER_ICON_ROOT"/icn8.gif";
		case GOPHER_ITEM_BINARY: 	return GOPHER_ICON_ROOT"/icn9.gif";
		case GOPHER_ITEM_TN3270:	return GOPHER_ICON_ROOT"/icnT.gif";
		case GOPHER_ITEM_GIF:		return GOPHER_ICON_ROOT"/icng.gif";
		case GOPHER_ITEM_JPEG: 		return GOPHER_ICON_ROOT"/icnI.gif";
		case GOPHER_ITEM_WAV: 		return GOPHER_ICON_ROOT"/icns.gif";
		case GOPHER_ITEM_PNG: 		return GOPHER_ICON_ROOT"/icnp.gif";
		case GOPHER_ITEM_HTML: 		return GOPHER_ICON_ROOT"/icnh.gif";
	}

	return "/gophericon/icn.png";
}
