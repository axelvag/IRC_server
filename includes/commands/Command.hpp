# ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../IRC.hpp"
#include "../Channel/Channel.hpp"


class Command
{
    private:

    
    public:
        Command(void);
        Command(Command const &src);
        Command &operator=(Command const &rhs);
        virtual ~Command(void);

        virtual void execute_cmd(std::string str) = 0;

};

class Invite: public Command
{
    private:
        bool _isActive;
        std::string _nickInvite;
        std::string _channelInvite;
        std::string _cmd;

    public:
        Invite(void);
        Invite(Invite const &src);
        ~Invite(void);
        Invite &operator=(Invite const &rhs);
        int User_on_channel(const std::map<std::string, std::list<std::string> >& channel, User *_tabUser);
        int InviteClient(User *_tabUser, std::deque<struct pollfd> _pfds, int y);
        int ExistChannel(const std::map<std::string, std::list<std::string> >& channel, std::deque<struct pollfd> _pfds, int i, std::string &client);
        void ParseInviteCmd(std::string &str);
        void execute_cmd(std::string str);
        void execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel);
};

class Kick: public Command
{
    private:
        std::string _user;
        std::string _channel;
        std::string _reason;

    public:
        Kick(void);
        ~Kick(void);
        Kick(Kick const &src);
        Kick &operator=(Kick const &rhs);
        void parse_cmd(std::string str);
        void execute_cmd(std::string str);
        void execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel);
        int verif_is_on_channel(std::string list);
};

class Mode: public Command
{
    private:
        std::string _channelMode;
        std::string _opt;
        std::string _who;
        int        _limit;

    public:
        Mode(void);
        ~Mode(void);
        Mode(Mode const &src);
        Mode &operator=(Mode const &rhs);
        void execute_cmd(std::string str);
        void execute_cmd(std::string str, Channel &channel);
        void changeMode(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds);
        void addMode(char mode, Channel &channel);
        void removeMode(char mode, Channel &channel);
        std::string getChannelMode(void);
        std::string getWho(void);
        void addModeO(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void removeModeO(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void addRemoveChanOperator(Channel &channel, User *_tabUser, int index, bool isAdd, std::deque<struct pollfd> _pfds);
        void addModeT(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void removeModeT(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void addModeK(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void removeModeK(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void addModeL(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds);
        void removeModeL(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds);
        void addModeI(Channel &channel, User *_tabUser, int index, std::deque<struct pollfd> _pfds);
        void removeModeI(Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds);

        int isWhoInChannel(Channel &channel);
        int isUserChannelOperatorInChannel(User *_tabUser, int index);

        void unknowMode(User *_tabUser, int index, std::deque<struct pollfd> _pfds, int i);
};

class Topic: public Command
{
    private:
        std::string _channelTopic;
        std::string _msgTopic;

    public:
        Topic(void);
        ~Topic(void);
        Topic(Topic const &src);
        Topic &operator=(Topic const &rhs);
        void execute_cmd(std::string str);
        void execute_cmd(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel);
        void checkTopic(std::string tmpChannel, Channel &channel, User *_tabUser, int i, std::deque<struct pollfd> _pfds);
        void rplTopic(User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel);
        void rplTopicWhoTime(User *_tabUser, int i, std::deque<struct pollfd> _pfds);
        std::string getChannelTopic(void);
        std::string getMsgTopic(void);
        void getCurrentTime(struct timeval& tv);
        std::string intToString(int value);

};

class Join: public Command
{
    private:
        std::vector<std::string> _tokensChannel;
        std::vector<std::string> _tokensKey;
        std::string _channelJoin;
        std::string _cmd;
        int _isMode;
        int _isInvitation;
    public:
        std::string nameChannel;
        Join(void);
        ~Join(void);
        Join(Join const &src);
        Join &operator=(Join const &rhs);

        int verifModeI(User *_tabUser, int y, std::string &tokenChannel);
        int verifModeK(Channel &channel, User *_tabUser, int y, std::string &tokenChannel);
        int verifModeL(Channel &channel, User *_tabUser, int y, std::string &tokenChannel);

        void connectChannelKey(Channel &channel);
        void execute_cmd(std::string str);
        int verifModeChannel(Channel &channel, User *_tabUser, int y, std::string &tokenChannel);
        void add_user_inChannel(Channel &channel, User *_tabUser, Join &join, int i, std::deque<struct pollfd> _pfds, std::string tokenChannel);
        int ParseJoinCmd(std::string &str, Channel &channel);
        void execute_cmd(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel, Join &join);
};

class Message: public Command
{
private:
    void   messageToChannel(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel, int pos);
    void   messageToChannelOp(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel, int pos);
    void   messageToSomeone(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, int pos);
public:
    Message(void);
    ~Message();
    Message(Message const &src);
    Message &operator=(Message const &rhs);
    void execute_cmd(std::string str);
    void execute_cmd(std::string str, User *_tabUser, int i, std::deque<struct pollfd> _pfds, Channel &channel);
};

class Part: public Command
{
    private:
        std::string _reason;
        std::vector<std::string> channel;

    public:
        Part(void);
        ~Part(void);
        Part(Part const &src);
        Part &operator=(Part const &rhs);
        void parse_cmd(std::string str);
        void execute_cmd(std::string str);
        void execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel);
        int verif_is_on_channel(std::string list, User *_tabUser, int y);
};

class Quit: public Command
{
    private:
        std::string _reason;

    public:
        Quit(void);
        ~Quit(void);
        Quit(Quit const &src);
        Quit &operator=(Quit const &rhs);
        void parse_cmd(std::string str);
        void execute_cmd(std::string str);
        void execute_cmd(std::string str, std::deque<struct pollfd> _pfds, User *_tabUser, int y, Channel &channel);
};

#endif