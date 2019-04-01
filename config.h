#ifndef CONFIG_H
#define CONFIG_H

#include <string.h>
#include "gopher.h"

/* CONFIGURATION
 *
 * These defines should be edited to match your configuration.
 *
 * See config.c to change MIME types and icons associated with Gopher resources
 */

/* Hostname of your Gopher server (must match buckd's $MYHOST, does
 * NOT need to match the hostname of the HTTP server!)
 */
#define MY_HOST "khzae.net"

/* Port of your Gopher server (must match buckd's $MYPORT) */
#define MY_PORT 70

/* USE_SOCKETS
 *
 * If defined, connect to the Gopher server over normal sockets. This
 * allows bucky to be used with Gopher servers other than Bucktooth.
 *
 * Also, due to the nature of how a webserver may run CGI scripts, there
 * are known issues with permissions on files between the Gopher server
 * and bucky. For most cases these are not a problem, but in the case
 * that your Gopher server must WRITE to the filesystem things can get
 * sticky, as bucky may not be running as the same user as your Gopher
 * server.
 *
 * Therefore, even when using Bucktooth, enabling sockets may be desirable.
 *
 * If you do not need this functionality, leave this undefined to make
 * use of Bucktooth's -isproxy feature instead. The benefits of NOT
 * using sockets, besides the obvious, include the Gopher server seeing
 * individual IPs of proxy users, otherwise they will all appear as the
 * server the proxy is running on.
 */
/*#define USE_SOCKETS*/

/* Path to Bucktooth daemon */
#define BUCKD "/usr/local/bin/buckd"

/* USE_REWRITE
 *
 * If defined, behave as if this is the root of the http server
 * Otherwise, behave like a normal CGI script
 *
 * With USE_REWRITE, the http server should rewrite requests to this CGI script
 * (e.g. "http://abc.com/0/file.txt" -> "http://abc.com/bucky.cgi?0/file.txt")
 *
 * An example with Apache mod_rewrite:
 *
 * RewriteRule "^/$" "/bucky.cgi"
 * RewriteRule "^/(.)/(.*)" "/bucky.cgi?$1/$2"
 *
 * Here we assume REWRITE_ROOT (see below) is "/"
 * Now a request to / is rewritten to request /bucky.cgi (which will display
 * the root menu of the gopher server), and requests in the form of /#/selector
 * are rewritten to /bucky.cgi?#/selector
 *
 * If you would prefer the 'root' of your gopher proxy to be something else
 * besides / (for example, "http://abc.com/gopher/1/file.txt") then change
 * REWRITE_ROOT below.
 *
 * The example above would define REWRITE_ROOT as:
 *     #define REWRITE_ROOT "/gopher/"
 * and RewriteRules like:
 *     RewriteRule "^/gopher/?$" "/bucky.cgi"
 *     RewriteRule "^/gopher/(.)/(.*)" "/bucky.cgi?$1/$2"
 *
 */
#define USE_REWRITE

/* If USE_REWRITE is defined, this if the fake 'root' */
#define REWRITE_ROOT "/"

/* Reference an external CSS file to apply custom styling */
/*#define EXTERNAL_CSS "/style.css"*/

/* Show navigational bars on the HTML page */
#define SHOW_NAVBARS

/* Display links with same font as inline text */
#define TT_LINKS

/* Show icons representing item types. Otherwise, show only the alt text */
#define GOPHER_ICONS

/* The root of the directory containing the item icons for each Gopher item type */
#define GOPHER_ICON_ROOT "/gophericon"

/* The icon used for URL: items */
#define GOPHER_HTTP_ICON "icnhURL.gif"

/* The alt text used for URL: items */
#define GOPHER_HTTP_ALT "url"

/* Show local pictures inline instead of as links */
/*#define INLINE_PICS*/

/* Width of inline pictures in pixels (comment out to show pictures at their
 * native size)
 */
/*#define INLINE_PIC_WIDTH 250*/

/* Display plain text files (type 0) as HTML pages, like menus. */
#define HTML_TEXT

const char *mime_type(char type);
const char *mime_type_from_ext(char *ext);
const char *gopher_item_icon(char type);
const char *gopher_item_icon_alt(char type);
#ifdef INLINE_PICS
int inline_pic(char type);
#endif
void custom_navbar_top(char type, char *selector);
void custom_navbar_bottom(char type, char *selector);

#endif
