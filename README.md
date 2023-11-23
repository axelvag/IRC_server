# IRC_server

Description

Welcome to the IRC project at School 42. This project involves implementing an IRC (Internet Relay Chat) client and server using the C++ programming language. The goal is to create a networked instant messaging system that enables communication between multiple users through channels.

### Features

IRC Server
Client connection management
Creation and management of channels
Handling common IRC commands (JOIN, PART, PRIVMSG, etc.)
User privilege management
IRC Client
Server connection
Joining and leaving channels
Sending private and public messages
Listing connected users

### Prerequisites

Operating System: Linux (the project was developed and tested on Linux but can be adapted to other operating systems)
Compiler: GCC
Dependencies: No external dependencies
Compilation and Execution

IRC Server
bash
Copy code
$ make server
$ ./server [port]
IRC Client
bash
Copy code
$ make client
$ ./client [server_ip] [port] [nickname]
Usage

Compile the server and client using the above commands.
Launch the server by specifying the port it will listen to for connections.
Launch the client by specifying the server's IP address, port, and a nickname.
Connect to the server using the /connect [server_ip] [port] command.
