#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cstdint>
#include <cmath>
#include "Node.h"
#include "Network.h"
#include "Print.h"

using namespace std;

ofstream out; //global variable for printing in file
extern ofstream out_node;


char menu_item()
{
	char variant;
	cout << endl << "Byzantine generals problem. Choose the option: \n";
	cout << "All data is written to the file 'Test_case.txt'" << endl;
	cout << "1. Print network\n" // print on the console the current state of the network
		<< "2. Send messages\n" //the user select which message to send
		<< "3. Set traitors\n"  //the user can set the number of traitors and the particular nodes numbers
		<< "4. CHECK GENERAL SOLUTION\n" //check the solution for this number of traitors
		<< "5. Resize network\n"  //change the size of current network
		<< "6. Test the algorithm\n" // check all the situations for different sizes of network and number of traitors
		<< "0. Exit\n" << endl;
	cout << "> ";
	cin >> variant;
	return variant;
}

bool check_recursive_sending(Network* from) {
	if (from->getMessage() == 999)
	{
		cout << "CAN'T CHECK! YOU DID NOT SEND ANY MESSAGE." << endl;
		return false;
	}
	if (!from->check_Byzantine_from_recursive())
		return false;

	uint32_t fake_mes = from->make_fake_mes(from->getMessage());

	from->send_messages_recursive(0,fake_mes);
	if (!from->check_Byzantine_from_recursive())
		return false;

	return true;
}



uint32_t setSizeOfNetwork()
{
	uint32_t value;
	bool Exit = false;
	do
	{
		cout << "Enter number of nodes (at least 3 nodes): " << endl << "> ";
		cin >> value;
		cout << endl;
		if (value < 3)
		{
			cout << "Should be at least 3 nodes in network" << endl;
			cout << endl;
		}
		else { Exit = true; }
	} while (!Exit);
	return value;
}




void testing_connectivity_recursive(Network* net)
{
	Network* temp_net = net;
	out.open("Test_nodes_connectivity_recursive.txt");
	uint32_t base = temp_net->get_number_of_nodes();
	for (uint32_t counter = 1; counter < base - 2; counter++)  // create a counter which goes from one traitor in the network to N-1 traitors
	{
		uint32_t degree = counter;  //current number of traitors
		uint32_t total = (int)pow(base, degree);
		for (uint32_t i = 1; i < total; i++)
		{
			vector <uint32_t> who_is_traitor(degree);
			uint32_t buffer = i;
			bool exit = false;
			for (uint32_t j = 0; j < degree; j++)  //in this loop getting the unique combination of traitors ( for instance 3 traitors - combination is 1, 2, 3)
			{
				if (exit)
					break;
				who_is_traitor[j] = buffer % base;
				//check if all the combination passed with the highest commander
				for (uint32_t z = 0; z < j; z++)
					if (who_is_traitor[z] <= who_is_traitor[j] || who_is_traitor[j] == 0)
					{
						exit = true;
						break;
					}
				buffer /= base;
			}
			if (!exit)
			{
				//print vector with traitors
				cout << endl;
				for (int32_t k = degree - 1; k >= 0; k--)
				{
					cout << who_is_traitor[k] << " ";
				}
				cout << endl;
				//goes through the traitors vector and set traitors
				for (int32_t k = degree - 1; k >= 0; k--)
					temp_net->network[who_is_traitor[k]].set_type(1);
				//set the number of traitors in the network
				temp_net->set_number_of_traitors(degree);
				// resize the majority matrices of all nodes according to the number of traitors
				/*for (uint32_t j = 0; j < temp_net->get_number_of_nodes(); j++)
				 temp_net->network[j].resize_major_matrix(temp_net->get_number_of_traitors() + 1, temp_net->get_number_of_nodes());
				 */
				temp_net->send_messages_recursive(0, 0);
				out << "Current traitors are:\t\t";
				for (int32_t k = degree - 1; k >= 0; k--)
				{
					out << who_is_traitor[k] << " ";
				}
				out << endl;

				//checking the solution
				out << "Solution (1 - found; 0 - not found)\t" << check_recursive_sending(temp_net) << endl << endl;

				//make the traitors loyal to make the next check
				for (int32_t k = degree - 1; k >= 0; k--)
					temp_net->network[who_is_traitor[k]].set_type(0);
			}
		}
	}
	out << endl << "--------------------------------------------------------------------------" << endl;
	out.close();
}

//global variables for finding the unique locations of the nodes in the network
vector <bool> a;
vector <uint32_t> b;
vector <vector<uint32_t>> matrix;

void location(uint32_t t, uint32_t degree) //finding the unique location of the nodes in the network
{
	if (t == degree)
	{
		auto iter = b.cbegin();
		b.insert(iter, 0);
		matrix.push_back(b);
		iter = b.cbegin();
		b.erase(iter);
	}

	for (uint32_t i = 0; i < degree; i++)
		if (!a[i])
		{
			b[t] = i + 1;
			a[i] = true;
			location(t + 1, degree);
			a[i] = false;
		}
}



int main(int argc, const char* argv[]) {
	//file where we put all the steps of sending messages
	//out_node.open("Test_case.txt");

	//create network and print class object
	Network* net = new Network(6);
	Print* pr = new Print();

	net->connect_matrix.connect_matrix_from_file();
	pr->print_connect_matrix(&net->connect_matrix);

	//testing_connectivity_recursive(net);


	//test**************************
	/*for (uint32_t i = 0; i < net->connect_matrix.connect_matrix.size(); i++) {
		for (uint32_t j = 0; j < net->connect_matrix.connect_matrix.size(); j++) {
			if (i == j)
				continue;
			vector<uint32_t> path = net->connect_matrix.shortest_path(i, j);

			cout << "The shortest path from " << i << " to " << j << " through" << endl;
			if (path.empty())
				cout << "Nodes connected";
			else
				for (uint32_t z = 0; z < path.size(); z++)
					cout << net->connect_matrix.shortest_path(i, j)[z] << ' ';
			cout << endl;
		}
	}*/
	
	//menu
	bool Exit = false;
	do
	{
		char Control = menu_item();
		switch (Control)
		{
		case '1':
			pr->print_network_on_screen(net);
			break;
		case '2':
			net->send_messages_recursive(1);
			break;
		case '3':
			net->setTraitors();
			break;
		case '4':
			check_recursive_sending(net);
			break;
		case '5':
			net->resize_network(setSizeOfNetwork());
			break;
		case '6':
			testing_connectivity_recursive(net);
			break;
		case '0':
			Exit = true;
			delete pr;
			delete net;
			break;
		default:
			cout << "There is no such action!" << endl;
			break;
		}
	} while (!Exit);
	return 0;
}


