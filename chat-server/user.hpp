#include <iostream>
#include <array>
#include <utility>
class User
{
public:
	User(const std::string& my_name, const std::pair<std::string, std::string>& user_data);
	User() = default;
	~User() = default;

	User& operator= (const User& user);

	// Registration
	User sign_up(const char *name, const char *login, const char *password);

	// Authorization
	User sign_in(User users[], int usersCount, const char *login, const char *password);

	// Send a message to someone
	void sendAMessage(User users[], int usersCount, const char *username, const char *message);

	// Send a message to everyone
	void SendAMessageToEveryone(User users[], int usersCount, const char *charMessage);

	bool _auth = false;
	std::string _my_name;
	std::array<std::string, 5> _my_messages;
	int _message_count = 0;
	// Username and password field
	std::pair<std::string, std::string> _user_data;
};

