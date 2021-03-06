#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>

#define UNIX_DOMAIN "/tmp/UNIX.domain"
#define DATELEN 1024

int main(int argc, char *argv[])
{
	int GuiConnect_fd = -1;
	int iRet = -1;
	int iRecvLen = 0;
	int iSendLen = 0;
	char GuiSendBuf[DATELEN] = {0};
	char GuiRecvBuf[DATELEN] = {0};

	static struct sockaddr_un ServAddr;
	//struct sockaddr_in ServAddr;

	//creat unix socket
	GuiConnect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	//GuiConnect_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("== GuiConnect_fd = %d\n", GuiConnect_fd);

	if (GuiConnect_fd < 0)
	{
		perror("cannot create communication socket");
		return 1;
	}

	ServAddr.sun_family = AF_UNIX;
	strncpy(ServAddr.sun_path, UNIX_DOMAIN, sizeof(ServAddr.sun_path) - 1);

	//memset(&ServAddr, 0, sizeof(ServAddr));
	//ServAddr.sin_family = AF_INET;
	//ServAddr.sin_port = htons(5050);
	//ServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//connect server

	iRet = connect(GuiConnect_fd, (struct sockaddr*)&ServAddr, sizeof(ServAddr));
	if(-1 == iRet)
	{
		perror("cannot connect to the server");
		close(GuiConnect_fd);
		return 1;
	}

	//receive and send message
	memset(GuiRecvBuf, 0, DATELEN);
	printf("GUI Receie Msg from TDC\n");
	//iRecvLen = read(GuiConnect_fd, GuiRecvBuf, sizeof(GuiRecvBuf));
	iRecvLen = recv(GuiConnect_fd, GuiRecvBuf, DATELEN, 0);
	printf("receive message from server (%d) :%s\n", iRecvLen, GuiRecvBuf);
	printf("GUI Send msg to TDC server:\n");
	memset(GuiSendBuf, 0, DATELEN);
	strcpy(GuiSendBuf, "receive message from GUI client\n");
	GuiSendBuf[strlen(GuiSendBuf)] = '\0';
	//iSendLen = write(GuiConnect_fd, GuiSendBuf, sizeof(GuiSendBuf));
	iSendLen = send(GuiConnect_fd, GuiSendBuf, strlen(GuiSendBuf), 0);
	printf("wrint Date Len to server (%d) : %s\n", iSendLen, GuiSendBuf);

	close(GuiConnect_fd);

	return 0;
}