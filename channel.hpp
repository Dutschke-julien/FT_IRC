//
// Created by Antoine Ho on 9/19/23.
//

#ifndef FT_IRC3_CHANNEL_HPP
#define FT_IRC3_CHANNEL_HPP

#include <list>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

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

public:
    Channel(int fd_oper);
	Channel(int fd_oper, std::string pass);
	Channel();
    ~Channel();

    void set_topic(std::string topic);
    std::string get_topic();
    int get_topic_restriction();
    int add_client(int fd_key);
	int add_client(int fd_key, std::string pass);
    int add_client_ban();
    int remove_client();
    int find_client(int fd_key);
    int find_oper(int fd_key);
    std::list<int> get_list_user();
    std::string get_pass();
};


#endif //FT_IRC3_CHANNEL_HPP
