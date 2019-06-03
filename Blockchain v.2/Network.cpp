#include "Network.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

extern ofstream out;
extern ofstream out_node;
ifstream fin;

Network::Network()
{
	uint32_t N = 1;   ///mode here
	network.resize(N);   //make size of array of Nodes equals N, avoid to have memory lost
	SetNumberOfNodes(N);
	for (uint32_t i = 0; i < N; i++)
	{
		network[i].SetID(i);   //set id from 0 to N-1
	}
	network[0].SetIsCommander(true);  //Node with 0 ID became commander
}

Network::Network(uint32_t number_of_nodes)
{
	network.resize(number_of_nodes);   //make size of array of Nodes equals N, avoid to have memory lost
	SetNumberOfNodes(number_of_nodes);
	resize_connect_matrix(number_of_nodes);
	for (uint32_t i = 0; i < number_of_nodes; i++)
	{
		network[i].SetID(i);   //set id from 0 to N-1
		network[i].resize_major_matrix(1, number_of_nodes); //resize the majority matrix
	}
	network[0].SetIsCommander(true);  //Node with 0 ID became commander
}

void Network::resizeNetwork(uint32_t value)
{
	network.resize(value);   //make size of array of Nodes equals N, avoid to have memory lost
	SetNumberOfNodes(value);
	setNumberOfTraitors(0);
	resize_connect_matrix(value);
	for (uint32_t i = 0; i < value; i++)
	{
		network[i].SetID(i);   //set id from 0 to N-1
		network[i].SetType(0);
		network[i].resize_major_matrix(1, value);
	}
	network[0].SetIsCommander(true);  //Node with 0 ID became commander
}

uint32_t Network::getMessage()
{
	return uint32_t(message);
}

uint32_t Network::make_fake_mes(uint32_t mes)
{
	if (mes == 0)
		return 1;//making fake message opposite to real message
	else 
		return 0;
}

void Network::setMessage(uint32_t value)
{
	message = value;
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
		if (M > this->GetNumberOfNodes())
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
	
	this->setNumberOfTraitors(M);  //set the number of traitors in the network

	vector <uint32_t> trait_num(M);
	uint32_t value;
	Exit = false;
	//get the traitors numbers
	do
	{
		cout << "Enter node numbers which should be traitors" << endl
			<< "(you can enter numbers between 0 and " << this->GetNumberOfNodes() - 1 << ")" << endl;
		bool exit = false;
		for (uint32_t i = 0; i < this->GetNumberOfTraitors(); i++)
		{
			if (exit)
				break;
			cin >> value;
			if (value > this->GetNumberOfNodes() - 1)   //the traitors number could not be more than number of nodes
			{
				cout << "There is no so many nodes in the network" << endl << endl;
				break;
			}
			trait_num[i] = value;  //will have a vector with number of Nodes to be the traitors
			if (i == this->GetNumberOfTraitors() - 1)
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

	for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
	{
		this->network[i].SetType(0);   //make all the nodes loyal
		this->network[i].resize_major_matrix(this->GetNumberOfTraitors() + 1, this->GetNumberOfNodes());  //make the majority matrix of each node with (GetNumberOfTraitors + 1) rows and (GetNumberOfNodes) columns
	}

	for (uint32_t i = 0; i < M; i++)
	{
		this->network[trait_num[i]].SetType(1);   //put the traitors to the select nodes in a previous step
	}
	cout << endl << "Traitors are set. The network returned to the default value." << endl;
}

bool Network::isCommanderLoyal()
{
	for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
	{
		if (this->network[i].GetIsCommander() && this->network[i].GetType()) { //if the node is commander and the node is traitor
			return false;
		}
	}
	return true;
}

void Network::PrintNetwork()
{
	out_node << endl << "The CURRENT MESSAGE in the network is " << this->getMessage() << endl << endl;
	for (uint32_t i = 0; i < network.size(); i++)
	{
		network[i].PrintNode();
	}
}

void Network::PrintNetworkOnScreen()
{
	cout << endl << "The CURRENT MESSAGE in the network is " << this->getMessage() << endl << endl;
	this->print_connect_matrix();
	cout << endl << endl;
	for (uint32_t i = 0; i < network.size(); i++)
	{
		network[i].PrintNodeOnScreen();
	}
}

void Network::SetNumberOfNodes(uint32_t value)
{
	NumberOfNodes = value;
}

void Network::setNumberOfTraitors(uint32_t value)
{
	numberOfTraitors = value;
}

uint32_t Network::GetNumberOfNodes()
{
	return NumberOfNodes;
}

uint32_t Network::GetNumberOfTraitors()
{
	return numberOfTraitors;
}


void Network::SendMessages(uint32_t value = 100)
{
	//clear the majority matrix of all nodes
	for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
	{
		for (uint32_t j = 0; j < this->GetNumberOfTraitors(); j++)
			network[i].default_value_major_matrix(j);
	}

	uint32_t fake_mes;
	bool messageEnteredByUser = false;
	//input value 100 means that the message will be entered by user
	if (value == 100)
	{
		messageEnteredByUser = true;
		cout << "Enter message (0 - retreat; 1 - attack)" << endl << "> ";
		cin >> value;
		cout << endl;
		setMessage(value);    //the original message in network
		fake_mes = make_fake_mes(value); //making fake message opposite to real message
	}
	else
	{
		setMessage(value);    //the original message in network
		fake_mes = make_fake_mes(value); //making fake message opposite to real message
	}

	//set the output of the commander
	for (uint32_t co = 0; co < this->GetNumberOfNodes(); co++)
	{
		if (network[co].GetIsCommander() == 1)
		{
			if (network[co].GetType() == 0)        //if commander loyal
				network[co].SetOutput(value);
			else
				network[co].SetOutput(fake_mes); //if commander traitor
			//sending  the commanders output to the highest level of each node
			for (uint32_t co2 = 0; co2 < this->GetNumberOfNodes(); co2++)
			{
				if (network[co2].GetIsCommander() == 1)  //do not send to himself
					continue;
				vector<uint32_t> path = shortest_path(co, co2);
				if(traitors_in_vector(path))  //if there are traitors on the sending path - node get the opposite message
					network[co2].major_matrix[0][co] = make_fake_mes(network[co].GetOutput()[0]);
				else
					network[co2].major_matrix[0][co] = network[co].GetOutput()[0];
			}
			break;
		}
	}

	//if there is no traitors in the network, just simple sending between nodes
	if (this->GetNumberOfTraitors() == 0)
	{
		this->low_level_send_mes(value, { 1000 });   //modify here
		for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
		{
			if (network[i].GetType() == 0)
				network[i].SetOutput(value);  //set the current message to the output of the node
		}
		return;
	}


	//create a vector with the commanders on each step
	uint32_t base = this->GetNumberOfNodes();
	uint32_t degree = this->GetNumberOfTraitors() + 1;
	uint32_t total = pow(base, degree);
	//cout << "Base and degree " << base << " " << degree << endl;
	//cout << "Total " << total << endl;

	bool firststep_end = false;
	bool firststep = true;
	vector <uint32_t> previous_commander(degree);
	uint32_t level = this->GetNumberOfTraitors(), column = 0;
	for (uint32_t i = 0; i < total; i++)
	{
		if (firststep_end)
			break;
		vector <uint32_t> who_is_commander(degree);
		uint32_t buffer = i;
		bool exit = false;
		for (uint32_t j = 0; j < degree; j++)
		{
			if (exit)
				break;
			who_is_commander[j] = buffer % base;
			//check if all the combination passed with the highest commander
			if (who_is_commander[degree - 1] > 0)
			{
				firststep_end = true;
				//we are on the last step we should count the majority of all not empty levels
				//step by step (from lowest to highest)

				out_node << "---------------------/LAST STEP/-------------------------------" << endl;
				//here we should count the majority of all the levels from the lowest to the highest
				uint32_t level_temp = this->GetNumberOfTraitors();
				while (level_temp)
				{
					for (uint32_t t = 0; t < this->GetNumberOfNodes(); t++)
					{
						if (network[t].major_matrix_majority(level_temp) == 999) // do not take into account nodes which didn't communicate
						{
							continue;
						}

						//the node should consider the value that he get from the comander of the previous step
						//check if any of the current commanders is traitor this mean that the node will get the fake message because the message was changed with any traitor
						bool find_traitor = false;
						//start from different k any time because we should check less number of commanders in who_is_commander vector
						for (uint32_t k = 1 + column; k < who_is_commander.size(); k++)
						{
							if (network[previous_commander[k]].GetType() == 1)  //if any commander is traitor
							{
								uint32_t fake_mes;
								if (this->getMessage() == 0) { fake_mes = 1; }  //making fake message opposite to real message
								else { fake_mes = 0; }
								network[t].major_matrix[level_temp][network[t].GetID()] = fake_mes;
								find_traitor = true;
								break;
							}
						}
						if (!find_traitor)
							network[t].major_matrix[level_temp][network[t].GetID()] = this->getMessage();

						out_node << "---------------------/AFTER VALUE OF THE PREVIOUS STEP NOT ON THE LOWEST LEVEL/-------------------------------" << endl;
						this->PrintNetwork();
						network[t].major_matrix[level_temp - 1][previous_commander[column]] = network[t].major_matrix_majority(level_temp);
						network[t].default_value_major_matrix(level_temp);
					}
					level_temp--;
					column++;
				}

				out_node << "---------------------/AFTER SET MAJORITY NOT ON THE LOWEST LEVEL/-------------------------------" << endl;
				this->PrintNetwork();

				//end of sending

				break;
			}
			for (uint32_t z = 0; z < j; z++)
				if (who_is_commander[z] == who_is_commander[j])
				{
					exit = true;
					break;
				}

			buffer /= base;
		}

		// HERE WE FOUND unique who_is_commander vector
		if (!exit)
		{
			//print vector with commanders
			out_node << "WHO_IS_COMMANDER VECTOR" << endl;
			for (uint32_t k = 0; k < degree; k++)
			{
				out_node << who_is_commander[k] << " ";
			}
			out_node << endl;

			//define level (row in majority matrix) and column (column in majority matrix)
			if (firststep)
			{
				previous_commander = who_is_commander;
				firststep = false;
				level = this->GetNumberOfTraitors();
				column = 0;
			}
			else {
				for (int32_t i1 = who_is_commander.size() - 1, counter = 0; i1 > -1; i1--)
				{
					if (previous_commander[i1] != who_is_commander[i1])
					{
						level = counter;
						column = i1;
						break;
					}
					counter++;
				}
			}

			//if changed not the lowest level
			if (level != this->GetNumberOfTraitors())
			{
				if (level == 0)
					break;
				uint32_t level_temp = this->GetNumberOfTraitors(), column_temp = 0;
				while (level_temp >= level)
				{
					for (uint32_t t = 0; t < this->GetNumberOfNodes(); t++)
					{
						if (network[t].major_matrix_majority(level_temp) == 999) // do not take into account nodes which didn't communicate
						{
							continue;
						}
						//the node should consider the value that he get from the comander of the previous step
						//check if any of the current commanders is traitor this mean that the node will get the fake message because the message was changed with any traitor
						bool find_traitor = false;
						for (uint32_t k = 1 + column_temp; k < who_is_commander.size(); k++)
						{
							if (network[previous_commander[k]].GetType() == 1)  //if any commander is traitor
							{
								//if we found traitor on any higher step that means that the node received fake message
								network[t].major_matrix[level_temp][network[t].GetID()] = make_fake_mes(this->getMessage());
								find_traitor = true;
								break;
							}
						}
						if (!find_traitor)
							//if we didn't find traitor on any higher step that means that the node received original message
							network[t].major_matrix[level_temp][network[t].GetID()] = this->getMessage();
						out_node << "---------------------/AFTER VALUE OF THE PREVIOUS STEP NOT ON THE LOWEST LEVEL/-------------------------------" << endl;
						out_node << "current node is " << t << endl;
						this->PrintNetwork();
						//put the majority of the 'level_temp' to the 'level_temp - 1'
						//detect the column where to put according to the previous who_is_commander vector and read the higher value of this vector on each step
						network[t].major_matrix[level_temp - 1][previous_commander[column_temp]] = network[t].major_matrix_majority(level_temp);
						//put the -1 to 'level_temp' level
						network[t].default_value_major_matrix(level_temp);
					}
					level_temp--;
					column_temp++;
				}
				//put the level and column to default values
				//level points to the lowest level of the majority matrix
				//column = 0 helps to execute the lowest commander in who_is_commander vector
				level = this->GetNumberOfTraitors();
				column = 0;
				out_node << "---------------------/WHO IS THE COMMANDER VECTOR/-------------------------------" << endl;
				for (uint32_t k = 0; k < degree; k++)
				{
					out_node << who_is_commander[k] << " ";
				}
				out_node << endl;
				out_node << "---------------------/AFTER SET MAJORITY NOT ON THE LOWEST LEVEL/-------------------------------" << endl;
				this->PrintNetwork();
			}
			previous_commander = who_is_commander;

			//check if any of current sending nodes is a traitor
			//if any of the commanders is a traitor we send the fake message
			if (traitors_in_vector(who_is_commander))
				this->low_level_send_mes(fake_mes, who_is_commander); 
			//if all the commanders are loyal we send the original message
			else
				this->low_level_send_mes(value, who_is_commander);

			//set the majority
			this->set_majority(who_is_commander, level, column);
			out_node << "---------------------/AFTER SET MAJORITY/-------------------------------" << endl;
			this->PrintNetwork();
		}
	}

	if (messageEnteredByUser)
	{
		cout << "Messages sent!" << endl;
	}
}

void Network::low_level_send_mes(uint32_t sending_mes, std::vector <uint32_t> who_is_commander)
{
	bool node_is_already_sending;
	bool node_not_receive;

	//check is any traitor in who_is_commander vector
	bool commanders_loyal = false;
	if (!traitors_in_vector(who_is_commander))  //if there is no traitors in commanders vector this mean all commanders are loyal
		commanders_loyal = true;

	for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
	{
		node_is_already_sending = false; //if there is such node in a who is commander vector
		//Check is sending node in the who is commander vector
		//if yes this node do not send
		for (uint32_t k = 1; k < who_is_commander.size(); k++) //k starts with 1 because who_is_commander[0] is the lowest commander
		{                                                        //the lowest commander is sending in the lowest level
			if (network[i].GetID() == who_is_commander[k])
			{
				node_is_already_sending = true;
				break;
			}
		}
		if (node_is_already_sending)  // if node was in who_is_commander vector
			continue;

		uint32_t current_mes = sending_mes;

		//checking is the current node a traitor and what to do with a current message
		if (network[i].GetType() == 1) //if the node is traitor he checks were any traitors before.
		{                                //    if yes he do not change the current message. because all traitors work in team
			if (commanders_loyal) {
				current_mes = make_fake_mes(sending_mes);  //making fake message opposite to sending message
				
			}
		}

		//begin sending messages
		for (uint32_t j = 0; j < this->GetNumberOfNodes(); j++)
		{
			//check commanders, all commanders do not receive messages
			node_not_receive = false;
			for (uint32_t k = 0; k < who_is_commander.size(); k++)
			{
				if (network[j].GetID() == who_is_commander[k])
				{
					node_not_receive = true;
					break;
				}
			}
			if (node_not_receive)
				continue;

			if (j == i)    // node do not send message to himself
			{
				//the node should consider the value that he get from the comander of the previous step
				bool find_traitor = false;
				for (uint32_t k = 1; k < who_is_commander.size(); k++)  //k starts with 1 because who_is_commander[0] is the lowest commander
				{
					if (network[who_is_commander[k]].GetType() == 1)
					{
						network[j].major_matrix[this->GetNumberOfTraitors()][i] = make_fake_mes(this->getMessage()); //making fake message opposite to real message
						find_traitor = true;
						break;
					}
				}
				if (!find_traitor)
					network[j].major_matrix[this->GetNumberOfTraitors()][j] = this->getMessage();

				if (j != GetNumberOfNodes() - 1)
					continue;            //go to the next node
				else
				{
					//here we pass all the nodes this is the last node
					break;                //if it is the last node - end sending messages
				}
			}
			//---------------------------------------------EXPERIMENT-----------------------------------
			out_node << "SHORTEST PATH" << endl;
			out_node << "from " << i << "	to " << j << endl;
			vector<uint32_t> path = shortest_path(i, j);
			for (int32_t path_c = path.size() - 1; path_c >= 0; path_c--)
				out_node << path[path_c] << '	';
			out_node << endl;
			out_node << "TRAITORS ON THE PATH " << traitors_in_vector(path) << endl;
			//---------------------------------------------END_OF_EXPERIMENT-----------------------------------
			if (network[i].GetType() != 1 && traitors_in_vector(path) && commanders_loyal) //if there is a traitor on the sending path and message wasn't previously changed - we change the current message
				network[j].major_matrix[this->GetNumberOfTraitors()][i] = make_fake_mes(sending_mes);  // for receving node j we put the current message to the row level and to the column of the sending node
			else
				network[j].major_matrix[this->GetNumberOfTraitors()][i] = current_mes;
		}
	}

	out_node << "---------------------/END OF LOW SENDING/-------------------------------" << endl;
	out_node << "WHO IS SENDING " << who_is_commander[0] << endl << endl;
	this->PrintNetwork();
}

void Network::set_majority(vector<uint32_t> who_is_commander, uint32_t level, uint32_t column)
{
	//if node in who_is_commander vector - we do not count majority for this node
	for (uint32_t n = 0; n < this->GetNumberOfNodes(); n++)
	{
		bool node_not_communicate = false;
		for (uint32_t k = 0; k < who_is_commander.size(); k++)
		{
			if (network[n].GetID() == who_is_commander[k])
			{
				node_not_communicate = true;
				break;
			}
		}
		if (node_not_communicate)  // if node is in who_is_commander vector
			continue;
		if (level == 0)
			break;

		//the node should consider the value that he get from the comander of the previous step
		bool find_traitor = false;
		for (uint32_t k = 1; k < who_is_commander.size(); k++)
		{
			if (network[who_is_commander[k]].GetType() == 1)
			{
				uint32_t fake_mes;
				if (this->getMessage() == 0) { fake_mes = 1; }  //making fake message opposite to real message
				else { fake_mes = 0; }
				network[n].major_matrix[level][network[n].GetID()] = fake_mes;
				find_traitor = true;
				break;
			}
		}
		if (!find_traitor)
			network[n].major_matrix[level][network[n].GetID()] = this->getMessage();

		//cout << "Majority " << network[n].major_matrix_majority(level) << " for node " << n << endl << endl;
		network[n].major_matrix[level - 1][who_is_commander[column]] = network[n].major_matrix_majority(level);
		network[n].default_value_major_matrix(level); //return the level to -1

	}
}

void Network::resize_connect_matrix(uint32_t number_of_nodes)
{
	connect_matrix.clear();
	for (uint32_t i = 0; i < number_of_nodes; i++)
	{
		vector<uint32_t> temp;
		for (uint32_t j = 0; j < number_of_nodes; j++)
		{
			temp.push_back(0);
		}
		connect_matrix.push_back(temp);
	}
	//put the connection to the random nodes
	/*if (number_of_nodes > 1) {
		for (uint32_t i = 0; i < number_of_nodes; )
		{
			uint32_t R = rand() % this->GetNumberOfNodes();
			if (R != i && connect_matrix[i][R] != 1)
			{
				connect_matrix[i][R] = 1;
				connect_matrix[R][i] = 1;
				i++;
			}
		}
		//check is any node has at least two connections
		for (uint32_t i = 0; i < number_of_nodes; i++)
		{
			uint32_t conn_quant = 0;
			for (uint32_t j = 0; j < number_of_nodes; j++)
			{
				if (connect_matrix[i][j] == 1)
					conn_quant++;
			}
			if (conn_quant < 2)
			{
				bool exit = false;
				while (!exit)
				{
					uint32_t R = rand() % this->GetNumberOfNodes();
					if (R != i && connect_matrix[i][R] != 1)
					{
						connect_matrix[i][R] = 1;
						connect_matrix[R][i] = 1;
						exit = true;
					}
				}
			}
		}
	}*/
}

void Network::connect_matrix_from_file()
{
	connect_matrix.clear();
	for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
	{
		vector<uint32_t> temp;
		for (uint32_t j = 0; j < this->GetNumberOfNodes(); j++)
		{
			temp.push_back(0);
		}
		connect_matrix.push_back(temp);
	}

	uint32_t node1, node2;
	ifstream fin("Connectivity.txt");
	while (fin >> node1 >> node2)
	{
		if (node1 >= this->GetNumberOfNodes() || node2 >= this->GetNumberOfNodes())
		{
			cout << "Connectivity file is wrong!" << endl;
			connect_matrix.clear();
			break;
		}
		connect_matrix[node1][node2] = 1;
		connect_matrix[node2][node1] = 1;
	}
	fin.close();
}

void Network::print_connect_matrix() 
{
	cout << "Connectivity matrix:" << '\n';
	for (uint32_t i = 0; i < connect_matrix.size(); i++)
	{
		cout << "[";
		for (uint32_t j = 0; j < connect_matrix[i].size(); j++)
		{
			if (j == connect_matrix[i].size() - 1)
			{
				cout << connect_matrix[i][j];
			}
			else
				cout << connect_matrix[i][j] << ", ";
		}
		cout << "] " << endl;
	}
}

std::vector<uint32_t> Network::shortest_path(uint32_t src, uint32_t dst)
{
	const uint32_t SIZE = this->GetNumberOfNodes();
	vector<uint32_t> dist(SIZE);  //minimal distance
	vector<bool> vert(SIZE); //checked vertices
	uint32_t minindex;
	dist.assign(SIZE, UINT32_MAX); //make all the distances really big
	dist[src] = 0; //the distance to itself equal to 0 
 	vert.assign(SIZE, false);    //make all the vertices not checked

	do {
		minindex = UINT32_MAX;
		uint32_t min = UINT32_MAX;
		for (uint32_t i = 0; i < SIZE; i++)
		{ // If vert not checked and dist less than min
			if ((vert[i] == 0) && (dist[i] < min))
			{ 
				min = dist[i];
				minindex = i;
			}
		}
		//	Add the minimum weight to the current weight of the vert
		if (minindex != UINT32_MAX)
		{
			for (uint32_t i = 0; i < SIZE; i++)
			{
				if (connect_matrix[minindex][i] > 0)
				{
					uint32_t temp = min + connect_matrix[minindex][i];
					if (temp < dist[i])
					{
						dist[i] = temp;
					}
				}
			}
			vert[minindex] = 1;
		}
	} while (minindex < UINT32_MAX);

	// Making the path
	//***********************************create array here******************************************************
	vector<vector<uint32_t>> ver(SIZE); // array of vertices
	uint32_t end = dst; // end vertex
	uint32_t j = 0;
	ver[j].push_back(end); // put the end vertex to beginning
	int weight = dist[end]; // weight of end vertex

	while (end != src) // while not the source vertex
	{
		for (uint32_t i = 0; i < SIZE; i++)
			if (connect_matrix[end][i] != 0)   // if there is a connection in the connectivity matrix
			{
				int temp = weight - connect_matrix[end][i]; // weight from the previous vertex
				if (temp == dist[i]) // if the weight equal
				{                 
					ver[j].push_back(i);
					weight = temp; // save a new weight
					end = i;       // make a new dst
					break;
				}
			}
	}
	//want to return only nodes in the middle between two nodes 
	//delete the first element
	auto iter = ver[j].cbegin();
	ver[j].erase(iter);
	//delete the last element
	ver[j].pop_back();

	return ver[j];
}

bool Network::traitors_in_vector (std::vector<uint32_t> vector)
{
	for (uint32_t i = 0; i < vector.size(); i++)
	{
		if (network[vector[i]].GetType() == 1) //if any of the middle nodes on the path is a traitor - return true
			return true;
	}
	return false;
}

bool Network::checkByzantine(uint32_t withConsoleMessages = 0, uint32_t printInFile = 0)
{
	bool onlyTraitors = true;  //in case that we will not check any node
	bool withMessages = false;  // print messages on console
	bool filePrint = false;
	if (withConsoleMessages != 0)
	{
		withMessages = true;
	}
	if (printInFile != 0)
	{
		filePrint = true;
	}
	if (this->isCommanderLoyal())    //in case the commander is loyal. (IC2 all loyal nodes should obey the order of loyal commander)
	{
		for (uint32_t i = 0; i < this->GetNumberOfNodes(); i++)
		{
			if (this->network[i].GetType() == 0 && !this->network[i].GetIsCommander())    //check only loyal nodes and not commander
			{
				onlyTraitors = false; //in case that all nodes are traitors
				if (this->network[i].major_matrix_majority(0) != getMessage())   //majority input of each loyal node not equal the original message
				{
					if (withMessages)
					{
						cout << endl << "SOLUTION NOT FOUND!!!" << endl <<
							"The commander is loyal." << endl <<
							"The node " << i << " didn't obey the commander's order " << getMessage() << endl;
						this->PrintNetwork();
					}
					if (filePrint)
					{
						out << endl << "SOLUTION NOT FOUND!!!" << endl <<
							"The commander is loyal." << endl <<
							"The node " << i << " didn't obey the commander's order " << getMessage() << endl;
					}
					return 0;
				}
			}
		}
		if (!onlyTraitors)
		{
			if (withMessages)
			{
				cout << endl << "SOLUTION FOUND!!!" << endl <<
					"The commander is loyal." << endl <<
					"All loyal nodes obey the commander's order " << getMessage() << endl;
			}
			if (filePrint)
			{
				out << endl << "SOLUTION FOUND!!!" << endl <<
					"The commander is loyal." << endl <<
					"All loyal nodes obey the commander's order " << getMessage() << endl;
			}
			return 1;
		}
		else
		{
			if (withMessages)
			{
				cout << endl << "SOLUTION NOT FOUND!!!" << endl
					<< "All nodes are traitors" << endl;
				this->PrintNetwork();
			}
			if (filePrint)
			{
				out << endl << "SOLUTION NOT FOUND!!!" << endl
					<< "All nodes are traitors" << endl;
			}
			return 0;
		}
	}
	else    //the commander is traitor (IC1 all loyal nodes should obey the same order)
	{
		if (this->GetNumberOfNodes() == this->GetNumberOfTraitors())
		{
			if (withMessages)
			{
				cout << endl << "SOLUTION NOT FOUND!!!" << endl
					<< "All nodes are traitors including commander" << endl;
				this->PrintNetwork();
			}
			if (filePrint)
			{
				out << endl << "SOLUTION NOT FOUND!!!" << endl
					<< "All nodes are traitors including commander" << endl;
			}
			return 0;
		}
		vector <uint32_t> loyalNodesSolution(this->GetNumberOfNodes() - this->GetNumberOfTraitors()); //put the size of vector equal number of loyal nodes

		for (uint32_t i = 0, j = 0; i < this->GetNumberOfNodes(); i++)
		{
			if (network[i].GetType() == 0)
			{
				loyalNodesSolution[j] = network[i].major_matrix_majority(0);  //the vector with the majorities of all loyal nodes
				j++;
			}
		}


		for (uint32_t i = 1; i < loyalNodesSolution.size(); i++)
		{
			if (loyalNodesSolution[0] != loyalNodesSolution[i])   //check that all the nodes take the same decision
			{
				if (withMessages)
				{
					cout << endl << "SOLUTION NOT FOUND!!!" << endl << "Commander is traitor." << endl <<
						"Not all loyal nodes took the same decision." << endl;
					this->PrintNetwork();
				}
				if (filePrint)
				{
					out << endl << "SOLUTION NOT FOUND!!!" << endl << "Commander is traitor." << endl <<
						"Not all loyal nodes took the same decision." << endl;
				}
				return 0;
			}
		}
		if (withMessages)
		{
			cout << endl << "SOLUTION FOUND!!!" << endl << "Commander is traitor." << endl <<
				"All loyal nodes took the same decision " << loyalNodesSolution[0] << endl;
		}
		if (filePrint)
		{
			out << endl << "SOLUTION FOUND!!!" << endl << "Commander is traitor." << endl <<
				"All loyal nodes took the same decision " << loyalNodesSolution[0] << endl;
		}
		return 1;
	}

}


Network::~Network() {};



