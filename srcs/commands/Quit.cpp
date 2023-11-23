#include "../../includes/commands/Command.hpp"

Quit::Quit(void){}

void Quit::execute_cmd(std::string str)
{
    (void)str;
}

Quit::Quit(Quit const &src):Command(src)
{
    this->_reason = src._reason;
}

Quit &Quit::operator=(Quit const &rhs)
{
    Command::operator=(rhs);
    this->_reason = rhs._reason;
    return (*this);
}

void Quit::execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel)
{
    parse_cmd(str);
    str = ":" + _tabUser[y].getNickname() + " QUIT " + this->_reason + "\r\n";
    for (int j = 1; j < MAX_USERS; j++)
        send(_pfds[j].fd, str.c_str(), str.size(), 0);
    for (std::map<std::string, std::list<std::string> >::const_iterator it = channel.mapChannel.begin(); it != channel.mapChannel.end(); ++it)
    {
        channel.mapChannel[it->first].erase(std::remove(channel.mapChannel[it->first].begin(), channel.mapChannel[it->first].end(), _tabUser[y].getNickname()), channel.mapChannel[it->first].end());
    }
    _tabUser[y]._chanOperator.erase(_tabUser[y]._chanOperator.begin(), _tabUser[y]._chanOperator.end());
    _tabUser[y].setNickname("");
    _tabUser[y].setUsername("");
}

void Quit::parse_cmd(std::string str)
{
    size_t pos;

    ft_trim(str);
    pos = str.find(":");
    if (pos != std::string::npos)
        this->_reason = str.substr(pos + 1, str.size() - pos);
}

Quit::~Quit(){}