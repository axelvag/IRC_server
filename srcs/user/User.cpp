#include "../includes/user/User.hpp"

User::User()
{
    this->_nickname = "";
    this->_username = "";
    this->_operator = 0;
}

User::User(User const &src)
{
    *this = src;
}

User::~User(){}

User User::operator=(User const &rhs)
{
    if (this == &rhs)
        return (*this);
    this->_nickname = rhs._nickname;
    this->_username = rhs._username;
    return(*this);
}

std::string User::getNickname(void)const
{
    return (this->_nickname);
}

void User::setNickname(std::string nickname)
{
    this->_nickname = nickname;
}

std::string User::getUsername(void)const
{
    return (this->_username);
}

void User::setUsername(std::string username)
{
    this->_username = username;
}

void User::setOperateur(bool ope)
{
    this->_operator = ope;
}

std::string User::getBufferSignal(void)
{
    return (this->_bufferSignal);
}

void User::setBufferSignal(std::string bufferSignal)
{
    this->_bufferSignal = bufferSignal;
}