#include"consolechat.h"
#include<iostream>
#include <windows.h>


bool ConsoleChat::�hatStarted() const
{
	return _�hatStarted;
}

void ConsoleChat::start()
{
	_�hatStarted = true;
}
int ConsoleChat::searchValue(const string& name, const std::map<std::string, std::string>& my_map)
{
	int i = 1;
	for (auto& element : my_map)
	{
		if (name == element.first)
					return i;
				i++;
	}
	return 0;
}
void ConsoleChat::startMenu() // ��������� ����, ������������ ��� �������
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << "\tWelcome to chat!\n\n" << "Choose an action to continue: \n\n";
	
	for (auto& element : main_menu)
	{		
		std::cout << element.first << " " << element.second << std::endl;
	}
	
	string action;


	std::cin >> action;

	switch (searchValue(action, main_menu))
	{
	case 1:
		SetConsoleTextAttribute(hConsole, 10);
		signUp(); // ������������������
		break;
	case 2:

		logIn(); // �����
		break;
	case 3:
		_�hatStarted = false; // ����� �� ����
		SetConsoleTextAttribute(hConsole, 15);
		break;
	default:
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Please check that the characters you are entering are correct.\n";
		SetConsoleTextAttribute(hConsole, 11);
		break;
	}
	
};

shared_ptr<User> ConsoleChat::getUserByLogin(const string login) const
{
	for (auto& user : _user) // ���������� vector ������������
	{
		if (login == user.getLogin()) // ���� ��������� ����� ������ � ������� ������ �� �������������
			return make_shared<User>(user); // ���������� ��������� �� ����� ������������
	}
	return nullptr;
}

shared_ptr<User>  ConsoleChat::getOnlineUser() const
{
	return _onlineUser;
}

void ConsoleChat::logIn() // ���� � ���
{
	string login;
	string password;

	std::cout << "Please enter a login: ";
	std::cin >> login;
	std::cout << "Enter password: ";
	std::cin >> password;

	_onlineUser = getUserByLogin(login); // ��������� �� ������ ������������ �������� ��������� �� ������������ � ��������� �������

	if (_onlineUser == nullptr || password != _onlineUser->getPassword()) // ���� ��� ������ ������������ ��� ��������� ������ �� ����� ������ ������ ������������
	{
		_onlineUser = nullptr; // ��������, ��� ������ ������������ ����������
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Invalid login or password, please try again\n";
		SetConsoleTextAttribute(hConsole, 11);
	}
}

void ConsoleChat::signUp() // �����������
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 10);
	string login, password, name;

	std::cout << "Please enter your login: ";
	std::cin >> login;
	std::cout << "Choose security password: ";
	std::cin >> password;
	std::cout << "Enter your name: ";
	std::cin >> name;

	try
	{
		if (getUserByLogin(login) || login == "all") // ���� ������������ � ����� ������� ���������� ��� ����� = all
		{
			throw LoginException("error: "); // ������������ ������
		};
	}

	catch (const LoginException& ex) // ����� ����������� ����������
	{
		SetConsoleTextAttribute(hConsole, 12);
		cerr << "\n\n (" << ex.what() << ") \n\n"; // �������� ����� what()
		SetConsoleTextAttribute(hConsole, 15);
		return;
	}

	User user = User(login, password, name); // ������� ������ ������������
	_user.push_back(user); // ��������� ������ ������������ � vector �������������
	_onlineUser = make_shared<User>(user); // ����� ������������ ���������� ������ �������������
	std::cout << "\nCongratulations! You have successfully registered!\n";
}

void ConsoleChat::openChat() const // �������� ���� � ������ �������������
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	string sender, recipient;

	std::cout << "-------users chatting online-----\n";
	std::cout << " login:  \n";

	for (auto& user : _user) // ���������� �������������
		std::cout << " " << user.getLogin() << "\n"; // ������� ������

	std::cout << "\n--------messages-----------------\n\n";

	for (auto& message : _message) // ���������� vector ���������
	{
		if (_onlineUser->getLogin() == message.getSender() || _onlineUser->getLogin() == message.getRecipient() || message.getRecipient() == "all") // ���� ����� ������ ������������ ����� ������ ����������� ��� ������ ���������� ��� ����� "all"
		{
			sender = (_onlineUser->getLogin() == message.getSender()) ? _onlineUser->getLogin() : getUserByLogin(message.getSender())->getName(); // ������������� �����������

			if (message.getRecipient() == "all") // ���� ��������� ������������� ��� ����
			{
				recipient = "all";
			}
			else // ���� ��������� ��� ����������� ������������
			{
				recipient = (_onlineUser->getLogin() == message.getRecipient()) ? _onlineUser->getLogin() : getUserByLogin(message.getRecipient())->getName(); // ������������� ����������
			}
			std::cout << "message from " << sender << ": to " << recipient << "\n";
			std::cout << " - " << message.getLetter() << " - \n";
		}
	}
	std::cout << "---------------------------------\n\n";

}

void ConsoleChat::sendMessage() // �������� ���������
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string recipient, text;
	std::cout << "to (login or all) - ";
	std::cin >> recipient; // ������� ����� ���������� ��� all
	std::cout << ">> ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!(recipient == "all" || getUserByLogin(recipient))) 
	{  // ���� ���������� �� ����� all ��� �� ������ ����� ������������

		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "error no such user  -- " << recipient << "\n";
		SetConsoleTextAttribute(hConsole, 11);
		return;
	}
	if (recipient == "all") // ���� ��������� ��� ����
		_message.push_back(Message(_onlineUser->getLogin(), "all", text)); // ������� ��������� ��� ����
	else
		_message.push_back(Message(_onlineUser->getLogin(), getUserByLogin(recipient)->getLogin(), text)); // ������� ��������� ��� ����������� ������������
	std::cout << "message send -  " << recipient << ":\n ";
}

void ConsoleChat::chatMenu() // ���� ����
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << "Welcome " << _onlineUser->getName() << "\n";
	while (_onlineUser) // ���� ���� ������ ������������
	{
		std::cout << "Choose an action: \n";
		for (auto& element : chat_menu)
		{
			std::cout << element.first << " " << element.second << std::endl;
		}
		string action;
		std::cin >> action;

		switch (searchValue(action, chat_menu))
		{
		case 1:
			openChat(); // ������� ���
			break;
		case 2:
			sendMessage(); // �������� ���������
			break;
		case 3:
			_onlineUser = nullptr; // �����
			break;
		default:
			std::cout << "Please try again\n";
			break;
		}
	}
}