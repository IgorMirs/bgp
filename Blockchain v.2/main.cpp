#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cstdint>
#include "Node.h"
#include "Network.h"
using namespace std;

ofstream out; //global variable for printing in file
extern ofstream out_node;


int menu_item()
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


void checkByzantineGeneral(Network from, uint32_t whereToPrint = 0)  //whereToPrint (0 - in file; 1 - to the console)
{
	Network temp_net = from;    //create a new network for checking with the opposite message
	uint32_t opposite_mes;
	if (from.getMessage() == 999)
	{
		cout << "CAN'T CHECK! YOU DID NOT SEND ANY MESSAGE." << endl;
		return;
	}
	if (!temp_net.checkByzantine(0, 0))  //check that even without changing messages and nodes solution not found
	{
		if (whereToPrint != 0)
		{
			temp_net.checkByzantine(1, 0);
		}
		else
		{
			temp_net.checkByzantine(0, 1);
		}
		return;
	}

	if (!from.isCommanderLoyal())    // if the commander traitor we should make it loyal and make one loyal node traitor
	{
		for (uint32_t i = 0; i < temp_net.GetNumberOfNodes(); i++)
		{
			if (temp_net.network[i].GetType() == 0)
			{
				temp_net.network[i].SetType(1);  //make the loyal node traitor
				break;
			}
		}
		for (uint32_t i = 0; i < temp_net.GetNumberOfNodes(); i++)
		{
			if (temp_net.network[i].GetIsCommander())
			{
				temp_net.network[i].SetType(0);  //make the commander traitor
				break;
			}
		}
		temp_net.SendMessages(from.getMessage());
		if (!temp_net.checkByzantine(0, 0))  //checking after changing nodes but still without changing message
		{
			if (whereToPrint != 0)
			{
				temp_net.checkByzantine(1, 0);
			}
			else
			{
				temp_net.checkByzantine(0, 1);
			}
			return;
		}
	}

	if (from.getMessage() == 0) { opposite_mes = 1; }  //making opposite message to real message
	else { opposite_mes = 0; }
	temp_net.SendMessages(opposite_mes);
	if (whereToPrint != 0)
	{
		temp_net.checkByzantine(1, 0);
	}
	else
	{
		temp_net.checkByzantine(0, 1);
	}
}

void test_case(uint32_t value)
{
	Network test_net(1), loyal(1), traitor(1);;
	out.open("Check table.txt");
	out << "M" << "\t|\t n" << "\t|\t m" << "\t|\t L" << "\t|\t T" << "\t|\t D" << endl; //message, number of nodes, number of traitors, loyals decision, traitor decision, total decision
	out << endl << "----------------------------------------------------------------------------------------" << endl;

	for (uint32_t i = 3; i <= value; i++)
	{
		
		test_net.resizeNetwork(i);
		for (uint32_t j = 1; j < test_net.GetNumberOfNodes(); j++)
		{
			test_net.setNumberOfTraitors(j);
			//make the majority matrix of each node with (GetNumberOfTraitors + 1) rows and (GetNumberOfNodes) columns
			for (uint32_t k = 0; k < test_net.GetNumberOfNodes(); k++)
			{
				test_net.network[k].resize_major_matrix(test_net.GetNumberOfTraitors() + 1, test_net.GetNumberOfNodes());  
			}
			//make the node traitor
			test_net.network[j].SetType(1);
			test_net.SendMessages(0);
			for (uint32_t k = 1; k < test_net.GetNumberOfNodes(); k++)
			{
			//	for (uint32_t z = 0; z < test_net.GetNumberOfNodes(); z++)
			//		out << test_net.network[k].major_matrix[0][z] << ", ";
			//	out << "\t Node type " << test_net.network[k].GetType();
			//	out << "\t Majority " << test_net.network[k].major_matrix_majority(0);
			//	out << endl;
				if (test_net.network[k].GetType() == 0)
					loyal.network[0].major_matrix[0].push_back(test_net.network[k].major_matrix_majority(0));
				else
					traitor.network[0].major_matrix[0].push_back(test_net.network[k].major_matrix_majority(0));
			}
		//	test_net.checkByzantine(0, 1);
			
		/*	out << "Loyal nodes decision " << endl;
			for (uint32_t z = 0; z < loyal_des.size(); z++)
				out << loyal_des[z] << " ";
			out << endl;
			


			out << "Traitor nodes decision " << endl;
			for (uint32_t z = 0; z < traitor_des.size(); z++)
				out << traitor_des[z] << " ";
			out << endl;
		*/	
			
			//test
			
			out << 0;
			out << "\t|\t " << i;
			out << "\t|\t " << j << "\t|\t ";

			//print majority of loyal nodes
			if (loyal.network[0].major_matrix_majority(0) != 999)
				out << loyal.network[0].major_matrix_majority(0);
			out << "\t|\t ";

			//print majority of traitor nodes
			if (traitor.network[0].major_matrix_majority(0) != 999)
				out << traitor.network[0].major_matrix_majority(0);
			out << "\t|\t ";

			out << test_net.checkByzantine(0,0) << endl;
			loyal.network[0].major_matrix[0].clear();
			traitor.network[0].major_matrix[0].clear();

		//	out << endl << "SENDING MESSAGE IS 1" << endl;
			test_net.SendMessages(1);
			for (uint32_t k = 1; k < test_net.GetNumberOfNodes(); k++)
			{
		/*		for (uint32_t z = 0; z < test_net.GetNumberOfNodes(); z++)
					out << test_net.network[k].major_matrix[0][z] << ", ";
				out << "\t Node type " << test_net.network[k].GetType();
				out << "\t Majority " << test_net.network[k].major_matrix_majority(0);
				out << endl;
		*/		if (test_net.network[k].GetType() == 0)
					loyal.network[0].major_matrix[0].push_back(test_net.network[k].major_matrix_majority(0));
				else
					traitor.network[0].major_matrix[0].push_back(test_net.network[k].major_matrix_majority(0));
			}

			out << 1;
			out << "\t|\t " << i;
			out << "\t|\t " << j << "\t|\t ";

			//print majority of loyal nodes
			if (loyal.network[0].major_matrix_majority(0) != 999)
				out << loyal.network[0].major_matrix_majority(0);
			out << "\t|\t ";

			//print majority of traitor nodes
			if (traitor.network[0].major_matrix_majority(0) != 999)
				out << traitor.network[0].major_matrix_majority(0);
			out << "\t|\t ";

			out << test_net.checkByzantine(0, 0) << endl;
			loyal.network[0].major_matrix[0].clear();
			traitor.network[0].major_matrix[0].clear();

			
		}
	}
	out.close();
	cout << "All data is written to the file 'Check table.txt'" << endl;
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

void testing(Network net)
{
	Network temp_net = net;
	temp_net.setTraitors();
	cout << endl << "SENDING MESSAGE IS 0" << endl;
	temp_net.SendMessages(0);
	checkByzantineGeneral(temp_net, 1);
	cout << endl << "--------------------------------------------------------------------------" << endl;
	cout << endl << "SENDING MESSAGE IS 1" << endl;
	temp_net.SendMessages(0);
	checkByzantineGeneral(temp_net, 1);
	cout << endl << "--------------------------------------------------------------------------" << endl;

}
int main()
{
	//file where we put all the steps of sending messages
	out_node.open("Test_case.txt");
	//get the size of the network
	uint32_t size = setSizeOfNetwork();
	//create network
	Network net(size);
	net.print_connect_matrix();
	//test_case(10);
	//menu
	bool Exit = false;
	do
	{
		char Control = menu_item();
		switch (Control)
		{
		case '1': {net.PrintNetworkOnScreen(); break; }
		case '2': {net.SendMessages(100); break; }
		case '3': {net.setTraitors(); break; }
		case '4': {checkByzantineGeneral(net, 1); break; }
		case '5': {cout << endl; net.resizeNetwork(setSizeOfNetwork()); break; }
		case '6': {testing(net); break; }
		case '0': {Exit = true; out_node.close(); break; }
		default: {cout << "There is no such action!" << endl; break; }
		}
	} while (!Exit);
	return 0;
}


