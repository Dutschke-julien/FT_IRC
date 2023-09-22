//
// Created by Antoine Ho on 9/19/23.
//

#ifndef FT_IRC3_CHANNEL_HPP
#define FT_IRC3_CHANNEL_HPP

#include <list>
#include <string>

class Channel {
private:
    int mode;

    std::list<int> _full_list;
	std::list<int> _oper;
	std::list<int> _regular_client;
	std::list<int> _ban;

public:
    Channel(int fd_oper);
	Channel();
    ~Channel();

    void set_mode();
    void add_client(int fd_key);
    void add_client_ban();
    void remove_client();
};


#endif //FT_IRC3_CHANNEL_HPP
