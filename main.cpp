//данные User и Message реализованы в <vector>
//стартовое меню и меню чата реализованы в <map>
//цветовая схема сделана для ОС Windows
#include"consolechat.h"
#include<iostream>

auto main() -> int
{
	ConsoleChat ConsoleChat;

	ConsoleChat.start(); // Запуск чата

	while (ConsoleChat.сhatStarted()) // Пока чат запущен
	{
		ConsoleChat.startMenu(); // Отображать стартовое меню

		while (ConsoleChat.getOnlineUser()) // Пока в системе есть пользователь
		{
			ConsoleChat.chatMenu(); // Отображать меню чата
		}
	}

	return 0;

}