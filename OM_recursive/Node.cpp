#include "Node.h"
#include <cstdint>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <map>

using namespace std;
ofstream out_node;

Node::Node(uint32_t id_value, uint32_t output, bool is_commander_value, uint32_t type_value, bool is_sending_value, uint32_t node_decision_value)
{
	set_id(id_value);
	output_value.create_mes(output);
	set_commander(is_commander_value);
	set_type(type_value);
	set_is_sending(is_sending_value);
	set_node_decision(node_decision_value);
}

void Node::receive_mes(Message mes) {
	//if the node has no messages in his input vector - he add the message
	if (this->input.empty())
		this->set_input(mes);
	else
	{
		bool uniq_mes = true;
		//check if received message is already in the input vector
		//if yes - do not receive such message
		for (uint32_t i = 0; i < this->input.size(); i++) {
			if (mes.get_mes() == input.at(i).get_mes()) {
				uniq_mes = false;
				break;
			}
		}
		if (uniq_mes)
			this->set_input(mes);
	}
}

uint32_t Node::input_majority(vector<uint32_t> val_vec)
{
	//if deleted all the values
	if (val_vec.empty())
		return 999;

	map<int, int> hash;
	//count how many times each value is in the vector and put it to the map
	for (int i = 0; i < val_vec.size(); i++) {
		auto it = hash.find(val_vec[i]); //if not found the value return iterator to the end
		if (it != hash.end())
			it->second++;
		hash.emplace(val_vec[i], 1);
	}

	//searching for the most often value
	int max = INT16_MIN, key = 0;
	for (auto it = hash.cbegin(); it != hash.cend(); ++it) {
		if (it->second > max) {
			max = it->second;
			key = it->first;
		}
	}

	//max should be unique, if it is not unique, so there is no majority and we return 0
	for (auto it = hash.cbegin(); it != hash.cend(); ++it) {
		if (it->second == max && it->first != key)
			return 0;
	}
	return key;
}


Node::~Node() {};


