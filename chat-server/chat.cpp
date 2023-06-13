#include "chat.hpp"
#include "user.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MESSAGE_BUFFER 4096

char buffer[MESSAGE_BUFFER]; 
char message[MESSAGE_BUFFER];
int socket_file_descriptor, message_size;
socklen_t length;
const char *endSession_string = "endSession";
const char *signUp_string = "signUp";
const char *signIn_string = "signIn";
const char *checkingUserMessages_string = "checkingUserMessages";
const char *logOut_string = "logOut";
const char *messageSomeOne_string = "messageSomeOne";
const char *messageAll_string = "messageAll";
struct sockaddr_in serveraddress, client;
char bufferName[MESSAGE_BUFFER];
char bufferLogin[MESSAGE_BUFFER]; 
char bufferPassword[MESSAGE_BUFFER]; 
char bufferAddressee[MESSAGE_BUFFER];
char bufferReceivedMessage[MESSAGE_BUFFER];

void chat()
{
    // Создадим UDP сокет 
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим порт для соединения
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет 
    bind(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));

	std::cout << "The server has created a socket!" << std::endl;

	const int usersCount = 3;
	User users[usersCount];
	User userRegOrAuth;
	static int index = 0;
	char usersFull[MESSAGE_BUFFER] = "usersFull";
	char usersNotFull[MESSAGE_BUFFER] = "usersNotFull";
	char successfulUserAuthorization[MESSAGE_BUFFER] = "successfulUserAuthorization";
	char failedUserAuthorization[MESSAGE_BUFFER] = "failedUserAuthorization";

	while (1)
	{
		length = sizeof(client);
		std::cout << "The server is waiting for a message.." << std::endl;
        message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
        buffer[message_size] = '\0';
		if(strcmp(buffer, endSession_string) == 0)
		{
			std::cout << "Server is Quitting" << std::endl;
            close(socket_file_descriptor);
            exit(0);
        }
		if(strcmp(buffer, signUp_string) == 0)
		{
			if(index < usersCount)
			{
				sendto(socket_file_descriptor, usersNotFull, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				std::cout << "The server creates a new user.." << std::endl;

				message_size = recvfrom(socket_file_descriptor, bufferName, sizeof(bufferName), 0, (struct sockaddr*)&client, &length);
        		bufferName[message_size] = '\0';
				std::cout << "Message Received from Client: " << bufferName << std::endl;

				message_size = recvfrom(socket_file_descriptor, bufferLogin, sizeof(bufferLogin), 0, (struct sockaddr*)&client, &length);
        		bufferLogin[message_size] = '\0';
				std::cout << "Message Received from Client: " << bufferLogin << std::endl;

				message_size = recvfrom(socket_file_descriptor, bufferPassword, sizeof(bufferPassword), 0, (struct sockaddr*)&client, &length);
        		bufferPassword[message_size] = '\0';
				std::cout << "Message Received from Client: " << bufferPassword << std::endl;
			
			
				users[index] = userRegOrAuth.sign_up(bufferName, bufferLogin, bufferPassword);
				std::cout << "The server has created a new user!: " << users[index]._my_name << std::endl;
				index++;
			}
			else 
			{
				std::cout << "Maximum number of users reached: " << usersCount << std::endl;
				sendto(socket_file_descriptor, usersFull, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
			}
        }
		if(strcmp(buffer, signIn_string) == 0) 
		{
			std::cout << "The server is authenticating.." << std::endl;

			message_size = recvfrom(socket_file_descriptor, bufferLogin, sizeof(bufferLogin), 0, (struct sockaddr*)&client, &length);
        	bufferLogin[message_size] = '\0';
			std::cout << "Message Received from Client: " << bufferLogin << std::endl;

			message_size = recvfrom(socket_file_descriptor, bufferPassword, sizeof(bufferPassword), 0, (struct sockaddr*)&client, &length);
        	bufferPassword[message_size] = '\0';
			std::cout << "Message Received from Client: " << bufferPassword << std::endl;

			userRegOrAuth = userRegOrAuth.sign_in(users, usersCount, bufferLogin, bufferPassword);
			if (userRegOrAuth._auth)
			{
				std::cout << "Successful authorization!" << std::endl;
				sendto(socket_file_descriptor, successfulUserAuthorization, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));

				while (userRegOrAuth._auth)
				{
					std::cout << "Waiting for an authorized user to select an action.." << std::endl;
					message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &length);
					buffer[message_size] = '\0';

					if(strcmp(buffer, logOut_string) == 0) 
					{
						std::cout << "Logging out.." << std::endl;
						userRegOrAuth._auth = 0;
					}

					if(strcmp(buffer, messageSomeOne_string) == 0)
					{
						std::cout << "Waiting for a message.." << std::endl;
						
						message_size = recvfrom(socket_file_descriptor, bufferAddressee, sizeof(bufferAddressee), 0, (struct sockaddr*)&client, &length);
						bufferAddressee[message_size] = '\0';
						std::cout << "Message Received from Client: " << bufferAddressee << std::endl;

						message_size = recvfrom(socket_file_descriptor, bufferReceivedMessage, sizeof(bufferReceivedMessage), 0, (struct sockaddr*)&client, &length);
						bufferReceivedMessage[message_size] = '\0';
						std::cout << "Message Received from Client: " << bufferReceivedMessage << std::endl;

						userRegOrAuth.sendAMessage(users, usersCount, bufferAddressee, bufferReceivedMessage);
					}
					if(strcmp(buffer, messageAll_string) == 0)
					{
						std::cout << "Waiting for a message to be sent to all users.." << std::endl;
						
						message_size = recvfrom(socket_file_descriptor, bufferReceivedMessage, sizeof(bufferReceivedMessage), 0, (struct sockaddr*)&client, &length);
						bufferReceivedMessage[message_size] = '\0';
						std::cout << "Message Received from Client: " << bufferReceivedMessage << std::endl;

						userRegOrAuth.SendAMessageToEveryone(users, usersCount, bufferReceivedMessage);
					}
					if(strcmp(buffer, checkingUserMessages_string) == 0)
					{
						for (int i = 0; i < usersCount; i++)
						{
							if (users[i]._user_data.first == userRegOrAuth._user_data.first)
							{
							char* charMessage;
							std::string stringMessage;
							for (int k = 0; k < 5; k++)
							{
								std::cout << "Message #" << k+1 << ": " << users[i]._my_messages[k] << std::endl;
								stringMessage = users[i]._my_messages[k];
								charMessage = &stringMessage[0];
								sendto(socket_file_descriptor, charMessage, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
							}
							break;
							}
						}
					}
				}
			}
			else
			{
				std::cout << "Incorrect login or password" << std::endl;
				sendto(socket_file_descriptor, failedUserAuthorization, MESSAGE_BUFFER, 0, (struct sockaddr*)&client, sizeof(client));
				
			}
		}
	}	
	// закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
}