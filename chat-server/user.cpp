#include "user.hpp"

User::User(const std::string& my_name, const std::pair<std::string, std::string>& user_data)
: _my_name(my_name), _user_data(user_data)
{
}

User& User::operator= (const User& user)
{
	if (&user == this)
		return *this;
	this->_my_name = user._my_name;
	this->_user_data.first = user._user_data.first;
	this->_user_data.second = user._user_data.second;
	this->_message_count = user._message_count;
	return *this;
}

// Registration
User User::sign_up(const char *name, const char *login, const char *password)
{
	std::string regName = name;
	std::string regLogin = login;
	std::string regPassword = password;
	std::pair<std::string, std::string> regData = make_pair(regLogin, regPassword);

	User newUser(regName, regData);
	return newUser;
}

// Authorization
User User::sign_in(User users[], int usersCount, const char *login, const char *password)
{
	std::string authLogin = login;
	std::string authPassword = password;
	std::pair<std::string, std::string> authData = make_pair(authLogin, authPassword);
	for(int i = 0; i <= usersCount; i++)
	{
		if (users[i]._user_data.first == authData.first && users[i]._user_data.second == authData.second)
		{
			this->_auth = 1;
			return users[i];
		}
	}
	User userNone;
	return userNone;

}

// Send a message to someone
void User::sendAMessage(User users[], int usersCount, const char *charUsername, const char *charMessage)
{
	std::string username = charUsername;
	std::string message = charMessage;
	for (int i = 0; i <= usersCount; i++)
	{
		if (i == usersCount)
		{
			std::cout << "User is not found" << std::endl;
		}
		if (users[i]._my_name == username)
		{
			if (users[i]._message_count != 5)
			{
				users[i]._my_messages[users[i]._message_count] = message;
				users[i]._message_count++;
				std::cout << "Message sent!" << std::endl;
				return;
			}
			else 
			{
				users[i]._message_count = 0;
				users[i]._my_messages[users[i]._message_count] = message;
				users[i]._message_count++;
				std::cout << "Message sent!" << std::endl;
				return;
			}
		}
	}
	std::cout << "Something went wrong while sending the message" << std::endl;
	return;
}

// Send a message to everyone
void User::SendAMessageToEveryone(User users[], int usersCount, const char *charMessage)
{
	std::string message = charMessage;
	for (int i = 0; i < usersCount; i++)
	{
		if (users[i]._message_count != 5)
		{
			users[i]._my_messages[users[i]._message_count] = message;
			users[i]._message_count++;
		}
		else 
		{
			users[i]._message_count = 0;
			users[i]._my_messages[users[i]._message_count] = message;
			users[i]._message_count++;
		}
	}
}