
**order**
1. Initaialize WAS -> WASstartup()
2. Create a socket -> socket()
3. Bind the socket -> bind()
4. Listen on the socket -> listen()
5. Accept a connection -> accept(), connect()
6. Send and receive data -> recv(), send(), recvfron(), sendto()
7. Disconnet -> closesocket()


**complie**
gcc -o (name) TCP_server.c
./(name) 5000

gcc -o (name) TCP_client.c
./(name) 127.0.0.1 "아무 내용" 5000

