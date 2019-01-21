#include "dbg.h"
#include "shttp.h"

int echo_index(int sock, char *path) {
	// int fd = open(path, O_RDONLY);
	// check(fd > 0, "Local path file open error.");
	char buffer[MAX_BUF_SIZE];
	debug("path:%s", path);

	const char *reply_header = "HTTP/1.1 200 OK\r\n";
	FILE *file;
	file = fopen(path, "r");
	fread(buffer, sizeof(buffer), 1, file);
	debug("buffer:%s&&&&&", buffer);
	write(sock, reply_header, strlen(reply_header));
	write(sock, "\r\n", 2);

	buffer[strlen(buffer)] = '\0';
	write(sock, buffer, strlen(buffer));

	return 0;
error:
	return -1;
}

//get http request line
int get_request_line(int sock, char *buf, http_request_line *reqline, int line_len) {
	int i = 0, j = 0;
	debug("I am in get_request_line");

	for(i = 0, j = 0; i < line_len && buf[i] != ' '; i++) {
		reqline->method[j++] = buf[i];
	}
	reqline->method[j] = '\0';

	for(i++, j = 0; i < line_len && buf[i] != ' '; i++) {
		reqline->url[j++] = buf[i];
	}	
	reqline->url[j] = '\0';

	for (i++, j = 0; i < line_len && buf[i] != '\r'; i++) {
		reqline->version[j++] = buf[i];
	}
	reqline->version[j] = '\0';
	// if(buf[i++] != '\n')
	// 	goto error;

	return 0;
error: 
	log_err("Http request line get error.");
	return -1;
}

//NOT CGI version
int handler(int sock) {
	debug("Hello, I am client socket handler.");

	int tcheck = 0;
	char buf[MAX_BUF_SIZE];
	char local_path[MAX_PATH_SIZE * 2] = "wwwroot";
	http_request_line *reqline = calloc(1, sizeof(http_request_line)); // not freed

	ssize_t readRET = read(sock, buf, sizeof(buf));
	check(readRET > 0, "No useful socket request.");
	debug("sock content: %s", buf);

	memset(reqline, 0, sizeof(http_request_line));
	debug("already memset");
	tcheck = get_request_line(sock, buf, reqline, sizeof(buf));
	check(tcheck == 0, "Failed to get request line.");
	debug("request line method:%s, url:%s, version:%s&&&&&", reqline->method, reqline->url, reqline->version);
	
	// if(strcasecmp(reqline->method, "GET")) {
		// debug("I am in if");
		if(reqline->url[strlen(reqline->url) - 1] == '/') {
			debug("I am in if-if");
			strcat(local_path, "/index.html");
		}
		tcheck = echo_index(sock, local_path);
		check(tcheck == 0, "echo index error.");
	// }
	return 0;
error:
	log_err("Client handler error.");
	return -1;
}
