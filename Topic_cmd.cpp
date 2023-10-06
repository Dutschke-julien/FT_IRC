#include "Serveur.hpp"

int Serveur::check_topic(std::string channel, int fd_key) {
    if (_listChannel.find(channel) == _listChannel.end()) {
        std::string error = ":42Mulhouse 403 " + _mapClients[fd_key].get_nickname() + " #" + channel + " :No such channel\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return 1;
    }
    if (_listChannel[channel].find_client(fd_key) == 0){
        std::string error = ":42Mulhouse 442 " + _mapClients[fd_key].get_nickname() + " #" + channel + " :You're not on that channel\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return 1;
    }
    return 0;
}

void Serveur::modify_topic(std::string channel, std::string topic, int fd_key) {
    if (check_topic(channel, fd_key) == 1)
        return;
    if (_listChannel[channel].get_topic_restriction() == -1 && _listChannel[channel].find_oper(fd_key) == 0){
        std::string error = ":42Mulhouse 482 "
                + _mapClients[fd_key].get_nickname()
                + " #" + channel
                + " :You're not channel operator\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    _listChannel[channel].set_topic(topic);
    std::list<int> user = _listChannel[channel].get_list_user();
    for (std::list<int>::iterator i = user.begin(); i != user.end() ; i++) {
        std::string reply = ":42Mulhouse 332 "
                            + _mapClients[*i].get_nickname()
                            + " #" + channel + " :"
                            + _listChannel[channel].get_topic()
                            + "\r\n";
        send(*i, reply.c_str(), reply.length(), 0);
    }
}

void Serveur::send_topic(std::string channel, int fd_key) {
    if (channel.empty()) {
        std::string error = ":42Mulhouse 461 " + _mapClients[fd_key].get_nickname() + " /topic :Not enough parameters\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    if (check_topic(channel, fd_key) == 1)
        return;
    if (_listChannel[channel].get_topic().empty()) {
        std::string error = ":42Mulhouse 331 " + _mapClients[fd_key].get_nickname() + " #" + channel + " :No topic is set\r\n";
        send(fd_key, error.c_str(), error.length(), 0);
        return;
    }
    std::string reply = ":42Mulhouse 332 "
            + _mapClients[fd_key].get_nickname()
            + " #" + channel + " :"
            + _listChannel[channel].get_topic()
            + "\r\n";
    send(fd_key, reply.c_str(), reply.length(), 0);
    return;
}


void Serveur::cmd_topic(std::string cmd, int fd_key) {
    cmd.erase(cmd.find_last_of('\n'), 1);
    cmd.erase(cmd.find_last_of('\r'), 1);
    cmd.erase(cmd.find_first_of('#'), 1);
    std::string word;
    std::istringstream iss(cmd);
    std::string first;
    std::string second;
    std::getline(iss, first, ':');
    std::getline(iss, second);
    std::istringstream split(first);

    if (_mapClients[fd_key].get_status() == 1) {
        std::string erreur = ":42Mulhouse 464 * :Mot de passe incorrect. Veuillez vérifier votre mot de passe.\r\n";
        send(fd_key, erreur.c_str(), erreur.length(), 0);
        return;
    }

    split >> word;
    if (word.empty()) {
        std::cout << "return the topic of the current channel\n";
        send_topic(_mapClients[fd_key].get_current_channel(), fd_key);
        return;
    } else {
        if (second.empty()) {
            std::cout << "return the topic of the channel\n";
            send_topic(word, fd_key);
            return;
        } else if (second == ":"){
            std::cout << "clear the topic of the channel\n";
            modify_topic(word, "", fd_key);
        } else {
            std::cout << "modify the topic of the channel\n";
            modify_topic(word, second, fd_key);
            return;
        }
    }
}