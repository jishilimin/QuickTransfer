// #include <stdio.h>  
// #include <stdlib.h>  
// #include <unistd.h>  
// #include <errno.h>  
// #include <sys/types.h>  
// #include <sys/socket.h>  
// #include <netinet/in.h>  
// #include <string.h>  
// #include "../piece/udp_piece.h"

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#include "prefixhead.h"
#include "server.h"

#define EXE_VERSION "1.0.1"
#define EXE_NAME    "Server"

// #define SIGUSR1 10

//CLog log__("log/libyi", "_lby_");

void exit_sigal(int)
{
	return;
}

void reload_sigal(int)
{
}

void prepare_server(int argc, const char* argv)
{
	// if (argc > 1 && strcmp("-D", argv) == 0)
	// {
	// 	puts("run as daemon!");
	// 	if (daemon(1, 0) < 0)
	// 	{
	// 		perror("error daemon...");
	// 		exit(1);
	// 	}
	// }
	// signal(SIGTERM, exit_sigal);
	// signal(SIGUSR1, reload_sigal);
}

int usage(int argc, const char *argv[])
{
    if (2 != argc)
		return 0;
    
	// if (0 == strcmp("-D", argv[1]))
	// {
	// 	prepare_server(argc, argv[1]);
	// 	return 1;
	// }
    
	if (0 == strcmp("-v", argv[1]))
	{
		//printf("%s version: %s (%s, %s)\n", EXE_NAME, EXE_VERSION, __DATE__, __TIME__);
		printf("version: %s (%s, %s)\n", EXE_VERSION, __DATE__, __TIME__);
	}
	else if (0 == strcmp("-h", argv[1]))
	{
		printf("Usage: %s [options]\n", EXE_NAME);
		printf("Options: \n");
		printf(" void\tRun %s\n", EXE_NAME);
		printf(" -D\tRun %s as daemon\n", EXE_NAME);
		printf(" -v\tDisplay %s version information\n", EXE_NAME);
		printf(" -h\tDisplay help information\n");
	}
	else
	{
		if (atoi(argv[1]) > 0)
			return 1;

		printf("Usage: %s 6666\n", EXE_NAME);
		// printf("Usage: %s [-v | -h | -D | 6666]\n", EXE_NAME);
	}
    
    return -1;
}

void show_desc()
{
    LOG_INF("/************************************************************************/");
    LOG_INF("/* @COPYRIGHT NOTICE                                                    */");
    LOG_INF("/* @copyright (c) 2019, jevstein                                        */");
    LOG_INF("/* @All rights reserved.                                                */");
    LOG_INF("/*                                                                      */");
    LOG_INF("/* @name    : %s                                                    */", EXE_NAME);
    LOG_INF("/* @version : %s                                                     */", EXE_VERSION);
    LOG_INF("/*                                                                      */");
    LOG_INF("/* @brief   : This is a server to send any file that the client requests*/");
    LOG_INF("/************************************************************************/");
}

void handle_lastword_segv(int signum)
{
	void *array[100];
	size_t size;
	char **strings;
	size_t i;

	signal(signum, SIG_DFL); /* 还原默认的信号处理handler */

	size = backtrace(array, 100);
	strings = (char **)backtrace_symbols(array, size);

	LOG_ERR("Launcher received SIG: %d Stack trace:\n", signum);
	for (i = 0; i < size; i++)
	{
		LOG_ERR("%lu %s \n", i, strings[i]);
	};

	free(strings);
}

void register_lastword()
{
	signal(SIGSEGV, handle_lastword_segv); // SIGSEGV    11  Core Invalid memory reference
	signal(SIGABRT, handle_lastword_segv); // SIGABRT    6   Core Abort signal from
	signal(SIGINT, handle_lastword_segv);
	signal(SIGTSTP, handle_lastword_segv);
	signal(SIGTERM, handle_lastword_segv);
}

void proc_bar()
{
	char bar[102];
	int i=0;
	const char *p="|/-\\";
	do {
		bar[i]='#';
		bar[i+1]='\0';
		printf("[%-100s]\033[1;32m %d\033[1;36m%%\033[1;31m %c\033[0m \r",bar,i,p[    i%4]);
		usleep(10000);
	} while(++i<=100);
	printf("\n");
}

int main(int argc, const char * argv[]) 
{
	//setlocale(LC_ALL, "chs");//support Chinese
	setlocale(LC_ALL, "C");
    
	int port = SERVER_PORT;

    // show version information
	int ret = usage(argc, argv);
    if (ret < 0)
        return -1;
	if (ret == 1)
		port = atoi(argv[1]);

	srand((unsigned)time(NULL));

	//register_lastword();

	// printf("download 'test_file.avi' ...\n");
	// proc_bar();

    // show the description information for this application
	show_desc();
    
	// server
	jvt_server_t S;
	if (jvt_server_init(&S, port) != 0)
	{
		LOG_ERR("failed to initailize server!");
    	return 0;
	}
	jvt_server_run(&S);
	jvt_server_uninit(&S);
    
    // destroy
	LOG_INF("the end!");

    return 0;
}