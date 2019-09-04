#include "Network.h"
#include "Print.h"
#include "Node.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

extern ofstream out;
extern ofstream out_node;
ifstream fin;

Network::Network(uint32_t value)
{
	network.resize(value);
	set_number_of_nodes(value);
	set_number_of_traitors(0);
	connect_matrix.resize_connectivity_matrix(value);
	for (uint32_t i = 0; i < value; i++)
	{
		network[i].set_id(i);   //set id from 0 to N-1
		network[i].set_type(0);
		network[i].set_node_decision(999);
		network[i].set_commander(false);
	}
	network[0].set_commander(true);  //Node with 0 ID became commander
	setMessage(999);   //set message to the default value
}

void Network::resize_network(uint32_t value)
{
	network.resize(value);
	set_number_of_nodes(value);
	set_number_of_traitors(0);
	connect_matrix.resize_connectivity_matrix(value);
	for (uint32_t i = 0; i < value; i++)
	{
		network[i].set_id(i);   //set id from 0 to N-1
		network[i].set_type(0);
		network[i].set_node_decision(999);
		network[i].set_commander(false);
	}
	network[0].set_commander(true);  //Node with 0 ID became commander
	setMessage(999);   //set message to the default value
}



uint32_t Network::make_fake_mes(uint32_t mes)
{
	if (mes == 0)
		return 1;//making fake message opposite to real message
	else
		return 0;
}


uint32_t Network::get_commander_id()
{
	for (uint32_t i = 0; i < get_number_of_nodes(); i++) {
		if (network[i].is_commander())
			return i;
	}
	return 999;
}



void Network::setTraitors()
{
	uint32_t M;
	bool Exit = false;
	//get the number of traitors
	do
	{
		cout << "Enter number of traitors: " << endl << "> ";
		cin >> M;
		cout << endl;
		if (M > this->get_number_of_nodes())
		{
			cout << "Number of traitors could not be more than number or nodes" << endl;
			cout << endl;
		}
		else { Exit = true; }
	} while (!Exit);

	if (M == 0) {  //if user entered 0 traitors
		cout << "You didn't set traitors. All nodes are loyals." << endl;
		return;
	}

	this->set_number_of_traitors(M);  //set the number of traitors in the network

	vector <uint32_t> trait_num(M);
	uint32_t value;
	Exit = false;
	//get the traitors numbers
	do
	{
		cout << "Enter node numbers which should be traitors" << endl
			<< "(you can enter numbers between 0 and " << this->get_number_of_nodes() - 1 << ")" << endl;
		bool exit = false;
		for (uint32_t i = 0; i < this->get_number_of_traitors(); i++)
		{
			if (exit)
				break;
			cin >> value;
			if (value > this->get_number_of_nodes() - 1)   //the traitors number could not be more than number of nodes
			{
				cout << "There is no so many nodes in the network" << endl << endl;
				break;
			}
			trait_num[i] = value;  //will have a vector with number of Nodes to be the traitors
			if (i == this->get_number_of_traitors() - 1)
				//here the user enterted all traitor numbers
			{
				//check if the user enterted the same traitor numbers
				for (uint32_t j = 0; j < trait_num.size(); j++)
				{
					if (exit)
						break;
					for (uint32_t k = j + 1; k < trait_num.size(); k++)
					{
						if (trait_num[j] == trait_num[k])
						{
							exit = true;
							cout << "You enterted similar values of traitors numbers. You should enter traitor numbers one more time." << endl << endl;
							break;
						}
					}
					if (j == trait_num.size() - 1)
						Exit = true;
				}
			}
		}

	} while (!Exit);

	for (uint32_t i = 0; i < this->get_number_of_nodes(); i++)
	{
		this->network[i].set_type(0);   //make all the nodes loyal
	}

	for (uint32_t i = 0; i < M; i++)
	{
		this->network[trait_num[i]].set_type(1);   //put the traitors to the select nodes in a previous step
	}
	cout << endl << "Traitors are set. The network returned to the default value." << endl;
}

bool Network::isCommanderLoyal()
{
	for (uint32_t i = 0; i < this->get_number_of_nodes(); i++)
	{
		if (this->network[i].is_commander() && this->network[i].get_type()) { //if the node is commander and the node is traitor
			return false;
		}
	}
	return true;
}


void Network::send_messages_recursive(bool entered_by_user, uint32_t mes,  uint32_t sender_id)
{
	if (entered_by_user){
		cout << "Enter message (0 - retreat; 1 - attack)" << endl << "> ";
		cin >> mes;
		cout << endl;
	}

	//set the original message in network and make the fake message
	setMessage(mes);    
	uint32_t fake_mes = make_fake_mes(mes);

	//set output of the sender
	//if the sender traitor - send the opposite message
	if (network[sender_id].get_type() == 0)
		network[sender_id].set_output(mes);
	else
		network[sender_id].set_output(fake_mes);
	
	//make the sender sending
	network[sender_id].set_is_sending(true);

	//clear the input vectors of all the nodes
	for (uint32_t i = 0; i < network.size(); i++)
		network.at(i).input_clear();

	//send the message from the sender to all other nodes
	for (uint32_t i = 0; i < network.size(); i++) {
		if (network[i].get_is_sending())
			continue;
		//calculate the shortest path from the sender to the current node
		//if there is at least one traitor on the path - send the opposite message
		vector<uint32_t> path = this->connect_matrix.shortest_path(sender_id, i);
		if (this->traitors_in_vector(path))
			network[i].set_input(fake_mes);
		else
			network[i].set_input(mes);
	}

	for (uint32_t i = 0; i < network.size(); i++) {
		if (network[i].get_is_sending())
			continue;
		//each node get a decision asking other nodes what they receive
		network.at(i).set_node_decision(calc_node_decision(network[sender_id].get_output()[0], i, this->get_number_of_traitors(), sender_id));
	}



	//here each node get the result
	cout << "Result input:" << endl;
	for (uint32_t i = 0; i < network.size(); i++) {
		if (network.at(i).get_is_sending())
			continue;
		cout << network.at(i).get_node_decision() << ' ';
	}

	cout << endl << "Sender output: " << endl;
	for (uint32_t i = 0; i < network.at(sender_id).get_output().size(); i++)
		cout << network.at(sender_id).get_output()[i] << ' ';
	cout << endl << "----------------------------------------" << endl;

	//end of sending
	network[sender_id].set_is_sending(false);

}

uint32_t Network::calc_node_decision(uint32_t mes, uint32_t node_number, uint32_t number_of_traitors, uint32_t sender_id)
{
	//returns the node's decision

	//if node is a traitor just simply respond the fake message opposite to the original
	if (network.at(node_number).get_type() == 1) 
			return make_fake_mes(this->getMessage());

	//if the node is loyal and there is no more traitors in the network
	//node return the majority of his input vector
	if (number_of_traitors == 0)
		return network[node_number].input_majority(network[node_number].get_input());

	//if the node is a loyal and there is still traitors in the network
	//the node recursively asks all the other not sending nodes about their decisions 
	//then put these responds to his input vector 
	//and finally take the majority to get his decision
	for (uint32_t i = 0; i < network.size(); i++) {
		if (i == node_number || network[i].get_is_sending())
			continue;
		network[node_number].set_is_sending(true);
		vector<uint32_t> path = this->connect_matrix.shortest_path(node_number, i);
		//if there is at least one traitor on the sending path
		//change the original message in the network to the opposite
		if (this->traitors_in_vector(path))
			network[node_number].set_input(make_fake_mes(this->getMessage()));
		else
			network[node_number].set_input(calc_node_decision(mes, i, number_of_traitors - 1, sender_id));
		network[node_number].set_is_sending(false);
	}
	uint32_t result = network[node_number].input_majority(network[node_number].get_input());
	network[node_number].set_default_input(mes);
	return result;
}







//vector <bool> a(6);
//vector <uint32_t> b(6);
//vector <vector<uint32_t>> matrix;
//
//void rec(uint32_t t)
//{
//    if (t == 6)
//    {
//        auto iter = b.cbegin();
//        b.insert(iter, 0);
//        matrix.push_back(b);
//        iter = b.cbegin();
//        b.erase(iter);
//    }
//
//    for (uint32_t i = 0; i < 6; i++)
//        if (!a[i])
//        {
//            b[t] = i + 1;
//            a[i] = true;
//            rec(t + 1);
//            a[i] = false;
//        }
//}




bool Network::traitors_in_vector(std::vector<uint32_t> vector)
{
	for (uint32_t i = 0; i < vector.size(); i++)
	{
		if (network[vector[i]].get_type() == 1) //if any of the middle nodes on the path is a traitor - return true
			return true;
	}
	return false;
}


bool Network::check_Byzantine_from_recursive()
{
	uint32_t commander_id = get_commander_id();
	if (commander_id == 999) {
		cout << "No commander in the network" << endl;
		return false;
	}
	bool only_traitors = true;
	for (uint32_t i = 0; i < this->get_number_of_nodes(); i++) {
		//do not check commander and traitors
		if (i == commander_id || network[i].get_type())
			continue;

		only_traitors = false;
		if (network[commander_id].get_output()[0] != network[i].get_node_decision())
			return false;
	}
	if (!only_traitors)
		return true;
	else
		return false;
}


Network::~Network() {};



