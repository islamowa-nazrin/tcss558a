# Chat App

This is a chat app built in C++. It is using the following concepts:

 - Sockets (with the connection through TCP)
 - Multithreading
 - Mutex

Multiple clients can connect to the server to chat with each other.

## How it works
First the server has to be started. It accepts a port number as an argument.
Then the clients are started. Clients accept two argument: server IP address and port number. The maximum number of clients is 100. 

Clients will receive initial message from server upon successfull connection. After this message the client can start chatting with other clients or start recieving messages from them.
