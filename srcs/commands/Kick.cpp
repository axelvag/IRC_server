#include "../../includes/commands/Command.hpp"

Kick::Kick(void){}

void Kick::execute_cmd(std::string str)
{
    (void)str;
}

Kick::Kick(Kick const &src):Command(src)
{
    this->_user = src._user;
    this->_reason = src._reason;
    this->_channel = src._channel;
}

Kick &Kick::operator=(Kick const &rhs)
{
    Command::operator=(rhs);
    this->_user = rhs._user;
    this->_reason = rhs._reason;
    this->_channel = rhs._channel;
    return (*this);
}

void Kick::execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel)
{
    parse_cmd(str);
    if (this->_channel.empty() || this->_user.empty())
    {
        writeInfd(ERR_NEEDMOREPARAMS(_tabUser[y].getNickname(), "KICK"), y, _pfds);
        return;
    }
    if (std::find(_tabUser[y]._chanOperator.begin(), _tabUser[y]._chanOperator.end(), this->_channel) == _tabUser[y]._chanOperator.end())
    {
        writeInfd(ERR_CHANOPRIVSNEEDED(_tabUser[y].getNickname(), this->_channel), y, _pfds);
        return ;
    }
    str = ":" + _tabUser[y].getNickname() + " " + str + "\r\n";
    std::string list = listUserChannel(channel.mapChannel, _tabUser, this->_channel, y);
    if (verif_is_on_channel(list) == 1)
    {
        writeInfd(ERR_USERNOTINCHANNEL(_tabUser[y].getNickname(), this->_user, this->_channel), y, _pfds);
        return ;
    }
    std::istringstream ss(list);
    std::string word;
    while (ss >> word)
    {
        for (int j = 1; j < MAX_USERS; j++)
        {
            if (word == _tabUser[j].getNickname() || word == "@" + _tabUser[j].getNickname())
                send(_pfds[j].fd, str.c_str(), str.size(), 0);
        }
    }
    channel.mapChannel[this->_channel].erase(std::remove( channel.mapChannel[this->_channel].begin(), channel.mapChannel[this->_channel].end(), this->_user), channel.mapChannel[this->_channel].end());
}

void Kick::parse_cmd(std::string str)
{
    size_t pos;
    size_t pos2 = 0;

    ft_trim(str);
    pos = str.find(" ") + 1;
    if (pos != std::string::npos)
    {
        pos2 = pos;
        while(str[pos2] != ' ' && str[pos2])
            pos2++;
        this->_channel = str.substr(pos, pos2 - pos);
    }
    pos = str.find(" ", pos2 + 1);
    if (pos != std::string::npos)
        this->_user = str.substr(pos2 + 1, pos - pos2 - 1);
    else
        this->_user = str.substr(pos2 + 1, str.size() - pos2);
    pos = str.find(":");
    if (pos != std::string::npos)
        this->_reason = str.substr(pos + 1, str.size() - pos);
}

int Kick::verif_is_on_channel(std::string list)
{
    std::istringstream ss(list);
    std::string word;
    while (ss >> word)
    {
        if (word == this->_user || word == "@" + this->_user)
            return 0;
    }
    return 1;
}

Kick::~Kick(){}
