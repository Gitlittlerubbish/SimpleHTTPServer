//#include "MyHttp.h"
#include "dbg.h"

//init server: init tcp connection
int initServer(const char *ip, const int port)
{
	return 0;
error:
	return -1;
}

//sub thread:handle http method
//void *pthread_start(void *sock)
//{
//	return;
//}

int main(int argc, char *argv[])
{
	check(argc == 3, "Usage: ./shttp [ip] [port]");	
	return 0;

error:
	return 1;
}
