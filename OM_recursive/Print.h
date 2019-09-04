#include "Node.h"
#include "Network.h"
#include "Connectivity_matrix.h"

#ifndef PRINT_H
#define PRINT_H

class Print
{

public:

	void print_node_in_file(Node* node);        //print all characteristics of the node to the file
	void print_node_on_screen(Node* node);        //print all characteristics of the node to the screen
	void print_network_in_file(Network* net);    //print every node in the network to the file
	void print_network_on_screen(Network* net);    //print every node in the network to the screen
	void print_connect_matrix(Connectivity_matrix* net_connect);    //print connectivity matrix of the network
	~Print();
};

#endif // PRINT_H
