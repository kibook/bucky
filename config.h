#ifndef CONFIG_H
#define CONFIG_H

#include "gopher.h"

/* CONFIGURATION
 *
 * These defines should be edited to match your configuration.
 *
 * See config.c to change MIME types and icons associated with Gopher resources
 */

/* Hostname of your Gopher server (must match buckd's $MYHOST, does
 * NOT need to match the hostname of the HTTP server!) */
#define MY_HOST "khzai.net"
#define MY_PORT 70

#define USE_SOCKETS

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
//#define EXTERNAL_CSS "/style.css"

/* Show navigational bars on the HTML page */
#define SHOW_NAVBARS

const char *mime_type(char type);
const char *gopher_item_icon(char type);

#endif
