# Chat App
## Group members


## General information
This is a chat app built in C++. It is using the following concepts:

 - Sockets (with the connection through TCP)
 - Multithreading
 - Mutex

Multiple clients can connect to the server to chat with each other.

## How it works
First the server has to be started. It accepts a port number as an argument.
Then the clients are started. Clients accept two argument: server IP address and port number. The maximum number of clients is 100. 

After compiling both server and cient, run server providing port number (e.g. 4200):
./server 4200  

then run client providing ip your IP address and port number you assigned to the server:
./client __your_IP_address__ 4200

Clients will receive initial message from server upon successfull connection. After this message the client can start chatting with other clients or start recieving messages from them.
