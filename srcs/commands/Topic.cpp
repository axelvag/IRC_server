#include "../../includes/commands/Command.hpp"
#include "../../includes/Channel/Channel.hpp"

Topic::Topic(void){}

Topic::~Topic(){}

Topic::Topic(Topic const &src):Command(src)
{
    this->_channelTopic = src._channelTopic;
    this->_msgTopic = src._msgTopic;
}

Topic &Topic::operator=(Topic const &rhs)
{
    Command::operator=(rhs);
    this->_channelTopic = rhs._channelTopic;
    this->_msgTopic = rhs._msgTopic;
    return (*this);
}

bool isOnlySpace(std::string str)
{
    if (str.size() < 3)
        return (0);
    if (str[0] != ':')
        return (0);
    for (size_t i = 1; i < str.size() - 1; i++)
    {
        if (str[i] != ' ')
            return (0);
    }
    return (1);
}

void Topic::execute_cmd(std::string str)
{
    (void)str;
}

void Topic::execute_cmd(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel)
{
    size_t endChannel = str.find(" ", 7);
    size_t startTopic;
    std::string tmpChannel;
    std::string tmpTopic;

    if (str.find('#') == std::string::npos)
    {
        errNeedMoreParams(_tabUser[i].getNickname(), "TOPIC", i, _pfds);
        return ;
    }
    tmpChannel = str.substr(6, endChannel - 6);
    ft_trim(tmpChannel);
    if (isInChannel(tmpChannel, _tabUser[i].getNickname(), channel))
    {
        std::string message = ERR_NOTONCHANNEL(_tabUser[i].getNickname(), tmpChannel);
        writeInfd(message, i, _pfds);
        return ;
    }
    if (isModePresentInChannel(channel, tmpChannel, 't'))
    {
        if (clientIsChannelOperator(tmpChannel, _tabUser, i, _pfds))
            return ;
    }
    if (str.find(":") == std::string::npos)
    {
        checkTopic(tmpChannel, channel, _tabUser, i, _pfds);
        return ;
    }
    this->_channelTopic = tmpChannel;
    startTopic = str.find(":");
    tmpTopic = str.substr(startTopic);
    ft_trim(tmpTopic);
    if ((tmpTopic.size() == 1 && (tmpTopic[0] == ':' && tmpTopic[1] != ':')) || isOnlySpace(tmpTopic))
    {
        this->_msgTopic = "";
        channel.mapTopic[this->getChannelTopic()] = this->getMsgTopic();
        this->rplTopic(_tabUser, i, _pfds, channel);
        return ;
    }
    this->_msgTopic = tmpTopic.substr(1);
    this->rplTopicWhoTime(_tabUser, i, _pfds);
    this->rplTopic(_tabUser, i, _pfds, channel);
    channel.mapTopic[this->getChannelTopic()] = this->getMsgTopic();
}

void Topic::rplTopic(User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel)
{
    std::string message = RPL_TOPIC(_tabUser[i].getNickname(), this->_channelTopic, this->_msgTopic);
    sendAll(message, channel, _tabUser, i, _pfds, this->_channelTopic);
}

void Topic::rplTopicWhoTime(User *_tabUser, int i, std::deque<struct pollfd> _pfds)
{
    struct timeval currentTime;
    getCurrentTime(currentTime);

    std::string message = RPL_TOPICWHOTIME(_tabUser[i].getNickname(), this->_channelTopic, _tabUser[i].getNickname(), intToString(currentTime.tv_sec));
    writeInfd(message, i, _pfds);
}

void Topic::checkTopic(std::string tmpChannel, Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds)
{
    this->_channelTopic = tmpChannel;
    this->_msgTopic = channel.mapTopic[this->getChannelTopic()];
    this->rplTopic(_tabUser, i, _pfds, channel);
}

std::string Topic::getChannelTopic(void)
{
    return (this->_channelTopic);
}

std::string Topic::getMsgTopic(void)
{
    return (this->_msgTopic);
}

void Topic::getCurrentTime(struct timeval& tv)
{
    gettimeofday(&tv, NULL);
}

std::string Topic::intToString(int value)
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}