#include "../../includes/commands/Command.hpp"

Message::Message(void){}

void Message::execute_cmd(std::string str)
{
    (void)str;
}

Message::Message(Message const &src):Command(src){}

Message &Message::operator=(Message const &rhs)
{
    Command::operator=(rhs);
    return (*this);
}

void Message::execute_cmd(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel)
{
    size_t pos;
    int pos2;
    pos = str.find("#");
    if (pos != std::string::npos)
    {
        pos2 = pos;
        pos = str.find(":");
        if (str[pos + 1] <= 32)
        {
            writeInfd(ERR_NOTEXTTOSEND(_tabUser[i].getNickname()), i, _pfds);
            return;
        }
        if(str[pos2 - 1] == '%' && (str[pos2 - 2] == '@' || str[pos2 - 2] == ' '))
        {
            if (str[pos2 - 2] == '@' && str[pos2 - 3] != ' ')
                return;
            this->messageToChannelOp(str, _tabUser, i, _pfds, channel, pos2);
            return;
        }
        this->messageToChannel(str, _tabUser, i, _pfds, channel, pos2);
    }
    else
    {
        pos = str.find(":");
        if (str[pos + 1] <= 32)
        {
            writeInfd(ERR_NOTEXTTOSEND(_tabUser[i].getNickname()), i, _pfds);
            return;
        }
        pos = str.find(" ");
        if (pos != std::string::npos)
            this->messageToSomeone(str, _tabUser, i, _pfds, pos);
    }
}

Message::~Message(){}

void Message::messageToChannel(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel, int pos)
{
        std::string list;
        int j = pos;
        int size = 0;
        while (str[j] && str[j] != ' ')
        {
            j++;
            size++;
        }
        std::string channelName = str.substr(pos, size);
        list = listUserChannel(channel.mapChannel, _tabUser, channelName, i);
        str = ":" + _tabUser[i].getNickname() + " " + str + "\r\n";
        std::istringstream ss(list);
        std::string word;
        while (ss >> word)
        {
            for (int j = 1; j < MAX_USERS; j++)
            {
                if ((word == _tabUser[j].getNickname() || word == "@" + _tabUser[j].getNickname()) && word != _tabUser[i].getNickname() && word != "@" + _tabUser[i].getNickname())
                    send(_pfds[j].fd, str.c_str(), str.size(), 0);
            }
        }
}

void Message::messageToSomeone(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, int pos)
{
    int j = pos + 1;
    int size = 0;
    while (str[j] && str[j] != ' ')
    {
        j++;
        size++;
    }
    std::string destName = str.substr(pos + 1, size);
    str = ":" + _tabUser[i].getNickname() + " " + str + "\r\n";
    for (int j = 1; j < MAX_USERS; j++)
    {
        if (destName == _tabUser[j].getNickname())
        {
            send(_pfds[j].fd, str.c_str(), str.size(), 0);
            return;
        }
    }
    writeInfd(ERR_NOSUCHNICK(_tabUser[i].getNickname(), destName), i, _pfds);
}

void   Message::messageToChannelOp(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel, int pos)
{
    std::string list;
    std::string newStr;
    int j = pos;
    int size = 0;
    while (str[j] && str[j] != ' ')
    {
        j++;
        size++;
    }
    std::string channelName = str.substr(pos, size);
    list = listUserChannel(channel.mapChannel, _tabUser, channelName, i);
    str = str.substr(pos, str.size() - pos - 1);
    str = ":" + _tabUser[i].getNickname() + " PRIVMSG " + str + "\r\n";
    if (list.empty())
    {
        writeInfd(ERR_NOSUCHSERVER(_tabUser[i].getNickname(), channelName), i, _pfds);
        return;
    }
    std::istringstream ss(list);
    std::string word;
    while (ss >> word)
    {
        for (int j = 1; j < MAX_USERS; j++)
        {
            if (word == "@" + _tabUser[j].getNickname() && word != "@" + _tabUser[i].getNickname())
                send(_pfds[j].fd, str.c_str(), str.size(), 0);
        }
    }
}