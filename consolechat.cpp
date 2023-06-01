#include"consolechat.h"
#include<iostream>
#include <windows.h>


bool ConsoleChat::сhatStarted() const
{
	return _сhatStarted;
}

void ConsoleChat::start()
{
	_сhatStarted = true;
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
void ConsoleChat::startMenu() // Стартовое меню, отображается при запуске
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
		signUp(); // Зарегистрироваться
		break;
	case 2:

		logIn(); // Войти
		break;
	case 3:
		_сhatStarted = false; // Выход из чата
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
	for (auto& user : _user) // Перебираем vector пользователй
	{
		if (login == user.getLogin()) // Если введенный логин совпал с логином одного из пользователей
			return make_shared<User>(user); // Возвращаем указатель на этого пользователя
	}
	return nullptr;
}

shared_ptr<User>  ConsoleChat::getOnlineUser() const
{
	return _onlineUser;
}

void ConsoleChat::logIn() // Вход в чат
{
	string login;
	string password;

	std::cout << "Please enter a login: ";
	std::cin >> login;
	std::cout << "Enter password: ";
	std::cin >> password;

	_onlineUser = getUserByLogin(login); // Указатель на онлайн пользователя получает указатель на пользователя с указанным логином

	if (_onlineUser == nullptr || password != _onlineUser->getPassword()) // Если нет онлайн пользователя или указанный пароль не равен паролю онлайн пользователя
	{
		_onlineUser = nullptr; // Сообщаем, что онлайн пользователь отсутсвует
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Invalid login or password, please try again\n";
		SetConsoleTextAttribute(hConsole, 11);
	}
}

void ConsoleChat::signUp() // Регистрация
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
		if (getUserByLogin(login) || login == "all") // Если пользователь с таким логином существует или логин = all
		{
			throw LoginException("error: "); // Уникальность логина
		};
	}

	catch (const LoginException& ex) // Ловим выброшенное исключение
	{
		SetConsoleTextAttribute(hConsole, 12);
		cerr << "\n\n (" << ex.what() << ") \n\n"; // Вызываем метод what()
		SetConsoleTextAttribute(hConsole, 15);
		return;
	}

	User user = User(login, password, name); // Создаем нового пользователя
	_user.push_back(user); // Добавляем нового пользователя в vector пользователей
	_onlineUser = make_shared<User>(user); // Новый пользователь становится онлайн пользователем
	std::cout << "\nCongratulations! You have successfully registered!\n";
}

void ConsoleChat::openChat() const // Просмотр чата и списка пользователей
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	string sender, recipient;

	std::cout << "-------users chatting online-----\n";
	std::cout << " login:  \n";

	for (auto& user : _user) // Перебираем пользователей
		std::cout << " " << user.getLogin() << "\n"; // Выводим логиня

	std::cout << "\n--------messages-----------------\n\n";

	for (auto& message : _message) // Перебираем vector сообщений
	{
		if (_onlineUser->getLogin() == message.getSender() || _onlineUser->getLogin() == message.getRecipient() || message.getRecipient() == "all") // Если логин онлайн пользователя равен логину отправителя или логину получателя или равен "all"
		{
			sender = (_onlineUser->getLogin() == message.getSender()) ? _onlineUser->getLogin() : getUserByLogin(message.getSender())->getName(); // Устанавливаем отправителя

			if (message.getRecipient() == "all") // Если сообщение предназначено для всех
			{
				recipient = "all";
			}
			else // Если сообщение для конкретного пользователя
			{
				recipient = (_onlineUser->getLogin() == message.getRecipient()) ? _onlineUser->getLogin() : getUserByLogin(message.getRecipient())->getName(); // Устанавливаем получателя
			}
			std::cout << "message from " << sender << ": to " << recipient << "\n";
			std::cout << " - " << message.getLetter() << " - \n";
		}
	}
	std::cout << "---------------------------------\n\n";

}

void ConsoleChat::sendMessage() // Написать сообщение
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string recipient, text;
	std::cout << "to (login or all) - ";
	std::cin >> recipient; // Указать логин получателя или all
	std::cout << ">> ";
	std::cin.ignore();
	getline(std::cin, text);

	if (!(recipient == "all" || getUserByLogin(recipient))) 
	{  // Если получатель не равен all или не найден логин пользователя

		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "error no such user  -- " << recipient << "\n";
		SetConsoleTextAttribute(hConsole, 11);
		return;
	}
	if (recipient == "all") // Если сообщение для всех
		_message.push_back(Message(_onlineUser->getLogin(), "all", text)); // Создаем сообщение для всех
	else
		_message.push_back(Message(_onlineUser->getLogin(), getUserByLogin(recipient)->getLogin(), text)); // Создаем сообщение для конкретного пользователя
	std::cout << "message send -  " << recipient << ":\n ";
}

void ConsoleChat::chatMenu() // Меню чата
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << "Welcome " << _onlineUser->getName() << "\n";
	while (_onlineUser) // Пока есть онлайн пользователь
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
			openChat(); // Открыть чат
			break;
		case 2:
			sendMessage(); // Написать сообщение
			break;
		case 3:
			_onlineUser = nullptr; // Выход
			break;
		default:
			std::cout << "Please try again\n";
			break;
		}
	}
}