#include <fstream>
#include "Print.h"

using namespace std;
extern ofstream out_node;

void Print::print_node_in_file(Node* node)
{
	//print number
	out_node << "Node number" << '\t' << node->get_id() << endl;
	//print output only for the commander. we don't care about output of regular nodes
	if (node->is_commander() == 1)
	{
		out_node << "Output value" << '\t';
		//vector<uint32_t> output = node->get_output();
		//for (uint32_t i = 0; i < output.size(); i++)  //printing output
		//{
			out_node << node->get_output() << ' ';
		//}
		out_node << "\t\t(0 - retreat; 1 - attack)" << endl;
	}
	//print type of node
	out_node << "Node type" << '\t' << node->get_type() << "\t\t(0 - loyal; 1 - traitor)" << endl;
	//print majority matrix
	out_node << "Majority matrix:" << '\n';
	for (uint32_t i = 0; i < node->major_matrix.size(); i++)
	{
		out_node << "[";
		for (uint32_t j = 0; j < node->major_matrix[i].size(); j++)
		{
			if (j == node->major_matrix[i].size() - 1)
			{
				out_node << node->major_matrix[i][j];
			}
			else
				out_node << node->major_matrix[i][j] << ", ";
		}
		out_node << "] \t\tlevel " << i << endl;
	}
	out_node << endl;
}

void Print::print_node_on_screen(Node* node)
{
	//print number
	cout << "Node number" << '\t' << node->get_id() << endl;
	//print output only for the commander. we don't care about output of regular nodes
	if (node->is_commander() == 1)
	{
		cout << "Output value" << '\t';
		uint32_t output = node->get_output();
		//for (uint32_t i = 0; i < output.size(); i++)  //printing output
		//{
			cout << output << ' ';
		//}
		cout << "\t\t(0 - retreat; 1 - attack)" << endl;
	}
	//print type of node
	cout << "Node type" << '\t' << node->get_type() << "\t\t(0 - loyal; 1 - traitor)" << endl;
	//print node's decision
	cout << "Node decision" << '\t' << node->get_node_decision() << "\t\t(0 - retreat; 1 - attack)" << endl;


	cout << endl;
}

void Print::print_network_in_file(Network* net)
{
	out_node << endl << "The CURRENT MESSAGE in the network is " << net->message.get_mes() << endl << endl;
	for (uint32_t i = 0; i < net->network.size(); i++)
	{
		print_node_in_file(&net->network[i]);
	}
}

void Print::print_network_on_screen(Network* net)
{
	cout << endl << "The CURRENT MESSAGE in the network is " << net->message.get_mes() << endl << endl;
	print_connect_matrix(&net->connect_matrix);
	for (uint32_t i = 0; i < net->network.size(); i++)
	{
		print_node_on_screen(&net->network[i]);
	}
}

void Print::print_connect_matrix(Connectivity_matrix* net_connect)
{
	cout << "Connectivity matrix:" << '\n';
	for (uint32_t i = 0; i < net_connect->connect_matrix.size(); i++)
	{
		cout << "[";
		for (uint32_t j = 0; j < net_connect->connect_matrix[i].size(); j++)
		{
			if (j == net_connect->connect_matrix[i].size() - 1)
			{
				cout << net_connect->connect_matrix[i][j];
			}
			else
				cout << net_connect->connect_matrix[i][j] << ", ";
		}
		cout << "] " << endl;
	}
	cout << endl;
}


Print::~Print()
{
}
