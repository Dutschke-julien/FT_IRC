
class Client;

#ifndef CLIENT_HPP
# define CLIENT_HPP

# define KICK -1
# define NO_PASS 0
# define NO_NICK 1
# define NO_USERNAME 2
# define ALL_OK 3

# include "Serveur.hpp"

struct package
{
  int type;
  std::string cmd;
  std::string extra;	
};

class Client
{
	private:
	sockaddr_in _clientAddr;
	std::string _nickname;
	std::string _username;
	std::string _realname;	
	int 		_status;
    std::string _current_channel;
	std::list<std::string> _channel_invite;

	public:
	Client();
	~Client();

	int _vector_index;
	std::string get_realname(void);
	void set_realname(std::string nick);
	void set_clientAddr(sockaddr_in new_struct);
	sockaddr_in get_clientAddr(void);
	std::string get_nickname(void);
	void set_nickname(std::string nick);
	void set_username(std::string nick);
	std::string get_username(void);
	int get_status(void);
	void set_status(int status);
	struct package pack;
    void        set_current_channel(std::string channel);
    std::string get_current_channel();
	void	add_channel_invitation(std::string channel);
	int	find_and_remove_invitation(std::string channel);
};



#endif
