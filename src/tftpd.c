
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int getPort(char *argv[]);
char* getDir(char *argv[]);
struct dirent* readFile(char* dir_path, char* file_name);
//void setUpUdp(port);

int main(int argc, char *argv[])
{
	if (argc > 2)
	{
		int port = getPort(argv);
		char* dir = getDir(argv);
		printf("%s",dir);
		printf(" ""%d\n", port);
		char* file_name = "example_data2";
		readFile(dir, file_name);
	}

	else{
		printf("Must have 2 parameters in main function!");
	}
	//setUpUdp(port);
	return 0;
}

int getPort(char *argv[])
{
	return atoi(argv[1]);
}

char* getDir(char *argv[])
{
	return argv[2];
}

struct dirent* readFile(char* dir_path, char* file_name)
{
	FILE *a;

	DIR *dir = opendir(dir_path);
	if (dir == NULL)
	{
		printf("Dir is NULL  \n");
	}

	struct dirent* file;

	while ((file = readdir(dir)) != NULL)
	{
		printf("Entered while loop");
		if (strcmp(file->d_name, file_name) == 0)
		{
			printf("Found file!");
			return file;
		}
		printf("%s\n", file->d_name);
	}
	return NULL;
}

void setUpUdp(int port)
{
    int sockfd;
    struct sockaddr_in server, client;
    char message[512];

    // Create and bind a UDP socket.
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;

    // Network functions need arguments in network byte order instead
    // of host byte order. The macros htonl, htons convert the values.
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    int bindStatus = bind(sockfd, (struct sockaddr *) &server, (socklen_t) sizeof(server));
	if (bindStatus < 0)
	{
		printf("Error bindStatus = -1");
	}

    for (;;) {
        // Receive up to one byte less than declared, because it will
        // be NUL-terminated later.
        socklen_t len = (socklen_t) sizeof(client);
        ssize_t n = recvfrom(sockfd, message, sizeof(message) - 1,
                             0, (struct sockaddr *) &client, &len);

        message[n] = '\0';
        fprintf(stdout, "Received:\n%s\n", message);
        fflush(stdout);

        // convert message to upper case.
        for (int i = 0; i < n; ++i) {
            message[i] = toupper(message[i]);
        }

        sendto(sockfd, message, (size_t) n, 0,
               (struct sockaddr *) &client, len);
	}
	
}
