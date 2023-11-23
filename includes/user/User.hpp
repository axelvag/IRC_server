# ifndef USER_HPP
#define USER_HPP

#include "../IRC.hpp"

class User
{
private:
    std::string _username;
    std::string _nickname;
    bool _operator;

public:
    std::string _bufferSignal;
    std::list<std::string> _chanOperator;
    
    std::map<bool, std::list<std::string> > _mapModeUser;
    User();
    User(User const &src);
    ~User();

    User operator=(User const &rhs);
    
    std::string getNickname(void)const;
    void setNickname(std::string nickname);
    std::string getUsername(void)const;
    void setUsername(std::string username);
    void setOperateur(bool ope);
    std::string getBufferSignal(void);
    void setBufferSignal(std::string bufferSignal);
};

#endif
