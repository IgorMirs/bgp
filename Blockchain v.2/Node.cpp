#include "Node.h"
#include <cstdint>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <map>

using namespace std;
ofstream out_node;

Node::Node()
{
	ID = 0;
	output_value.push_back(999);
	commander = false;
	type = 0;
	is_sending = false;
}

uint32_t Node::get_id()
{
	return ID;
}

uint32_t Node::get_type()
{
	return type;
}

void Node::set_id(uint32_t value)
{
	ID = value;
}

bool Node::is_commander()
{
	return commander;
}

bool Node::get_is_sending()
{
	return is_sending;
}


void Node::set_commander(bool value)
{
	commander = value;
}

void Node::set_is_sending(bool value)
{
	is_sending = value;
}

vector<uint32_t> Node::get_output()
{
	return output_value;
}

std::vector<uint32_t> Node::get_input()
{
	return input;
}

void Node::set_output(uint32_t value)
{
	output_value.assign(1, value);  // insert value instead default value
}

void Node::set_input(uint32_t value)
{
	input.push_back(value);
}

void Node::set_type(uint32_t value)
{
	type = value;
}


void Node::print_node()
{
	
	//print number
	out_node << "Node number" << '\t' << get_id() << endl;
	//print output only for the commander. we don't care about output of regular nodes
	if (this->is_commander() == 1)
	{
		out_node << "Output value" << '\t';
		for (uint32_t i = 0; i < get_output().size(); i++)  //printing output
		{
			out_node << get_output()[i] << ' ';
		}
		out_node << "\t\t(0 - retreat; 1 - attack)" << endl;
	}
	//print type of node
	out_node << "Node type" << '\t' << get_type() << "\t\t(0 - loyal; 1 - traitor)" << endl;  
	//print majority matrix
	out_node << "Majority matrix:" << '\n';
	for (uint32_t i = 0; i < major_matrix.size(); i++)
	{
		out_node << "[";
		for (uint32_t j = 0; j < major_matrix[i].size(); j++)
		{
			if (j == major_matrix[i].size() - 1)
			{
				out_node << major_matrix[i][j];
			}
			else
				out_node << major_matrix[i][j] << ", ";
		}
		out_node << "] \t\tlevel " << i << endl;
	}
	out_node << endl;
}

void Node::print_node_on_screen()
{
	//print number
	cout << "Node number" << '\t' << get_id() << endl;
	//print output only for the commander. we don't care about output of regular nodes
	if (this->is_commander() == 1)
	{
		cout << "Output value" << '\t';
		for (uint32_t i = 0; i < get_output().size(); i++)  //printing output
		{
			cout << get_output()[i] << ' ';
		}
		cout << "\t\t(0 - retreat; 1 - attack)" << endl;
	}
	//print type of node
	cout << "Node type" << '\t' << get_type() << "\t\t(0 - loyal; 1 - traitor)" << endl;
	//print majority matrix
	cout << "Majority matrix:" << '\n';
	for (uint32_t i = 0; i < major_matrix.size(); i++)
	{
		cout << "[";
		for (uint32_t j = 0; j < major_matrix[i].size(); j++)
		{
			if (j == major_matrix[i].size() - 1)
			{
				cout << major_matrix[i][j];
			}
			else
				cout << major_matrix[i][j] << ", ";
		}
		cout << "] \t\tlevel " << i << endl;
	}
	cout << endl;
}


uint32_t Node::major_matrix_majority(uint32_t row_number)
{
	vector<int32_t> val_vec = major_matrix[row_number];
	int value_to_remove = -1;
	//delete -1 from the vector
	val_vec.erase(remove(val_vec.begin(), val_vec.end(), value_to_remove), val_vec.end());

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
	int max = INT16_MIN, key;
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

uint32_t Node::input_majority(vector<uint32_t> val_vec)
{
	int value_to_remove = -1;

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
	int max = INT16_MIN, key;
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

void Node::set_default_input(uint32_t mes)
{
	input.assign(1, mes);
}

void Node::input_clear()
{
	input.clear();
}

void Node::output_clear()
{
	output_value.clear();
}


	

void Node::default_value_major_matrix(uint32_t row_number)
{
	major_matrix[row_number].assign(major_matrix[row_number].size(), -1);  //put to the particular row value -1
}

void Node::resize_major_matrix(uint32_t number_of_rows, uint32_t number_of_colums)
{
	major_matrix.clear();
	for (uint32_t i = 0; i < number_of_rows; i++)
	{
		vector<int32_t> temp;
		for (uint32_t j = 0; j < number_of_colums; j++)
		{
			temp.push_back(-1);
		}
		major_matrix.push_back(temp);
	}
}

void Node::put_majority(uint32_t row_number, uint32_t column_number, uint32_t result)
{
	major_matrix[row_number][column_number] = result;
}

Node::~Node() {};


