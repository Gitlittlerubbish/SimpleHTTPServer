#include "shttp.h"
#include "dbg.h"

//init server: init tcp connection
int initServer(const char *ip, const int port)
{
	int tcheck = 0;

	//创建套接字
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	check(sock != -1, "Socket create error.");

	//套接字绑定ip、port等
	struct sockaddr_in serv_sockaddr;
	memset(&serv_sockaddr, 0, sizeof(struct sockaddr_in));
	serv_sockaddr.sin_family = AF_INET;
	serv_sockaddr.sin_addr.s_addr = inet_addr(ip);
	serv_sockaddr.sin_port = htons(port);
	
	//bind sock with server_sockaddr
	tcheck = bind(sock, (struct sockaddr*)(&serv_sockaddr), sizeof(serv_sockaddr));
	check(tcheck == 0, "Socket bind error.");

	//now listen port
	tcheck = listen(sock, 5);
	check(tcheck == 0, "Socket listen error.");


	return sock;
error:
	log_err("Failed to init server.....");
	return -1;
}

// sub thread: handle http method
void *pthread_start(void *clnt_sock)
{
	int sock = (int)clnt_sock;

	pthread_detach(pthread_self());
	return (void *)handler(sock);
}

int main(int argc, char *argv[])
{
	int serv_sock = 0;
	//check initial input.
	check(argc == 3, "USAGE: ./shttp [ip] [port]");	
	
	//init server with args
	serv_sock = initServer(argv[1], atoi(argv[2]));
	check(serv_sock != -1, "Go to check things whit init.");

	while (1) {
		//setup client sockaddr_in structure
		struct sockaddr_in clnt_sockaddr;
		memset(&clnt_sockaddr, 0, sizeof(struct sockaddr_in));
		socklen_t clnt_sockaddr_size = sizeof(clnt_sockaddr);
		int clnt_sock = accept(serv_sock, (struct sockaddr *)(&clnt_sockaddr), &clnt_sockaddr_size);
		// check(clnt_sock != -1, "Client socket accept error.");
		if (clnt_sock == -1) {
			log_warn("Client accept error");
			continue;
		}

		pthread_t tid = 0;
		pthread_create(&tid, NULL, pthread_start, (void *)clnt_sock);
			
	}

	//socket not close
	return 0;
error:
	return 1;
}
