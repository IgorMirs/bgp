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


