#include "../../includes/commands/Command.hpp"
#include "../../includes/IRC.hpp"
#include "../../includes/Utils.hpp"

Mode::Mode(void){}

void Mode::execute_cmd(std::string str)
{
    (void)str;
}

Mode::Mode(Mode const &src):Command(src)
{
    this->_channelMode = src._channelMode;
    this->_who = src._who;
    this->_opt = src._opt;
}

Mode &Mode::operator=(Mode const &rhs)
{
    Command::operator=(rhs);
    this->_channelMode = rhs._channelMode;
    this->_who = rhs._who;
    this->_opt = rhs._opt;
    return (*this);
}

void Mode::execute_cmd(std::string str, Channel &channel)
{
    size_t endChannel = str.find(" ", 6);
    size_t startOpt;
    size_t endOpt;
    std::string tmpChannel;
    std::string tmpOpt;
    std::string tmpWho;

    if (str.find('#') == std::string::npos)
        return ;
    tmpChannel = str.substr(5, endChannel - 5);
    if (tmpChannel.size() < 2)
        return ;
    this->_channelMode = tmpChannel;
    if (str.find("+") == std::string::npos && str.find("-") == std::string::npos)
        return ;
    if (str.find("+") != std::string::npos && str.find("-") != std::string::npos)
    {
        int tmpPosAdd = str.find("+");
        int tmpPosRemove = str.find("-");
        if (tmpPosAdd < tmpPosRemove)
            startOpt = tmpPosAdd;
        else
            startOpt = tmpPosRemove;
    }
    else
    {
        if (str.find("-") == std::string::npos)
            startOpt = str.find("+");
        else
            startOpt = str.find("-");
    }
    for (int i = startOpt; i < (int)str.size(); i++)
    {
        if (str[i] == ' ')
        {
            endOpt = i;
            break ;
        }
        else if (i == (int)str.size() - 1)
            endOpt = i + 1;
    }
    tmpOpt = str.substr(startOpt, endOpt - startOpt);
    this->_opt = tmpOpt;
    tmpWho = str.substr(endOpt, str.size() - endOpt);
    ft_trim(tmpWho);
    this->_who = tmpWho;
    channel.mapTopic[this->getChannelMode()];
}

Mode::~Mode(){}

void Mode::changeMode(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    if (this->_opt.size() < 2)
        return ;
    for (size_t i = 1; i < this->_opt.size(); i++)
    {
        if (this->_opt[i] == 'i')
        {
            if (this->_opt[0] == '+')
                addModeI(channel, _tabUser, index, _pfds);
            else
                removeModeI(channel, _tabUser, index, _pfds);
        }
        else if (this->_opt[i] == 't')
        {
            if (this->_opt[0] == '+')
                addModeT(channel, _tabUser, index, _pfds);
            else if (this->_opt[0] == '-')
                removeModeT(channel, _tabUser, index, _pfds);
        }
        else if (this->_opt[i] == 'k')
        {
            if (this->_opt[0] == '+')
                addModeK(channel, _tabUser, index, _pfds);
            else if (this->_opt[0] == '-')
                removeModeK(channel, _tabUser, index, _pfds);
        }
        else if (this->_opt[i] == 'o')
        {
            if (this->_opt[0] == '+')
                addModeO(channel, _tabUser, index, _pfds);
            else if (this->_opt[0] == '-')
                removeModeO(channel, _tabUser, index, _pfds);
        }
        else if (this->_opt[i] == 'l')
        {
            if (this->_opt[0] == '+')
                addModeL(channel, _tabUser, index, _pfds);
            else
                removeModeL(channel, _tabUser, index, _pfds);
        }
        else
            unknowMode(_tabUser, index, _pfds, i);
    }
}

void Mode::unknowMode(User *_tabUser, int index, std::deque<struct pollfd> _pfds, int i)
{
    if (this->_opt[i] != '\r' && this->_opt[i] != '\n')
    {
        std::stringstream ss;
        ss << this->_opt[i];
        std::string mode = ss.str();
        std::string message = ERR_UNKNOWNMODE(_tabUser[index].getUsername(), mode);
        writeInfd(message, index, _pfds);
    }
}

void Mode::addModeL(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds)
{
    if (!_who.size())
    {
        writeInfd(ERR_NEEDMOREPARAMS(_tabUser[i].getNickname(), "Usage /mode (+l)"), i, _pfds);
        return ;
    }

    if (clientIsChannelOperator(_channelMode, _tabUser, i, _pfds) == 1)
        return ;

    _limit = std::atoi(_who.c_str());
    if (_limit <= 0)
    {
        writeInfd(ERR_INVALIDINPUT(_tabUser[i].getNickname(), "Usage /mode (+l)"), i, _pfds);
        return ;
    }

    std::string list = listUserChannel(channel.mapChannel, _tabUser, _channelMode, i);
    std::istringstream iss(list);
    std::string token;
    int number = 0;
    while (iss >> token)
        number++;
    if (number > _limit)
    {
        writeInfd(ERR_INVALIDINPUT(_tabUser[i].getNickname(), "Usage /mode (+l), inconsistency"), i, _pfds);
        return ;
    }

    channel._mapChannelLimit[_channelMode] = _limit;
    addMode('l', channel);
    std::string message = RPL_MODEADDL(_tabUser[i].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()), this->getWho());
    writeInfd(message, i, _pfds);
}

void Mode::removeModeL(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds)
{
    channel._mapChannelLimit[_channelMode] = MAX_USERS;
    removeMode('l', channel);
    std::string message = RPL_MODEREMOVEL(_tabUser[i].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()));
    writeInfd(message, i, _pfds);
}

void Mode::addMode(char mode, Channel &channel)
{
    std::vector<char>::iterator iterator = channel.mapMode[this->_channelMode].begin();
    while (iterator != channel.mapMode[this->_channelMode].end())
    {
        if (*iterator == mode)
            return ;
        iterator++;
    }
    channel.mapMode[this->_channelMode].push_back(mode);
}

void Mode::removeMode(char mode, Channel &channel)
{
    std::vector<char>::iterator iterator = channel.mapMode[this->_channelMode].begin();
    while (iterator != channel.mapMode[this->_channelMode].end())
    {
        if (*iterator == mode)
        {
            channel.mapMode[this->_channelMode].erase(iterator);
            return ;
        }
        iterator++;
    }
}

std::string Mode::getChannelMode(void)
{
    return (this->_channelMode);
}

std::string Mode::getWho(void)
{
    return (this->_who);
}

void Mode::addModeO(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    if (clientIsChannelOperator(_channelMode, _tabUser, index, _pfds) == 1)
        return ;
    if (this->_who == "")
    {
        std::string message = ERR_NEEDMOREPARAMS(_tabUser[index].getNickname(), "MODE o");
        writeInfd(message, index, _pfds);
        return ;
    }
    _tabUser[index].setOperateur(true);
    addRemoveChanOperator(channel, _tabUser, index, 1, _pfds);
    addMode('o', channel);
}


void Mode::removeModeO(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    if (clientIsChannelOperator(_channelMode, _tabUser, index, _pfds) == 1)
        return ;
    if (this->_who == "")
    {
        std::string message = ERR_NEEDMOREPARAMS(_tabUser[index].getNickname(), "MODE o");
        writeInfd(message, index, _pfds);
        return ;
    }
    addRemoveChanOperator(channel, _tabUser, index, 0, _pfds);
    removeMode('o', channel);
}

void Mode::addModeI(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    if (clientIsChannelOperator(_channelMode, _tabUser, index, _pfds) == 1)
        return ;
    addMode('i', channel);
    std::string message = RPL_MODEADDI(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()));
    writeInfd(message, index, _pfds);
}

void Mode::removeModeI(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds)
{
    if (clientIsChannelOperator(_channelMode, _tabUser, i, _pfds) == 1)
        return ;
    removeMode('i', channel);
    std::string message = RPL_MODEREMOVEI(_tabUser[i].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()));
    writeInfd(message, i, _pfds);
}

int Mode::isWhoInChannel(Channel &channel)
{
    std::list<std::string>::iterator iterator = channel.mapChannel[this->_channelMode].begin();

    while (iterator != channel.mapChannel[this->_channelMode].end())
    {
        if (*iterator == this->_who)
            return (0);
        iterator++;
    }
    return (1);
}

int Mode::isUserChannelOperatorInChannel(User *_tabUser, int index)
{
    std::list<std::string>::iterator iterator = _tabUser[index]._chanOperator.begin();

    while (iterator != _tabUser[index]._chanOperator.end())
    {
        if (*iterator == this->_channelMode)
            return (0);
        iterator++;
    }
    return (1);
}

void Mode::addRemoveChanOperator(Channel &channel, User *_tabUser, int index, bool isAdd, std::deque<struct pollfd> _pfds)
{
    if (isWhoInChannel(channel))
    {
        std::string message = ERR_USERNOTINCHANNEL(_tabUser[index].getNickname(), this->_who, this->_channelMode);
        writeInfd(message, index, _pfds);
        return ;
    }
    if (isUserChannelOperatorInChannel(_tabUser, index))
    {
        std::string message = ERR_CHANOPRIVSNEEDED(_tabUser[index].getNickname(), this->_channelMode);
        writeInfd(message, index, _pfds);
        return ;
    }
    int who = 0;
    for (; who < MAX_USERS; who++)
    {
        if (_tabUser[who].getNickname() == this->_who)
        {
            break;
        }
    }
    if (std::find(_tabUser[who]._chanOperator.begin(), _tabUser[who]._chanOperator.end(), this->_channelMode) == _tabUser[who]._chanOperator.end())
    {
        if (isAdd == 1)
        {
            _tabUser[who]._chanOperator.push_back(this->_channelMode);
            std::string message = RPL_MODEADDO(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()), this->getWho());
            sendAll(message, channel, _tabUser, index, _pfds, this->_channelMode);
        }
    }
    else
    {
        if (isAdd != 1)
        {
            if (_tabUser[who].getNickname() == _tabUser[index].getNickname())
                return ;
            _tabUser[who]._chanOperator.erase(std::remove(_tabUser[who]._chanOperator.begin(), _tabUser[who]._chanOperator.end(), this->_channelMode), _tabUser[who]._chanOperator.end());
            std::string message = RPL_MODEREMOVEO(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()), this->getWho());
            sendAll(message, channel, _tabUser, index, _pfds, this->_channelMode);
        }
    }
}

void Mode::addModeT(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    if (isUserChannelOperatorInChannel(_tabUser, index))
    {
        std::string message = ERR_CHANOPRIVSNEEDED(_tabUser[index].getNickname(), this->_channelMode);
        writeInfd(message, index, _pfds);
        return ;
    }
    addMode('t', channel);
    std::string message = RPL_MODEADDT(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()));
    writeInfd(message, index, _pfds);
}

void Mode::removeModeT(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    if (isUserChannelOperatorInChannel(_tabUser, index))
    {
        std::string message = ERR_CHANOPRIVSNEEDED(_tabUser[index].getNickname(), this->_channelMode);
        writeInfd(message, index, _pfds);
        return ;
    }
    removeMode('t', channel);
    std::string message = RPL_MODEREMOVET(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()));
    writeInfd(message, index, _pfds);
}

void Mode::addModeK(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    (void)_tabUser;
    (void)index;
    (void)_pfds;
    if (isUserChannelOperatorInChannel(_tabUser, index))
    {
        std::string message = ERR_CHANOPRIVSNEEDED(_tabUser[index].getNickname(), this->_channelMode);
        writeInfd(message, index, _pfds);
        return ;
    }
    if (!this->_who.size())
    {
        std::string message = ERR_NEEDMOREPARAMS(_tabUser[index].getNickname(), "MODE k");
        writeInfd(message, index, _pfds);
        return ;
    }
    channel._mapChannelKey[this->_channelMode] = this->_who;
    addMode('k', channel);
    std::string message = RPL_MODEADDK(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()), this->getWho());
    writeInfd(message, index, _pfds);
}

void Mode::removeModeK(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds)
{
    (void)_tabUser;
    (void)index;
    (void)_pfds;
    
    if (isUserChannelOperatorInChannel(_tabUser, index))
    {
        std::string message = ERR_CHANOPRIVSNEEDED(_tabUser[index].getNickname(), this->_channelMode);
        writeInfd(message, index, _pfds);
        return ;
    }

    std::map<std::string, std::string>::iterator iterator = channel._mapChannelKey.find(this->_channelMode);
    
    if (iterator == channel._mapChannelKey.end())
    {
        std::string message = ERR_NEEDMOREPARAMS(_tabUser[index].getNickname(), "MODE k");
        writeInfd(message, index, _pfds);
        return ;
    }
    channel._mapChannelKey.erase(iterator);
    removeMode('k', channel);
    std::string message = RPL_MODEREMOVEK(_tabUser[index].getNickname(), this->_channelMode.substr(1, this->_channelMode.size()));
    writeInfd(message, index, _pfds);
}
