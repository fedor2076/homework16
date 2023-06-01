

#include <iostream>
#include <string>
#include <map>
#include <regex> //Regular Expression = регулярные выражения
using namespace std;
//входные параметры = текст вопроса =string user_text
//выходные данные =текст ответа
//функция выводит в cout ответ на данный текст
// тип void функция не возвращает никакого результата 
map <string, string> questions_answers = {
    //{фраза, ответ}
    {"Hello", "Hello to you human."},
    {"How (do you do|are you)", "I'm function within normal parametrs!"},
    {"Exit", "Never wanted talk to you in first place!"},
    {"What are you doing", "Answering stupid questions"},
    {"No pain", "No gain"},
    {"(What is|Whats) (your )?name", "My name is [bot: Luisa]" },

};


void bot_reply(string user_text) {
    //cout << "Can't undestand\n";
    //найти ответ перебором
    // цикл for(:)= выполнить действие для всех элементов (массива, map, ...)
    //element = переменная
    // auto определить тип автоматически
    for (auto element : questions_answers) {
       
        // {фраза, ответ} = (first, second)
        //"." = один любой символ 
        //".*" =любое количество символов
        //icase = ignore case = игнорировать регистр символов What what WhaT
        regex phrase(".*" + element.first + ".*", regex_constants::icase);
        if (regex_match(user_text, phrase)) { //if (element.first == user_text)
            cout <<"[bot]: " << element.second << "\n";
        }
    }
}

int main()
{
    //setlocale(LC_ALL, "rus");
    //Учим С++
    //нейм спейс std
    //cout = console out = текстовый вывод
    //cin = console IN = текстовый ввод
    // \n = new line = перевод строки
    cout << "Welcome to Skilbox Chat!!\n";

    //переменная question имеет тип string строковые данные
    string question, get_out ;
     get_out = "exit";
    
    while (question != "exit")



    {
        cout << "[user]: ";
        getline(cin, question);//читаем строчку из cin, помещаем в question
        bot_reply(question);
        regex phrase(".*" + get_out + ".*", regex_constants::icase);
        if (regex_match(question, phrase)) {
            question = get_out;
        };
      //  cout << question << "\n";

     //   regex_replace (question, ".*" + question + ".*", "exit", regex_constants::icase);
        
        }


    
        
    

    //if (question == "hello") {//проверяем что в переменной question содержится "hello"
    //    cout << "Hello, human!\n";//выводим ответ на экран
    //}
    //if (question == "bye") {
    //    cout << "Have a good day!\n";
    //}
    //Цикл While
    //Map
}


