#pragma once

# include "../includes/IRC.hpp"
# include "../Channel/Channel.hpp"
#include "../../includes/user/User.hpp"
#include "../../includes/commands/Command.hpp"
#include "../../includes/commands/Parsing.hpp"

class Server
{

private:

    std::string _mdp;
    unsigned int _host;
    int newSocket;                 // pour le nouveau socket client lorsqu'une connexion est acceptée
    struct sockaddr_in serverAddr; // pour stocker les info du serveur
    struct sockaddr_in newAddr;    // et du client
    char buffer[1024];             // pour stocker les données reçues des clients
    socklen_t addrSize;
    std::deque<struct pollfd> _pfds;
    int serverSocket;
    Parsing command;
    User _tabUser[MAX_USERS];


public:
    Server(void);
    Server(Server const &src);
    Server(std::string host, std::string mdp);
    ~Server();
    Server operator=(Server const &assigment);

    void Start_Server(void);
    void Run_Server(void);
    void connect_client(void);
    int password(int i, std::string newBuffer, User *_tabUser);
    void fillUserCtrlD(User *_tabUser, int i, std::string newBuffer);

    /*Test close Socket*/
    static std::vector<int> openSockets;//partage entre toute les instances de la classe == tous acces a la meme liste de sockets ouvert
    void closeAllSockets();// que avec le static(Acces direct sans instance specifique car dans le main et coherance avec l'autre variable static)
    void serverPartCommand(User *_tabUser, int i, std::deque<struct pollfd> _pfds, Parsing command, Channel &channel);
    void serverPartPassword(User *_tabUser, const int i, std::deque<struct pollfd> _pfds);
};