
class Client;

#ifndef CLIENT_HPP
# define CLIENT_HPP

# define NO_PWD -2
# define NO_NKN -1
# define ALL_OK 0

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

	public:
	Client();
	~Client();

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
};



#endif