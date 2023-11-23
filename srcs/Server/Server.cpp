#include "../../includes/Server/Server.hpp"

Server::Server(void)
{
    this->_mdp = "NULL";
    this->_host = 1001;
    this->_pfds.resize(MAX_USERS + 1);
}

Server::Server(std::string host, std::string mdp)
{
    this->_mdp = mdp;
    this->_host = atoi(host.c_str());
    this->_pfds.resize(MAX_USERS + 1);
}

Server::Server(Server const &src)
{
    *this = src;
}

Server::~Server()
{
    closeAllSockets();
}

Server Server::operator=(Server const &assignment)
{
    if (this == &assignment)
        return (*this);
    this->_mdp = assignment._mdp;
    this->_host = assignment._host;
    return(*this);
}

void Server::Start_Server(void)
{
    // Création d'un socket serveur IPv4 et TCP
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverSocket < 0)
    {
        perror("Erreur lors de la création du socket serveur");
        exit(1);
    }

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->_host);
    this->serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    // Pour éviter l'erreur "Address already use"
    int opt = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) < 0)
    {
        perror("Erreur lors de la configuration de SO_REUSEADDR");
        exit(1);
    }

    // Liaison du socket serveur à une adresse IP et un port
    if (bind(this->serverSocket, (struct sockaddr*)&this->serverAddr, sizeof(this->serverAddr)) < 0)
    {
        perror("Erreur lors de la liaison du socket serveur");
        exit(1);
    }

    // Attente de connexions entrantes 10 maximum
    listen(this->serverSocket, 10);
    std::cout << "En attente de connexions..." << std::endl;

    Run_Server();

    close(this->serverSocket);
}

int Server::password(int i, std::string newBuffer, User *_tabUser)
{
    std::string str;
    std::string mdp;
    size_t pos;
    size_t sizeStr;
    std::istringstream iss(newBuffer);
    while (std::getline(iss, str))
    {
        sizeStr = str.size() - 1;
        // pos = str.find_last_of("PASS");
        pos = str.find("PASS");
        if (pos !=  std::string::npos)
        {
            if (sizeStr <= 3)
                return (1);
            mdp = str.substr(pos + 5, sizeStr + 1 - (pos + 5));
            ft_trim(mdp);
            if (mdp != this->_mdp)
            {
                std::cout << "<client> :Password incorrect\n";
                writeInfd(ERR_PASSWDMISMATCH(_tabUser[i].getNickname()), i, _pfds);
                return (1);
            }
        }
    }
    return (0);
}

void Server::fillUserCtrlD(User *_tabUser, int i, std::string newBuffer)
{
    std::string bufferStr(newBuffer);
    std::string nickname, username;

    size_t indexNick = bufferStr.find("NICK");
    size_t indexUser = bufferStr.find("USER");

    int sizeNick = 0;
    int sizeUser = 0;
    if (_tabUser[i].getNickname() != "" && _tabUser[i].getUsername() != "")
        return;
    if (bufferStr.size() == 4)
        return;
    if (indexNick != std::string::npos && indexUser != std::string::npos)
    {
        sizeNick = (indexUser - 2) - (indexNick + 4) - 1;
        sizeUser = bufferStr.find(" 0 *") - (indexUser + 4) - 1;
        nickname = bufferStr.substr(indexNick + 5, sizeNick + 1);
        username = bufferStr.substr(indexUser + 5, sizeUser + 1);
        ft_trim(nickname);
        ft_trim(username);
        if (nickname == "")
            return ;
        _tabUser[i].setNickname(nickname);
        _tabUser[i].setUsername(username);
        return ;
    }
    if (indexNick != std::string::npos) {
        sizeNick = bufferStr.find(" ", indexNick + 5) - (indexNick + 5);
        nickname = bufferStr.substr(indexNick + 5, sizeNick);
        ft_trim(nickname);
        _tabUser[i].setNickname(nickname);
        return;
    }

    if (indexUser != std::string::npos) {
        sizeUser = bufferStr.find(" ", indexUser + 5) - (indexUser + 5);
        username = bufferStr.substr(indexUser + 5, sizeUser);
        ft_trim(username);
        _tabUser[i].setUsername(username);
        return;
    }
}



char* stringToCharArray(const std::string& str) {
    char* result = new char[str.length() + 1]; // +1 for null-terminator
    std::strcpy(result, str.c_str());
    return result;
}

void Server::serverPartPassword(User *_tabUser, const int i, std::deque<struct pollfd> _pfds)
{
    if (static_cast<std::string>(this->buffer).find("\n") != std::string::npos)
    {
        if (_tabUser[i].getBufferSignal().find("PASS") != std::string::npos)
        {
            std::string toAppend = _tabUser[i].getBufferSignal();
            size_t newSize = toAppend.size() + strlen(this->buffer) + 1;
            char* newBuffer = new char[newSize];
            std::strcpy(newBuffer, toAppend.c_str());
            std::strcat(newBuffer, this->buffer);
            _tabUser[i].setBufferSignal("");
            if (password(i, newBuffer, _tabUser))
            {
                close(_pfds[i].fd);
                _pfds[i].fd = 0;
            }
            delete [] newBuffer;
        }
        else if (password(i, this->buffer, _tabUser))
        {
            close(_pfds[i].fd);
            _pfds[i].fd = 0;
        }
    }
    else 
    {
        _tabUser[i].setBufferSignal(this->buffer);
    }
}

void Server::serverPartCommand(User *_tabUser, int i, std::deque<struct pollfd> _pfds, Parsing command, Channel &channel)
{
    if (static_cast<std::string>(this->buffer).find("\n") != std::string::npos)
    {
        if (_tabUser[i].getBufferSignal() != "")
        {
            std::string toAppend = _tabUser[i].getBufferSignal();
            size_t newSize = toAppend.size() + strlen(this->buffer) + 1;
            char* newBuffer = new char[newSize];
            std::strcpy(newBuffer, toAppend.c_str());
            std::strcat(newBuffer, this->buffer);
            _tabUser[i].setBufferSignal("");
            if (_tabUser[i].getNickname() != "" && _tabUser[i].getUsername() != "")
            {
                command.whatCommand(newBuffer, _tabUser, i, _pfds, channel);
                delete [] newBuffer;
            }
            else 
            {
                fillUserCtrlD(_tabUser, i, newBuffer);
                delete [] newBuffer;
            }
        }
        
        _tabUser[i].setBufferSignal("");
        if (_tabUser[i].getNickname() != "" && _tabUser[i].getUsername() != "")
            command.whatCommand(this->buffer, _tabUser, i, _pfds, channel);
    }
    else
    {
        
        _tabUser[i].setBufferSignal(this->buffer);
    }
}

void Server::Run_Server(void)
{
    _pfds[0].fd = this->serverSocket;
    _pfds[0].events = POLLIN;
    Channel channel;
    for (int j = 1; j < MAX_USERS; j++)
        _tabUser[j].setBufferSignal("");
    while (true)
    {
        //en attente d'un event
        if (!_pfds.empty())
        {
            if (poll(&_pfds.front(), _pfds.size(), -1) < 0)
                return;
                // throw std::runtime_error("Error while polling from fd!");
        }

        // event de connexion
        if (_pfds[0].revents & POLLIN)
            connect_client();

        // Gestion des données reçues des clients
        for (int i = 1; i < MAX_USERS; i++)
        {
            if (_pfds[i].revents & POLLIN)
            {
                int bytesRead = recv(_pfds[i].fd, this->buffer, sizeof(this->buffer), 0);
                if (bytesRead <= 0)
                {
                    if (bytesRead == 0)
                    {
                        Quit quit;
                        std::string str = "QUIT :Leaving\r\n";
                        quit.execute_cmd(str, _pfds, _tabUser, i, channel);
                    }
                    close(_pfds[i].fd);
                    _pfds[i].fd = 0;
                }
                else
                {
                    this->buffer[bytesRead] = '\0';
                    // serverPartPassword(_tabUser, i, _pfds);
                    if (password(i, this->buffer, _tabUser))
                    {
                        close(_pfds[i].fd);
                        _pfds[i].fd = 0;
                    }
                    fillUserCtrlD(_tabUser, i, this->buffer);
                    if (_tabUser[i].getNickname() != "")
                    {
                        for(int j = 1; j < MAX_USERS; j++)
                        {
                            if (_tabUser[i].getNickname() == _tabUser[j].getNickname() && j != i)
                            {
                                close(_pfds[i].fd);
                                _pfds[i].fd = 0;
                                _tabUser[i].setNickname(""); 
                            }
                        }
                    }
                    serverPartCommand(_tabUser, i, _pfds, command, channel);
                }
            }
        }
    }

}

void Server::connect_client(void)
{
    // Nouvelle connexion entrante
    addrSize = sizeof(this->newAddr);
    this->newSocket = accept(this->serverSocket, (struct sockaddr*)&this->newAddr, &this->addrSize);
    if (this->newSocket < 0)
        throw std::runtime_error("Error: failed accept()");
    openSockets.push_back(this->newSocket);

    std::cout << "Nouvelle connexion, socket FD : " << this->newSocket << std::endl;

    // Ajout du nouveau socket client à la liste des sockets à surveiller
    for (int i = 1; i < MAX_USERS; i++)
    {
        if (_pfds[i].fd == 0)
        {
            _pfds[i].fd = this->newSocket;
            _pfds[i].events = POLLIN;
             break;
        }
    }
}

std::vector<int> Server::openSockets;

void Server::closeAllSockets() {
    for (std::vector<int>::iterator it = openSockets.begin(); it != openSockets.end(); ++it) {
        close(*it);
    }
    openSockets.clear();
}
