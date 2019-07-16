#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>
#include "Node.h"
#ifndef NETWORK_H
#define NETWORK_H

class Network
{

	uint32_t NumberOfNodes;
	uint32_t numberOfTraitors = 0;
	uint32_t message = 999;
public:
	std::vector<Node> network;  //array of nodes
	std::vector<std::vector<uint32_t>>  connect_matrix;  //connectivity matrix to detect connectivity of each node
	Network(uint32_t number_of_nodes);					
	void resizeNetwork(uint32_t value);			//edit the current network and make it with a new number of nodes
	uint32_t getMessage();						//get the original message of the network
	uint32_t make_fake_mes(uint32_t mes);		//make an opposite message from the given message
	void setMessage(uint32_t value);			//set the original message of the network
	void setTraitors();							//set the traitors in the network
	bool isCommanderLoyal();					//if commander is traitor - false; if loyal - true
	void SetNumberOfNodes(uint32_t value);		//set number of nodes in the network
	void setNumberOfTraitors(uint32_t value);	//set number of traitors in the network
	uint32_t GetNumberOfNodes();				//get number of nodes in the network
	uint32_t GetNumberOfTraitors();				//get number of traitors in the network
	void PrintNetwork();						//print every node in the network to the file
	void PrintNetworkOnScreen();				//print every node in the network to the screen
	void SendMessages(uint32_t value); // send messages to the network; if value == 0 user define the message; else - send "value" to network
	void low_level_send_mes(uint32_t sending_mes, std::vector<uint32_t>  who_is_commander); //sending messages between nodes on the lowest level
	void set_majority(std::vector<uint32_t> who_is_commander, uint32_t level, uint32_t column);		//set the majority of the particular level of majority matrix
																									// to the particular column
	void resize_connect_matrix(uint32_t number_of_nodes); //make a random connection of nodes
	void connect_matrix_from_file(); //upload connectivity matrix from file
	void connect_matrix_from_file2(std::vector <uint32_t> number); //upload connectivity matrix from file
	void print_connect_matrix();  //print connectivity matrix
	std::vector<uint32_t> shortest_path(uint32_t src, uint32_t dst); //define the shortest path for sending messages
	bool traitors_in_vector (std::vector<uint32_t> vector); //true if there is a traitors in any of the vector (for instance shortest path vector or who is commander vector)
	bool checkByzantine(uint32_t withConsoleMessages, uint32_t printInFile); //check current state of network (print message on console or in file)
	~Network();
};
#endif // NETWORK_H

