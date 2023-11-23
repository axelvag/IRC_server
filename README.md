# IRC Server

## Description

Welcome to the IRC project at School 42. </b>
A project make in collaboration with [@Alessio-scat](https://github.com/Alessio-scat) and [@Lucas-Ve](https://github.com/Lucas-Ve)
This project involves implementing an IRC (Internet Relay Chat) client and server using the C++ programming language. The goal is to create a networked instant messaging system that enables communication between multiple users through channels.

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
git clone https://github.com/axelvag/IRC_server.git IRC_server
cd IRC_server/
```

## Compilation and Execution

### IRC Server
```bash
$ make
$ ./ircserver [port between 2000 and 60000] [password]
```
Now the server waits for client connections !

## Connect Client

We use two different logiciel to simulate a client:
- HexChat (It provides a graphical interface that makes it easy to join channels, participate in discussions, and manage various aspects of the IRC experience)
- NetCat/nc (Is a versatile networking utility commonly used for reading from and writing to network connections using TCP in command line, less visual)

### HEXCHAT
> Start logiciel </b>

> Enter a Nickname and Username </b>

> Create a new Network </b>

> Edit him: 127.0.0.1/[same PORT than the ircserver] </b>

> Put the same password than the ircserver </b>

> Then connect </b>

> enter "/join #channel"

### NETCAT
```bash
$ nc localhost [port]
$ PASS [password]
$ NICK [name]
$ USER [name]
$ JOIN #channel
```

TADAMMMMM !!! You are in your channel, you can talk now.

To see our subject clic [here](https://github.com/fpetras/42-subjects/blob/master/ft_irc.en.pdf)
