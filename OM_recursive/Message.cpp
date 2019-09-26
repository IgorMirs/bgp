#include "Message.h"
#include <iostream>

void Message::create_mes(uint32_t m)
{
	if (!mes.empty())
		mes.clear();
	mes.push_back(m);
}

void Message::sign_mes(uint32_t sign)
{
	mes.push_back(sign);
}

void Message::print_mes()
{
	for (uint32_t i = 0; i < mes.size(); i++)
		std::cout << mes.at(i) << ' '; 
	std::cout << std::endl;
}

void Message::clear_mes()
{
	mes.clear();
}

uint32_t Message::get_mes()
{
	return mes.at(0);
}


void Message::set_mes(Message message)
{
	mes.clear();
	mes.insert(std::begin(mes), std::begin(message.mes), std::end(message.mes));
}

Message Message::get_message()
{
	return *this;
}
