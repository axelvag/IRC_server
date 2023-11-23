# IRC Server

## Description

Welcome to the IRC project at School 42. This project involves implementing an IRC (Internet Relay Chat) client and server using the C++ programming language. The goal is to create a networked instant messaging system that enables communication between multiple users through channels.

## Features

### IRC Server
- Client connection management
- Creation and management of channels
- Handling common IRC commands (JOIN, PART, PRIVMSG, etc.)
- User privilege management
- Put mode in a channel (MODE +i +o +l +k +t)

### IRC Client
- Server connection
- Joining and leaving channels
- Sending private and public messages
- Listing connected users

## Prerequisites

- **Operating System:** Linux (the project was developed and tested on Linux but can be adapted to other operating systems)
- **Compiler:** GCC
- **Dependencies:** No external dependencies
- **Install HexChat for a simple and graphic IRC Client**

## Installation

```bash
git clone [URL REPO] IRC
cd IRC/
```

## Compilation and Execution

### IRC Server
```bash
$ make
$ ./ircserver [port between 2000 and 60000] [password]
```
Now the server waits for client connections !
