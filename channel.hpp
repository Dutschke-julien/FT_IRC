//
// Created by Antoine Ho on 9/19/23.
//

#ifndef FT_IRC3_CHANNEL_HPP
#define FT_IRC3_CHANNEL_HPP

#include <list>
#include <string>

class Channel {
private:
    int             _mode;
	std::string     _pass;
    std::list<int>  _full_list;
	std::list<int>  _oper;
	std::list<int>  _ban;

public:
    Channel(int fd_oper);
	Channel(int fd_oper, std::string pass);
	Channel();
    ~Channel();

    int set_mode();
    int add_client(int fd_key);
	int verif_pass(std::string pass);
    int add_client_ban();
    int remove_client();
};


#endif //FT_IRC3_CHANNEL_HPP
