#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5
#define RCVBUFSIZE 32

void DieWithError (char * errorMessage);
void HandleTCPClient(int clntSocket);

/*

*** order ***
1. Create a socket -> socket()
2. Identify the socket address with sockaddr_in -> htonl(), htons()
3. Bind the socket -> bind()
4. Listen on the socket -> listen()
5. Accept a connection -> accept(), connect()
6. Send and receive data -> recv(), send(), recvfron(), sendto()
7. Disconnet -> closesocket()


** detail **

What is socket?
A socket is the mechanism that most popular operating systems provide to give programs access to the network.
It allows messages to be sent and received between applications (unrelated processes) on different networked machines.
The sockets mechanism has been created to be independent of any specific type of network.
IP, however, is by far the most dominant network and the most popular use of sockets.



(1.) Create a socket
A socket, server_fd, is created with the socket system call
protocal of the socket func
------------------------------------------------
int server_fd = socket(domain, type, protocol);
------------------------------------------------
All the parameters as well as the return value are integers

- domain, or address family —
소켓이 생성되어야 하는 통신 도메인.
일부 주소 패밀리는 AF_INET (IP), AF_INET6 (IPv6), AF_UNIX (local channel, similar to pipes),
AF_ISO (ISO protocols)및 AF_NS (Xerox Network Systems protocols)입니다.

- type —
서비스 유형. 
이것은 애플리케이션에 필요한 속성에 따라 선택됩니다:
ex) SOCK_STREAM (virtual circuit service), SOCK_DGRAM (datagram service), SOCK_RAW (direct IP service).
특정 서비스를 사용할 수 있는지 알아보려면 주소 패밀리에 문의하십시오.


- protocol -
소켓 작업을 지원하는 데 사용할 특정 프로토콜을 나타냅니다.
이는 일부 가족이 주어진 유형의 서비스를 지원하기 위해 둘 이상의 프로토콜을 가질 수 있는 경우에 유용합니다.
반환 값은 파일 설명자(작은 정수)입니다. 소켓을 만드는 것은 전화 회사에 전화선을 요청하는 것과 유사합니다.

AF_INETTCP/IP 소켓의 경우 IP 주소 패밀리( ) 및 가상 회로 서비스( ) 를 지정하려고 합니다 SOCK_STREAM.
가상 회선 서비스의 형태는 하나뿐이므로 프로토콜의 변형이 없으므로 마지막 인수인 protocol 은 0입니다.



(2.) Identify the socket address with sockaddr_in
(3.) Bind the socket

소켓 이름 지정 에 대해 이야기할 때 소켓에 전송 주소(IP 네트워킹의 포트 번호)를 할당하는 것에 대해 이야기하고 있습니다.
소켓에서는 이 작업을 주소 바인딩이라고 하며 이를 위해 bind 시스템 호출이 사용됩니다.

전송 주소는 소켓 주소 구조에서 정의됩니다.
소켓은 다양한 유형의 통신 인터페이스와 함께 작동하도록 설계되었기 때문에 인터페이스는 매우 일반적입니다.
예를 들어 포트 번호를 매개변수로 받아들이는 대신 sockaddr사용 중인 주소 패밀리(네트워크 유형)에 따라 실제 형식이 결정되는 구조를 사용합니다
예를 들어 UNIX 도메인 소켓을 사용하는 경우 bind 는 실제로 파일 시스템에 파일을 만듭니다.
시스템 호출에 대한 bind는 다음과 같습니다.

------------------------------------------------------------------------------
int bind(int socket, const struct sockaddr *address, socklen_t address_len);
------------------------------------------------------------------------------

- sin_family -
소켓을 설정할 때 사용한 주소 패밀리입니다. 우리의 경우에는 AF_INET.

- sin_port -
포트 번호(전송 주소).
전송 주소(포트)를 명시적으로 할당하거나 운영 체제에서 할당하도록 허용할 수 있습니다.
클라이언트이고 들어오는 연결을 수신하지 않는 경우 일반적으로 운영 체제가 포트 0을 지정하여 사용 가능한 포트 번호를 선택하도록 합니다.
서버인 경우 일반적으로 특정 번호를 선택합니다. 클라이언트는 연결할 포트 번호를 알아야 합니다.

- in_addr -
이 소켓의 주소입니다.
이것은 컴퓨터의 IP 주소일 뿐입니다.
IP를 사용하면 시스템이 각 네트워크 인터페이스에 대해 하나의 IP 주소를 갖게 됩니다.
예를 들어 시스템에 Wi-Fi와 이더넷 연결이 모두 있는 경우 해당 시스템에는 각 인터페이스에 하나씩 두 개의 주소가 있습니다. 
대부분의 경우 특정 인터페이스를 지정하는 데 신경 쓰지 않고 운영 체제가 원하는 것을 사용하도록 할 수 있습니다.
이를 위한 특수 주소는 0.0.0.0이며 기호 상수로 정의됩니다 INADDR_ANY.

주소 구조는 사용되는 전송 유형에 따라 다를 수 있으므로 세 번째 매개변수는 해당 구조의 길이를 지정합니다.
이것은 단순히 sizeof(struct sockaddr_in)

* htonl converts a long integer (e.g. address) to a network representation
* htons converts a short integer (e.g. port) to a network representation



(4.) Listen on the socket
서버에 들어오는 연결을 기다리는 단계.

클라이언트가 서버에 연결하려면 서버에 연결을 수락할 준비가 된 소켓이 있어야 합니다.
시스템 호출 은 listen들어오는 연결을 수락할 수 있어야 함을 소켓에 알려줍니다.
--------------------------------------
#include <sys/socket.h> 
int listen(int socket, int backlog);
--------------------------------------
두 번째 매개변수 backlog는 연결이 거부되기 전에 대기할 수 있는 보류 중인 연결의 최대 수를 정의합니다.

시스템 호출 은 accept보류 중인 연결 대기열(에서 설정)에서 첫 번째 연결 요청을 잡고 해당 연결에 listen대한 새 소켓을 만듭니다.
청취용으로 설정된 원래 소켓은 데이터 교환이 아닌 연결 수락 에만 사용됩니다.
기본적으로 소켓 작업은 동기식 또는 차단이며 수락은 대기열에 연결이 있을 때까지 차단됩니다.


(5.) Accept a connection

accept은 다음과 같습니다
-----------------------------------------------------------------------------------
#include <sys/socket.h>
int accept(int socket, struct sockaddr *제한 주소, socklen_t *restrict address_len);
------------------------------------------------------------------------------------

첫 번째 매개변수 socket는 와의 연결을 허용하도록 설정된 소켓입니다 listen.
두 번째 매개변수는 연결을 위한 주소 구조체입니다. 이를 통해 원하는 경우 연결 소켓의 주소와 포트 번호를 검사할 수 있습니다.
세 번째 매개변수는 주소 구조체의 길이로 채워집니다.

듣고 수락하는 코드는 다음과 같습니다.
-----------------------------------------------------------------------------------------------
if (listen(server_fd, 3) < 0) 
{ 
    perror("듣기 중"); 
    종료(EXIT_FAILURE); 
}
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
{ 
    perror("수락 중");            
    종료(EXIT_FAILURE);        
}
--------------------------------------------------------------------------------------------------


(6.) Send and receive data

마침내 클라이언트(웹 브라우저에서 서버의 IP 주소를 방문할 때)와 서버 사이에 소켓을 연결했습니다!
의사 소통은 쉬운 부분입니다. read파일 에서 작동 하는 동일한 write시스템 호출은 소켓에서도 작동합니다.
------------------------------------------------------------------
int send(int socket, const void *msg, size_t len, int flags);
int recv(int socket, void *buf, size_t len, int flags);
------------------------------------------------------------------

send - 정보를 받을 소켓 디스크립터 주소
recv - 정보를 보내는 소켓 디스크립터 주소
        정상적으로 데이터를 수신하였으며, 실제로 수신한 데이터의 길이를 return합니다.
        오류가 발생하면 -1

socket  : 통신의 주체가 되는 소켓 디스크립터
msg     : 상대에게 보낼 자료의 포인터
buf     : 받은 메세지를 저장할 버퍼 포인터
len     : 전송되는 메세지의 크기 (byte 단위)
flags   :  플레그(옵션)



(7.) Disconnet
통신이 끝나면 가장 쉬운 방법은 파일에 사용되는 것과 동일한 close 시스템 호출 로 소켓을 닫는 것입니다.close
--------------------
close(new_socket);
--------------------

example
https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

*/

int main(int argc, char * argv[])
{
    int server_fd;
    int clntSock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort;
    unsigned int clntLen;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]);
        exit(1);
    }
    else
    {
        fprintf(stderr, "%s\n", argv[0]);
    }
    

    echoServPort = atoi(argv[1]);

    if ((server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    printf("server_fd = %d\n", server_fd);

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);

    if (bind(server_fd, (struct sockaddr * ) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("bind() failed");

    if (listen(server_fd, MAXPENDING) < 0)
        DieWithError("listen() failed");

    fprintf(stderr, "server ready \n");

    for (;;)
    {
        clntLen = sizeof(echoClntAddr);

        if ((clntSock = accept(server_fd, (struct sockaddr * ) &echoClntAddr, &clntLen)) < 0)
            DieWithError("accept() failed");

        printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

        HandleTCPClient(clntSock);
    }
}

void DieWithError(char * errorMessage)
{
    perror(errorMessage);
    exit(1);
}

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize;

    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    fprintf(stderr, "echoBuffer = %s \n", echoBuffer);
    fprintf(stderr, "recvMsgSize = %d \n", recvMsgSize);

    while (recvMsgSize > 0)
    {
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);
}