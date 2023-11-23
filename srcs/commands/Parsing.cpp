#include "../../includes/commands/Parsing.hpp"
#include "../../includes/commands/Command.hpp"
#include "../../includes/Channel/Channel.hpp"
#include "../../includes/Server/Server.hpp"

void Parsing::whatCommand(char *buffer, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel)
{
    std::string str;
    std::istringstream iss(buffer);
    size_t pos;
    while (std::getline(iss, str))
    {
        pos = str.find("KICK");
        if (pos !=  std::string::npos)
        {
            Kick kick;
            kick.execute_cmd(str, _pfds, _tabUser, i, channel);
        }
        pos = str.find("INVITE");
        if (pos !=  std::string::npos)
        {
            Invite invite;
            invite.execute_cmd(str, _pfds, _tabUser, i, channel);
        }
        pos = str.find("TOPIC");
        if (pos !=  std::string::npos)
        {
            Topic topic; 
            topic.execute_cmd(str, _tabUser, i, _pfds, channel);
        }
        pos = str.find("JOIN");
        if (pos !=  std::string::npos)
        {

            Join join;
            join.execute_cmd(str, _tabUser, i, _pfds, channel, join);
        }
        pos = str.find("MODE");
        if (pos !=  std::string::npos)
        {
            Mode mode;
            mode.execute_cmd(str, channel);
            mode.changeMode(channel, _tabUser, i, _pfds);
        }
        pos = str.find("PRIVMSG");
        if (pos !=  std::string::npos)
        {
            Message message;
            message.execute_cmd(str, _tabUser, i, _pfds, channel);
        }
        pos = str.find("PART");
        if (pos !=  std::string::npos)
        {
            Part part;
            part.execute_cmd(str, _pfds, _tabUser, i, channel);
        }
        pos = str.find("QUIT");
        if (pos !=  std::string::npos)
        {
            Quit quit;
            quit.execute_cmd(str, _pfds, _tabUser, i, channel);
        }
    }
}