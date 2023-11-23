#include "../../includes/commands/Command.hpp"
# include "../../includes/Utils.hpp"
# include "../../includes/Server/Server.hpp"

Join::Join(void){}

void Join::execute_cmd(std::string str){(void)str;}

int Join::verifModeI(User *_tabUser, int y, std::string &tokenChannel)
{
    _isMode = 1;
    for (std::map<bool, std::list<std::string> >::iterator it = _tabUser[y]._mapModeUser.begin(); it != _tabUser[y]._mapModeUser.end(); ++it)
    {
        if (it->first == true)
        {
            for (std::list<std::string>::const_iterator subIt = it->second.begin(); subIt != it->second.end(); ++subIt)
            {
                if (*subIt == tokenChannel)
                    _isInvitation = 1;
            }
        }
    }

    if (_isMode == 1 && _isInvitation != 1)
        return 1;

    return (0);
}

int Join::verifModeK(Channel &channel, User *_tabUser, int y, std::string &tokenChannel)
{
    if (!_tokensKey.size())
        return 2;
    for (size_t i = 0; i < _tokensKey.size(); i++)
    {
        std::string list = listUserChannel(channel.mapChannel, _tabUser, tokenChannel, y);
        if (list.empty())
            connectChannelKey(channel);
        else
        {
            for (std::map<std::string, std::string>::iterator it = channel._mapChannelKey.begin(); it != channel._mapChannelKey.end(); ++it)
            {
                if (it->first == _tokensChannel[i])
                {
                    if (it->second != _tokensKey[i])
                        return 2;
                }
            }
        }
    }
    return 0;
}

int Join::verifModeL(Channel &channel, User *_tabUser, int y, std::string &tokenChannel)
{
    std::string list = listUserChannel(channel.mapChannel, _tabUser, tokenChannel, y);
    std::istringstream iss(list);
    std::string token;
    int number = 0;
    while (iss >> token)
        number++;
    for (std::map<std::string, int>::iterator it = channel._mapChannelLimit.begin(); it != channel._mapChannelLimit.end(); ++it)
    {
        if (it->first == tokenChannel)
        {
            if (it->second <= number)
                return 3;
        }
    }
    return 0;
}

void Join::connectChannelKey(Channel &channel)
{
    for (size_t i = 0; i < _tokensKey.size(); i++)
    {
        channel._mapChannelKey[_tokensChannel[i]] = _tokensKey[i];
    }
}

int Join::ParseJoinCmd(std::string &str, Channel &channel)
{
    std::istringstream iss(str);
    std::string token;

    _tokensChannel.clear();
    _tokensKey.clear();
    while (std::getline(iss, token, ' '))
    {
        std::istringstream iss2(token);
        std::string subtoken;

        while (std::getline(iss2, subtoken, ','))
        {
            ft_trim(subtoken);
            if (subtoken == "JOIN")
                _cmd = subtoken;
            else if (subtoken[0] == '#' || subtoken[0] == '&')
            {
                if (subtoken[1] == '\0' || subtoken[1] == ' ')
                    return 1;
                _tokensChannel.push_back(subtoken);
            }
            else
            {
                _tokensKey.push_back(subtoken);
            }
                
        }
    }

    for (size_t i = 0; i < _tokensKey.size(); i++)
        channel.mapMode[_tokensChannel[i]].push_back('k');

    return 0;
}

void Join::add_user_inChannel(Channel &channel, User *_tabUser, Join &join, int i, std::deque<struct pollfd> _pfds, std::string tokenChannel)
{
    (void)join;
    std::string list;
    std::string word;

    channel.mapChannel[tokenChannel].push_back(_tabUser[i].getNickname());
    list = listUserChannel(channel.mapChannel, _tabUser, tokenChannel, i);
    std::istringstream ss(list);
    while (ss >> word)
    {
        for (int j = 1;j < MAX_USERS; j++)
        {
            if (word == _tabUser[j].getNickname() || word == "@" + _tabUser[j].getNickname())
                writeInfd(RPL_NAMREPLY(_tabUser[i].getNickname(), tokenChannel, list), j, _pfds);
        }
    }
}


int Join::verifModeChannel(Channel &channel, User *_tabUser, int y, std::string &tokenChannel)
{
    _isMode = 0;
    _isInvitation = 0;

    for (std::map<std::string, std::vector<char> >::iterator it = channel.mapMode.begin(); it != channel.mapMode.end(); ++it)
    {
        if (tokenChannel == it->first)
        {
            for (std::vector<char>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
            {
                if (*vecIt == 'i')
                {
                    if (verifModeI(_tabUser, y, tokenChannel) == 1)
                        return 1;
                }
                else if (*vecIt == 'k')
                {
                    if (verifModeK(channel, _tabUser, y, tokenChannel) == 2)
                        return 2;
                }
                else if (*vecIt == 'l')
                {
                    if (verifModeL(channel, _tabUser, y, tokenChannel) == 3)
                        return 3;
                }
            }
        }
    }
    
    return 0;
}

void Join::execute_cmd(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel, Join &join)
{
    if (str.find('#') == std::string::npos && str.find('&') == std::string::npos)
    {
        std::string name = "channel";
        writeInfd(ERR_BADCHANMASK(name), i, _pfds);
        return ;
    }
    if (ParseJoinCmd(str, channel) == 1)
    {
        writeInfd(ERR_NOSUCHCHANNEL(_tabUser[i].getUsername(), "#"), i, _pfds);
        return ;
    }

    for (size_t k = 0; k < _tokensChannel.size(); k++)
    {
        int result = verifModeChannel(channel, _tabUser, i, _tokensChannel[k]);
        if (result == 1)
        {
            writeInfd(ERR_INVITEONLYCHAN(_tabUser[i].getUsername(), _tokensChannel[k]), i, _pfds);
            return ;
        }
        else if (result == 2)
        {
            writeInfd(ERR_BADCHANNELKEY(_tabUser[i].getUsername(), _tokensChannel[k]), i, _pfds);
            return ;
        }
        else if (result == 3)
        {
            writeInfd(ERR_CHANNELISFULL(_tabUser[i].getUsername(), _tokensChannel[k]), i, _pfds);
            return ;
        }
    }
    for (size_t j = 0; j < _tokensChannel.size() ; j++)
    {
        if (isInChannel(_tokensChannel[j], _tabUser[i].getNickname(), channel) == 0)
        {
            writeInfd(ERR_USERONCHANNEL(_tabUser[i].getNickname(), _tokensChannel[j]), i, _pfds);
            return ;
        }
        writeInfd(RPL_JOIN(_tabUser[i].getNickname(), _tokensChannel[j]), i, _pfds);
        add_user_inChannel(channel, _tabUser, join, i, _pfds, _tokensChannel[j]);
    }
}

Join::~Join(){}