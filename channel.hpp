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

    std::list<std::string> _full_list;
    std::list<std::string> _oper;
    std::list<std::string> _regular_client;
    std::list<std::string> _ban;

public:
    explicit Channel(std::string name_oper);
    ~Channel();

    void set_mode();
    void add_client();
    void add_client_ban();
    void remove_client();
};


#endif //FT_IRC3_CHANNEL_HPP
