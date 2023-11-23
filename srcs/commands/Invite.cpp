#include "../../includes/commands/Command.hpp"
# include "../../includes/Utils.hpp"

Invite::Invite(void):Command()
{
    this->_isActive = 0;
}

Invite::~Invite(void){}

void Invite::execute_cmd(std::string str)
{
    (void)str;
}

Invite::Invite(Invite const &src):Command(src)
{
    this->_isActive = src._isActive;
    this->_channelInvite = src._channelInvite;
    this->_cmd = src._cmd;
    this->_nickInvite = src._nickInvite;
}

Invite & Invite::operator=(Invite const &rhs)
{
    Command::operator=(rhs);
    this->_isActive = rhs._isActive;
    this->_channelInvite = rhs._channelInvite;
    this->_cmd = rhs._cmd;
    this->_nickInvite = rhs._nickInvite;
    return (*this);
}

void Invite::ParseInviteCmd(std::string &str)
{
    std::istringstream ss(str);
    std::string token;
    int i = 0;

    while (ss >> token)
    {
        if (i == 0)
        {
            _cmd = token;
            i = 1;
        }
        else if (i == 1)
        {
            _nickInvite = token;
            i = 2;
        }
        else if (i == 2)
        {
            if (token == "IRChub")
            {
                _channelInvite = "";
                break;
            }
            _channelInvite = token;
            break;
        }
    }
}

int Invite::ExistChannel(const std::map<std::string, std::list<std::string> >& channel, std::deque<struct pollfd> _pfds, int i, std::string &client)
{
    for (std::map<std::string, std::list<std::string> >::const_iterator it = channel.begin(); it != channel.end(); ++it) {
        if (it->first == this->_channelInvite)
            return 0;
    }
    writeInfd(ERR_NOSUCHCHANNEL(client, _channelInvite), i, _pfds);
    return 1;
}

int Invite::InviteClient(User *_tabUser, std::deque<struct pollfd> _pfds, int y)
{
    int i;
    int verif = 0;
    for (i = 0; i < MAX_USERS; i++)
    {
        if (_tabUser[i].getNickname() == _nickInvite)
        {
            verif = 1;
            break ;
        }
    }

    if (verif == 0)
        return 1;

    if (clientIsChannelOperator(_channelInvite, _tabUser, y, _pfds) == 1)
        return 2;
    
    std::string message = ":" + _tabUser[y].getUsername() + " INVITE " + _nickInvite + " " + _channelInvite + "\r\n";
    write(_pfds[i].fd, message.c_str(), message.size());
    write(_pfds[y].fd, message.c_str(), message.size());
    _tabUser[i]._mapModeUser[true].push_back(this->_channelInvite);
    return 0;
}

int Invite::User_on_channel(const std::map<std::string, std::list<std::string> >& channel, User *_tabUser)
{
    int y;
    int exist = 0;

    for (y = 1; y < MAX_USERS; y++)
    {
        if (_tabUser[y].getNickname() == _nickInvite)
        {
            exist = 1;
            break;
        }
    }
    if (exist == 0)
        return (0);

    for (std::map<std::string, std::list<std::string> >::const_iterator it = channel.begin(); it != channel.end(); ++it) {
        if (it->first == this->_channelInvite)
        {
            for (std::list<std::string>::const_iterator subIt = it->second.begin(); subIt != it->second.end(); ++subIt)
            {
                if (*subIt == _tabUser[y].getNickname()){
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* 
    Make a : ERR_CHANOPRIVSNEEDED (482)
*/

void Invite::execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel)
{
    ParseInviteCmd(str);

    if (_cmd.empty() || _nickInvite.empty() || _channelInvite.empty())
    {
        writeInfd(ERR_NEEDMOREPARAMS(_tabUser[y].getNickname(), _cmd), y, _pfds);
        return ;
    }

    if (ExistChannel(channel.mapChannel, _pfds, y, _nickInvite) == 1)
        return ;

    if (isInChannel(_channelInvite, _tabUser[y].getNickname(), channel) == 1){
        writeInfd(ERR_NOTONCHANNEL(_tabUser[y].getNickname(), _cmd), y, _pfds);
        return ;
    }

    if (User_on_channel(channel.mapChannel, _tabUser) == 1){
        writeInfd(ERR_USERONCHANNEL(_tabUser[y].getNickname(), _cmd), y, _pfds);
        return ;
    }
    
    int verif = InviteClient(_tabUser, _pfds, y);
    if (verif == 1)
    {
        writeInfd(ERR_INVALIDINPUT(_tabUser[y].getNickname(), _cmd), y, _pfds);
        return ;
    }
    else if (verif == 2)
        return ;
}