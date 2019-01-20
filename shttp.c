#include "dbg.h"
#include "shttp.h"

//get http request line
int get_request_line(int sock, char *buf, int line_len) {
	int cnt = 0;

	return cnt;
error: 
	return -1;
}

//NOT CGI version
int handler(int sock) {
	debug("Hello, I am client socket handler.");

	return 0;
error:
	log_err("Client handler error.");
	return -1;
}
