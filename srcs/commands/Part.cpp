#include "../../includes/commands/Command.hpp"

Part::Part(void){}

void Part::execute_cmd(std::string str)
{
    (void)str;
}

Part::Part(Part const &src):Command(src)
{
    this->channel = src.channel;
    this->_reason = src._reason;
}

Part &Part::operator=(Part const &rhs)
{
    Command::operator=(rhs);
    this->channel = rhs.channel;
    this->_reason = rhs._reason;
    return (*this);
}

void Part::execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel)
{
    parse_cmd(str);
    for (size_t k = 0; k < this->channel.size(); k++)
    {
        if (this->channel[k].empty())
        {
            writeInfd(ERR_NEEDMOREPARAMS(_tabUser[y].getNickname(), "PART"), y, _pfds);
            return;
        }
        str = ":" + _tabUser[y].getNickname() + " PART " + this->channel[k] + "\r\n";
        std::string list = listUserChannel(channel.mapChannel, _tabUser, this->channel[k], y);
        if (list.empty())
        {
            writeInfd(ERR_NOSUCHCHANNEL(_tabUser[y].getNickname(), this->channel[k]), y, _pfds);
            return;
        }
        if (verif_is_on_channel(list, _tabUser, y) == 1)
        {
            writeInfd(ERR_NOTONCHANNEL(_tabUser[y].getNickname(), this->channel[k]), y, _pfds);
            return;
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
        channel.mapChannel[this->channel[k]].erase(std::remove(channel.mapChannel[this->channel[k]].begin(), channel.mapChannel[this->channel[k]].end(), _tabUser[y].getNickname()), channel.mapChannel[this->channel[k]].end());
        _tabUser[y]._chanOperator.erase(std::remove(_tabUser[y]._chanOperator.begin(), _tabUser[y]._chanOperator.end(), this->channel[k]), _tabUser[y]._chanOperator.end());
    }
}

void Part::parse_cmd(std::string str)
{
    size_t pos;

    ft_trim(str);
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, ' '))
    {
        std::istringstream iss2(token);
        std::string subtoken;

        while (std::getline(iss2, subtoken, ','))
        {
            if (subtoken[0] == '#' || subtoken[0] == '&')
                channel.push_back(subtoken);
        }
    }
    pos = str.find(":");
    if (pos != std::string::npos)
        this->_reason = str.substr(pos + 1, str.size() - pos);
}

int Part::verif_is_on_channel(std::string list, User *_tabUser, int y)
{
    std::istringstream ss(list);
    std::string word;
    while (ss >> word)
    {
        if (word == _tabUser[y].getNickname() || word == "@" + _tabUser[y].getNickname())
            return 0;
    }
    return 1;
}

Part::~Part(){}
