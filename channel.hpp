#ifndef FT_IRC3_CHANNEL_HPP
#define FT_IRC3_CHANNEL_HPP

#include <list>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>

class Channel {
private:
    int             _invite_only;
    int             _topic_restriction;
    int             _password_set;
    int             _limit_client;
	std::string     _pass;
    std::list<int>  _full_list;
	std::list<int>  _oper;
	std::list<int>  _ban;
    std::string     _topic;
	int				_host;

public:
    Channel(int fd);
	Channel(int fd, std::string pass);
	Channel();
    ~Channel();

    int add_client(int fd_key);
    int add_client(int fd_key, std::string pass);
    int add_client_ban();
	int add_oper(int fd_key);

    void remove_client(int fd_key);
	void remove_oper(int fd_key);

    void set_topic(std::string topic);
    void set_mode_invite(int flag);
	void set_mode_topic(int flag);
	void set_password(std::string pass, int flag);
	void set_limit(std::string number);

    std::string     get_topic();
    int             get_topic_restriction();
    std::list<int>  get_list_user();
    std::string     get_pass();
	int				get_invite_only();

    int find_client(int fd_key);
    int find_oper(int fd_key);
	int find_host(int fd_key);
};


#endif //FT_IRC3_CHANNEL_HPP
