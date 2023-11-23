# ifndef PARSING_CLASS_HPP
# define PARSING_CLASS_HPP

#include "../IRC.hpp"
#include "../includes/Channel/Channel.hpp"

class Parsing
{
    private:

    public:
       void whatCommand(char *buffer, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel); 
};

#endif