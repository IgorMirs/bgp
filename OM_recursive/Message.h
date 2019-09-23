#include <cstdint>
#include <vector>
#ifndef MESSAGE_H
#define MESSAGE_H

class Message
{
	std::vector<uint32_t> mes;
public:
	void create_mes(uint32_t m);
	void sign_mes(uint32_t sign);
	void print_mes();
	void clear_mes();
	uint32_t get_mes();
};

#endif // MESSAGE_H