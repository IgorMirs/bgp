#include "Node.h"
#include <cstdint>
#include <fstream>
#include <iterator>

using namespace std;
ofstream out_node;

Node::Node()
{
	ID = 0;
	output_value.push_back(999);
	IsCommander = false;
	type = 0;
}

uint32_t Node::GetID()
{
	return ID;
}

uint32_t Node::GetType()
{
	return type;
}

void Node::SetID(uint32_t value)
{
	ID = value;
}

bool Node::GetIsCommander()
{
	return IsCommander;
}


void Node::SetIsCommander(bool commander)
{
	IsCommander = commander;
}

vector<uint32_t> Node::GetOutput()
{
	return output_value;
}

void Node::SetOutput(uint32_t value)
{
	output_value.assign(1, value);  // insert value instead default value
}

void Node::SetType(uint32_t value)
{
	type = value;
}


void Node::PrintNode()
{
	//print number
	out_node << "Node number" << '\t' << GetID() << endl;
	//print output only for the commander. we don't care about output of regular nodes
	if (this->GetIsCommander() == 1)
	{
		out_node << "Output value" << '\t';
		for (uint32_t i = 0; i < GetOutput().size(); i++)  //printing output
		{
			out_node << GetOutput()[i] << ' ';
		}
		out_node << "\t\t(0 - retreat; 1 - attack)" << endl;
	}
	//print type of node
	out_node << "Node type" << '\t' << GetType() << "\t\t(0 - loyal; 1 - traitor)" << endl;  
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

void Node::PrintNodeOnScreen()
{
	//print number
	cout << "Node number" << '\t' << GetID() << endl;
	//print output only for the commander. we don't care about output of regular nodes
	if (this->GetIsCommander() == 1)
	{
		cout << "Output value" << '\t';
		for (uint32_t i = 0; i < GetOutput().size(); i++)  //printing output
		{
			cout << GetOutput()[i] << ' ';
		}
		cout << "\t\t(0 - retreat; 1 - attack)" << endl;
	}
	//print type of node
	cout << "Node type" << '\t' << GetType() << "\t\t(0 - loyal; 1 - traitor)" << endl;
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
	vector<int32_t> valid_vector;
	//make a vector without -1
	bool no_values = true;  //in case that all values in a row equal to -1
	for (uint32_t i = 0; i < major_matrix[row_number].size(); i++)
	{
		if (major_matrix[row_number][i] == -1)
			continue;
		no_values = false;
		valid_vector.push_back(major_matrix[row_number][i]);
	}
	if (no_values)
		return 999;

	vector<int32_t> check = valid_vector;
	uint32_t count;
	for (uint32_t i = 0; i < valid_vector.size(); i++)
	{
		count = 1;
		for (uint32_t j = 0; j < valid_vector.size(); j++)
		{
			if (valid_vector[i] == valid_vector[j] && i != j)   //count the number of values
			{
				count++;
			}
			check[i] = count;
		}
		for (uint32_t k = 0; k < i; k++)  //check if this number already been counted
		{
			if (valid_vector[i] == valid_vector[k])
			{
				check[i] = 0;
			}
		}
	}
	int32_t max = check[0];
	uint32_t point = 0;
	for (uint32_t i = 1; i < check.size(); i++)   //searching for the most often value
	{
		if (check[i] > max)
		{
			max = check[i];
			point = i;
		}
	}
	bool NoMajority = false;
	for (uint32_t i = 1; i < check.size(); i++)            //check equal number
	{
		if (check[i] == check[point] && i != point)
		{
			//    cout << "There is no majority " << endl;
			NoMajority = true;
			return 0;
		}
	}
	if (!NoMajority)
	{
		return valid_vector[point];
	}
	else
	{
		return 0;
	}
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


