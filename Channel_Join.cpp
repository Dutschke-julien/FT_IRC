//
// Created by Antoine Ho on 9/26/23.
//

# include<iostream>
# include<iterator>
# include <stdio.h>
# include <iostream>
# include <stdlib.h>
# include <string>
# include <map>
# include <vector>
# include <unistd.h>
# include <cstring>
# include <arpa/inet.h>
# include <netinet/in.h>
# include "client.hpp"
# include "channel.hpp"

void cmd_join(std::string string, int fd_key) {

    for (std::string::iterator it = string.begin(); *it == ' ' ; it++) {
        string.erase(it);
    }
    string.erase(string.find_last_of('\n'), 1);
    string.erase(string.find_last_of('\r'), 1);

    if (string == ":") {
        std::string error = ":yourserver 001 aho :OK\r\n";
//        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    if (string.empty()) {
        std::string error = ":yourserver 461 aho : not enough parameters\r\n";
//        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    std::string::iterator  it = string.begin();
    std::vector<std::string> list_channel;
    std::vector<std::string> list_key;
    std::string word;
    int index = 0;
    if (*it != '#' && ++it != string.end()) {
        std::cout << "send : no channel found" << *it << std::endl;
        std::string error = ":yourserver 403 aho : bad channel mask\r\n";
//        send(fd_key, error.c_str(), error.length(), 0);
        return;
    } else {

        std::string::iterator it_loop = string.begin();
        while (it_loop != string.end()){
            while (it_loop != string.end() && *it_loop == ' ') {
                it_loop++;
            }
            while (it_loop != string.end() && (*it_loop == '#' || *it_loop == ',')) {
                if (*it_loop == '#')
                    index = 1;
                it_loop++;
            }
            while ( it_loop != string.end() && *it_loop != ' ' && *it_loop != ',') {
                word += *it_loop;
                it_loop++;
            }
            if (index == 1 && !(word.empty()))
                list_channel.push_back(word);
            else if (!(word.empty()))
                list_key.push_back(word);
            index = 0;
            word.erase(word.begin(), word.end());
            if (it_loop != string.end())
                it_loop++;
        }
    }

    for (std::vector<std::string>::iterator test = list_channel.begin(); test != list_channel.end() ; test++) {
        std::cout << "[" << *test << "] ";
    }
    std::cout << std::endl;
    for (std::vector<std::string>::iterator test = list_key.begin(); test != list_key.end() ; test++) {
        std::cout << "{" << *test << "} ";
    }
    std::cout << std::endl;
}

int main() {
    std::string test = "#channel1#channel2\r\n";
    std::string test2 = "#channel1 #channel2\r\n";
    std::string test3 = "#channel1 #channel2 \r\n";
    std::string test4 = "#channel1,#channel2\r\n";
    std::string test5 = "#channel1 , #channel2\r\n";
    std::string test6 = "#channel1 key1\r\n";
    std::string test7 = "#channel1 #channel2 key1key2\r\n";
    std::string test8 = "#channel1 #channel2 key1 key2\r\n";
    std::string test9 = "#channel1 #channel2 key1     \r\n";
    std::string test10 = "   #channel1 key1 key2\r\n";

    std::cout << "debut test\n";
    cmd_join(test, 0);
    std::cout << "\n\ndebut test2\n";
    cmd_join(test2, 0);
    std::cout << "\n\ndebut test3\n";
    cmd_join(test3, 0);
    std::cout << "\n\ndebut test4\n";
    cmd_join(test4, 0);
    std::cout << "\n\ndebut test5\n";
    cmd_join(test5, 0);
    std::cout << "\n\ndebut test6\n";
    cmd_join(test6, 0);
    std::cout << "\n\ndebut test7\n";
    cmd_join(test7, 0);
    std::cout << "\n\ndebut test8\n";
    cmd_join(test8, 0);
    std::cout << "\n\ndebut test9\n";
    cmd_join(test9, 0);
    std::cout << "\n\ndebut test10\n";
    cmd_join(test10, 0);
}
