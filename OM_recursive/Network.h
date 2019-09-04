#include <cstdint>
#include <iostream>
#include <vector>
#include <iterator>
#include "Node.h"
#include "Connectivity_matrix.h"
#ifndef NETWORK_H
#define NETWORK_H

class Network
{

	uint32_t NumberOfNodes = 0;
	uint32_t numberOfTraitors = 0;
	uint32_t message = 999;

	//the node_number decision:
	//mes - particular message that sender_id sends to the network
	//number_of_traitors in the network
	//sender_id - observer who sends the message to the network
	uint32_t calc_node_decision(uint32_t mes, uint32_t node_number, uint32_t number_of_traitors, uint32_t sender_id);


public:
	std::vector<Node> network;                             //array of nodes
	Connectivity_matrix connect_matrix;

	//constructor
	Network(uint32_t number_of_nodes = 1);
	void resize_network(uint32_t size);
	

	//getters
	uint32_t getMessage() { return uint32_t(message); }                        //get the original message of the network
	uint32_t get_number_of_nodes() { return NumberOfNodes; }            //get number of nodes in the network
	uint32_t get_number_of_traitors() { return numberOfTraitors; }            //get number of traitors in the network
	uint32_t get_commander_id();                                        //get the commander's id
	//setters
	void set_number_of_nodes(uint32_t value) { NumberOfNodes = value; }        //set number of nodes in the network
	void set_number_of_traitors(uint32_t value) { numberOfTraitors = value; }    //set number of traitors in the network
	void setMessage(uint32_t value) { message = value; }            //set the original message of the network
	void setTraitors();                            //set the traitors in the network

	uint32_t make_fake_mes(uint32_t mes);        //make an opposite message from the given message

	bool isCommanderLoyal();                    //if commander is traitor - false; if loyal - true
	void send_messages_recursive(bool entered_by_user = false, uint32_t mes = 999,  uint32_t sender_id = 0);



	bool traitors_in_vector(std::vector<uint32_t> vector); //true if there is a traitors in any of the vector (for instance shortest path vector or who is commander vector)
	bool check_Byzantine_from_recursive();
	~Network();
};
#endif // NETWORK_H

