//
// Created by Antoine Ho on 9/26/23.
//

#include "Serveur.hpp"

void Serveur::cmd_join(std::string string, int fd_key) {

    for (std::string::iterator it = string.begin(); *it == ' ' ; it++) {
        string.erase(it);
    }
    string.erase(string.find_last_of('\n'));
    string.erase(string.find_last_of('\r'));
    if (string == ":") {
        error = ":yourserver 001 aho :OK\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    if (string.empty()) {
        error = ":yourserver 461 aho : not enough parameters\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    std::string::iterator  it = string.begin();
    std::vector<std::string> list_channel;
    std::vector<std::string> list_key;
    std::string word;
    int index = 0;
    if (*it != '#' && ++it != string.end()) {
        std::cout << "send : no channel found\n";
        error = ":yourserver 403 aho : bad channel mask\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    } else {
        for (std::string::iterator it_loop = string.begin(); it_loop != string.end() ; it_loop++) {
            while (*it_loop == ' ') {
                it_loop++;
            }
            while (*it_loop == '#' || *it_loop == ',') {
                it_loop++;
                index = 1;
            }
            while (*it_loop != ' ' && *it_loop != '#' && *it_loop != ',' && it_loop != string.end()) {
                word += *it_loop;
                it_loop++;
            }
            if (index == 1 && !(word.empty()))
                list_channel.push_back(word);
            else if (!(word.empty()))
                list_key.push_back(word);
        }

    }
}