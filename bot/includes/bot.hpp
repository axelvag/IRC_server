#ifndef BOT_CLASS_HPP
#define BOT_CLASS_HPP

#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <cstring>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <poll.h>
#include <cerrno>
#include <ctime>
#include <vector>
#include <map>
#include <deque>
# include <list>

#define IP "127.0.0.1"

class Bot
{
    private:
        std::string _name;
        int    _BotHost;
        std::string _BotPassword;
        std::string         _BotIp;
        int            _BotSocket;
        struct sockaddr_in _BotServerAddr;

    public:
        Bot();
        Bot(Bot const &src);
        Bot operator=(Bot const &rhs);
        ~Bot();

        Bot(const std::string& name, int host, std::string password, const std::string& ip);
        void identificationBotInServer();
        int		Bot_Start();
        int		Bot_Run(); 
        int		Bot_Stop();
};

#endif