#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>
#ifndef NODE_H
#define NODE_H

class Node
{
	std::vector<uint32_t> output_value;
	uint32_t ID;
	bool commander;									//1 - commander
	uint32_t type;									//0 - loyal; 1 - traitor

public:
	std::vector<std::vector<int32_t>>  major_matrix;  //matrix for finding majority
	Node();											//constructor				
	uint32_t get_id();								//get the node number
	uint32_t get_type();							//get the node status (0 - loyal; 1 - traitor)
	void set_id(uint32_t value);					//set the node number
	bool is_commander();							//get if the node a commander (0 - not a commander; 1 - commander)
	void set_commander(bool commander);				//make the node commander. 
	std::vector <uint32_t> get_output();			//get output vector of the node
	void set_output(uint32_t value);				//put the value to the output vector of the node
	void set_type(uint32_t value);					//0 - loyal; 1 - traitor
	void print_node();								//print all characteristics of the node to the file
	void print_node_on_screen();					//print all characteristics of the node to the screen
	uint32_t major_matrix_majority(uint32_t row_number);  //count the majority of the particular row in majority matrix
	void default_value_major_matrix(uint32_t row_number);  //make all the values of the particular row equal to -1
	void resize_major_matrix(uint32_t number_of_rows, uint32_t number_of_columns);    //resize the number of columns and rows of majority matrix. The size should be equal to the number of nodes in the network
	void put_majority(uint32_t row_number, uint32_t column_number, uint32_t result);	//put the result on a particular place of the majority matrix
	~Node();
};
#endif // NODE_H


