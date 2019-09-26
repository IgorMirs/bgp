#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>
#include "Message.h"
#ifndef NODE_H
#define NODE_H

class Node
{
	Message output_value;
	std::vector<Message> input;
	uint32_t ID{ 0 };                                    //node id (number)
	bool commander{ false };                                    //1 - commander
	uint32_t type{ 0 };                                    //0 - loyal; 1 - traitor
	bool is_sending{ false };
	uint32_t node_decision{ 999 };                        //1 - node decides to attack; 0 - retreat

public:
	std::vector<std::vector<int32_t>>  major_matrix;  //matrix for finding majority

	//constructor
	Node(uint32_t id_value = 0, uint32_t output = 999, bool is_commander_value = false, uint32_t type_value = 0, bool is_sending_value = false, uint32_t node_decision = 999);

	uint32_t input_majority(std::vector<uint32_t> val_vec);

	//getters and setters
	uint32_t get_id() { return ID; }                                //get the node number
	uint32_t get_type() { return type; }                            //get the node status (0 - loyal; 1 - traitor)
	void set_id(uint32_t value) { ID = value; }                        //set the node number
	bool is_commander() { return commander; }                        //get if the node a commander (0 - not a commander; 1 - commander)
	bool get_is_sending() { return is_sending; }
	void set_commander(bool value) { commander = value; }            //make the node commander.
	void set_is_sending(bool value) { is_sending = value; }
	uint32_t get_output_mes() { return output_value.get_mes(); }            //get the sending message from the output
	Message get_output_message() { return output_value.get_message(); }            //get the whole message with signs
	Message get_last_input() { return input[input.size() - 1]; }
	void set_output(Message value) {			//put the value to the output vector of the node
		output_value.set_mes(value);
	}  
	void set_input(Message value) {
		input.push_back(value); 
	}

	
	void receive_mes(Message mes);  //the node should receive only new messages (which are not in his input vector)
	void set_type(uint32_t value) { type = value; }                //0 - loyal; 1 - traitor
	bool is_input_empty() { if (input.empty()) return 1; else return 0; }
	void set_default_input(Message mes) { input.assign(1, mes); }
	void input_clear() { input.clear(); }
	void output_clear() { output_value.clear_mes(); }
	uint32_t get_node_decision() { return node_decision; }
	void set_node_decision(uint32_t value) { node_decision = value; }

	~Node();
};
#endif // NODE_H


