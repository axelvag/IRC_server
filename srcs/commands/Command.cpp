#include "../../includes/commands/Command.hpp"

Command::Command()
{
}

Command::Command(Command const &src)
{
   *this = src; 
}

Command::~Command()
{
}

Command &Command::operator=(Command const &rhs)
{
    (void)rhs;
    return (*this);
}