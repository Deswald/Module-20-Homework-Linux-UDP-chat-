#include "chat.hpp"
 
char buffer[MESSAGE_BUFFER];
char message[MESSAGE_BUFFER];
int socket_descriptor, message_size;
struct sockaddr_in serveraddress;

void chat()
{
    // Укажем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта для соединения с сервером
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Создадим сокет 
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    // Установим соединение с сервером
    if(connect(socket_descriptor, (struct sockaddr*) &serveraddress, sizeof(serveraddress)) < 0) {
        std::cout << std::endl << " Something went wrong Connection Failed" << std::endl;
        exit(1);
    }
	else {
		std::cout << "Server connection established!" << std::endl;
	}

	int options;
	char signUp[MESSAGE_BUFFER] = "signUp";
	char signIn[MESSAGE_BUFFER] = "signIn";
	char endSession[MESSAGE_BUFFER] = "endSession";
	char usersFull[MESSAGE_BUFFER] = "usersFull";
	char successfulUserAuthorization[MESSAGE_BUFFER] = "successfulUserAuthorization";
	char logOut[MESSAGE_BUFFER] = "logOut";
	char messageSomeOne[MESSAGE_BUFFER] = "messageSomeOne";
	char messageAll[MESSAGE_BUFFER] = "messageAll";
	char checkingUserMessages[MESSAGE_BUFFER] = "checkingUserMessages";

	char myName[MESSAGE_BUFFER];
	char myLogin[MESSAGE_BUFFER];
	char myPassword[MESSAGE_BUFFER];

	bool auth = false;
	bool open = true;
	while (open)
	{
		std::cout << "Welcome to the chat, select an operation: " << std::endl;
		std::cout << "1 - to sign up" << std::endl;
		std::cout << "2 - to sign in" << std::endl;
		std::cout << "0 - to quit" << std::endl;


		std::cin >> options;

		switch (options)
		{
		case 1:
				sendto(socket_descriptor, signUp, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
				std::cout << "Sign Up..." << std::endl;

				message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
				buffer[message_size] = '\0';
				if(strcmp(buffer, usersFull) == 0)
				{
					std::cout << "Maximum number of users reached" << std::endl;
					break;
				}

				std::cout << "Enter your name" << std::endl;
				std::cin >> myName;
				sendto(socket_descriptor, myName, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

				std::cout << "Enter login" << std::endl;
				std::cin >> myLogin;
				sendto(socket_descriptor, myLogin, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

				std::cout << "Enter password" << std::endl;
				std::cin >> myPassword;
				sendto(socket_descriptor, myPassword, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

				break;
		case 2:
				sendto(socket_descriptor, signIn, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
				std::cout << "Sign In..." << std::endl;

				std::cout << "Enter login" << std::endl;
				std::cin >> myLogin;
				sendto(socket_descriptor, myLogin, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

				std::cout << "Enter password" << std::endl;
				std::cin >> myPassword;
				sendto(socket_descriptor, myPassword, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

				message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
				buffer[message_size] = '\0';
				if(strcmp(buffer, successfulUserAuthorization) == 0)
				{
					std::cout << "Successful authorization!" << std::endl;
					auth = true;
				}
				else
				{
				std::cout << "Incorrect login or password" << std::endl;
				break;
				}

				while(auth)
				{
				std::cout << "Hello, select an operation: " << std::endl;
				std::cout << "1 - to send a message to someone" << std::endl;
				std::cout << "2 - to send a message to everyone" << std::endl;
				std::cout << "3 - check messages" << std::endl;
				std::cout << "0 - to log out" << std::endl;

				std::cin >> options;
				
				switch (options)
				{
				case 1:
					sendto(socket_descriptor, messageSomeOne, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

					std::cout << "Whom to send a message to? Enter a username:" << std::endl;
					std::cin >> myName;
					sendto(socket_descriptor, myName, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
					
					std::cout << "Write a message:" << std::endl;
					std::cin >> message;
					sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

					std::cout << "Message sent!" << std::endl;

					break;
				case 2:
					sendto(socket_descriptor, messageAll, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));

					std::cout << "Write a message:" << std::endl;
					std::cin >> message;
					sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
					std::cout << "Message sent to all users!" << std::endl;
					break;
				case 3:
					std::cout << "Checking messages.." << std::endl;
					sendto(socket_descriptor, checkingUserMessages, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
					for(int i = 0; i < 5; i++)
					{
						message_size = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
						buffer[message_size] = '\0';
						std::cout << "Message #" << i+1 << ": " << buffer << std::endl;
					}
					break;
				case 0:
					std::cout << "Logging out.." << std::endl;
					auth = 0;
					sendto(socket_descriptor, logOut, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
					break;
				default:
					break;
				}

			}
			break;
		case 0:
			open = false;
			sendto(socket_descriptor, endSession, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
        	std::cout << "Client work is done.!" << std::endl;
        	close(socket_descriptor);
        	exit(0);
			break;
		default:
			break;
		}
	}
}