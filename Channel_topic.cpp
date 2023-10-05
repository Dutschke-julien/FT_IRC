//
// Created by Antoine Ho on 10/4/23.
//

#include "Serveur.hpp"

int Serveur::check_topic(std::string channel, int fd_key) {
    if (_listChannel.find(channel) == _listChannel.end()) {
        std::string error = ":yourserver 403 " + _mapClients[fd_key].get_nickname() + " #" + channel + " :No such channel\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return 1;
    }
    if (_listChannel[channel].find_client(fd_key) == 0){
        std::string error = ":yourserver 442 " + _mapClients[fd_key].get_nickname() + " #" + channel + " :You're not on that channel\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return 1;
    }
    return 0;
}

void Serveur::modify_topic(std::string channel, std::string topic, int fd_key) {
    /*
     * first, I need to check for the basic, if it exists and if the user is in it
     * need to check for the mode and the right for the topic's modification
     * then I should change the topic and use a loop to notify every client about the change
     */
    if (check_topic(channel, fd_key) == 1)
        return;
    if (_listChannel[channel].get_topic_restriction() == 1 && _listChannel[channel].find_oper(fd_key) == 0){
        std::string error = ":yourserver 482 "
                + _mapClients[fd_key].get_nickname()
                + " #" + channel
                + " :You're not channel operator\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    _listChannel[channel].set_topic(topic);
    std::list<int> user = _listChannel[channel].get_list_user();
    for (std::list<int>::iterator i = user.begin(); i != user.end() ; i++) {
        std::string reply = ":yourserver 332 "
                            + _mapClients[*i].get_nickname()
                            + " #" + channel + " :"
                            + _listChannel[channel].get_topic()
                            + "\r\n";
        send(fd_key, reply.c_str(), reply.length(), 0);
    }
}

void Serveur::send_topic(std::string channel, int fd_key) {
    if (channel.empty()) {
        std::string error = ":yourserver 461 " + _mapClients[fd_key].get_nickname() + " /topic :Not enough parameters\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    if (check_topic(channel, fd_key) == 1)
        return;
    if (_listChannel[channel].get_topic().empty()) {
        std::string error = ":yourserver 331 " + _mapClients[fd_key].get_nickname() + " #" + channel + " :No topic is set\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    std::string reply = ":yourserver 332 "
            + _mapClients[fd_key].get_nickname()
            + " #" + channel + " :"
            + _listChannel[channel].get_topic()
            + "\r\n";
    send(fd_key, reply.c_str(), reply.length(), 0);
    return;
}


void Serveur::cmd_topic(std::string cmd, int fd_key) {
    std::map<int, std::string> word;
    std::istringstream iss(cmd);
    std::string first;
    std::string second;
    std::getline(iss, first, ':');
    std::getline(iss, second);
    std::istringstream split(first);

    if (_mapClients[fd_key].get_status() == 1) {
        std::string erreur = ":server_name 464 * :Mot de passe incorrect. Veuillez vérifier votre mot de passe.\r\n";
        send(fd_key, erreur.c_str(), erreur.length(), 0);
        return;
    }

    cmd.erase(cmd.find_last_of('\n'), 1);
    cmd.erase(cmd.find_last_of('\r'), 1);
    for (int i = 0; split ; i++) {
        split >> word[i];
    }
    if (!(word[0].empty()))
        word[0].erase(word[0].find_first_of('#'));
    switch (word.size()) {
        case 1:
            if (second.empty()) {
                std::cout << "return the topic of the channel\n";
                send_topic(word[0], fd_key);
                return;
            } else {
                std::cout << "modify the topic of the channel\n";
                modify_topic(word[0], second, fd_key);
                return;
            }
        case 0:
            std::cout << "return the topic of the current channel\n";
            send_topic(_mapClients[fd_key].get_current_channel(), fd_key);
            return;
        default:
            std::cout << "return error\n";
            std::string error = ":yourserver 400 " + _mapClients[fd_key].get_nickname() + " TOPIC :Unexpected error from a /topic command\r\n";
            send(fd_key, error.c_str(), error.length(), 0);
            return;
    }
}