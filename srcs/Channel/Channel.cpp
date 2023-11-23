# include "../../includes/Channel/Channel.hpp"

Channel::Channel(void){}

Channel::Channel(Channel const &src)
{
    (void)src;
}

Channel &Channel::operator=(Channel const &rhs)
{
    (void)rhs;
    return (*this);
}

Channel::~Channel(void){}

int isInChannel(std::string channelFind, std::string nick, Channel &channel)
{
    int channelExist = 0;
    
    std::map<std::string, std::list<std::string> >::iterator it = channel.mapChannel.begin();
    for (; it != channel.mapChannel.end(); ++it)
    {
        if (it->first == channelFind)
        {
            channelExist = 1;
            break;
        }
    }
    if (channelExist != 0)
    {
        for (std::list<std::string>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
        {
            if (*it2 == nick)
                return (0);
        }
    }
    return (1);
}

int isModePresentInChannel(Channel &channel, std::string channelFind, char mode)
{
    for (std::map<std::string, std::vector<char> >::iterator it = channel.mapMode.begin(); it != channel.mapMode.end(); ++it)
    {
        if (channelFind == it->first)
        {

            for (std::vector<char>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
            {
                if (*vecIt == mode)
                    return (1);
            }
        }
    }
    return (0);
}