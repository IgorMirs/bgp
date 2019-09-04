#include "Connectivity_matrix.h"
#include <vector>
#include <iostream>
#include <fstream>


using namespace std;

void Connectivity_matrix::fill_connectivity_matrix(uint32_t value)
{
	uint32_t size = (uint32_t)connect_matrix.size();
	connect_matrix.clear();
	for (uint32_t i = 0; i < size; i++)
	{
		vector<uint32_t> temp;
		for (uint32_t j = 0; j < size; j++)
		{
			temp.push_back(value);
		}
		connect_matrix.push_back(temp);
	}
}

void Connectivity_matrix::connect_matrix_from_file()
{
	ifstream fin("Connectivity.txt");  // for Mac - Users/mirsalikhova/Documents/Igor programming/Blockchain v2/OM_recursion/OM_recursion/OM_recursion/Connectivity.txt");
	if (!fin.is_open()) {
		cout << "Could not open connectivity file!" << endl << endl;
		return;
	}

	//disconnect all nodes in the network
	fill_connectivity_matrix(0);

	uint32_t node1, node2;
	while (fin >> node1 >> node2)
	{
		if (node1 >= connect_matrix.size() || node2 >= connect_matrix.size())
		{
			cout << "Connectivity file is wrong!" << endl;
			cout << "The network connectivity set to the default value." << endl << endl;
			fill_connectivity_matrix(1);
			return;
		}
		connect_matrix[node1][node2] = 1;
		connect_matrix[node2][node1] = 1;
	}
	fin.close();
}

bool Connectivity_matrix::p_regular_check(uint32_t regularity)
{
	//the graph is not regular if the number of verticies and the number of connections are odd at the same time
	if (this->connect_matrix.size() % 2 && regularity % 2)
		return false;

	for (uint32_t i = 0; i < this->connect_matrix.size(); i++) {
		uint32_t conn_quant = 0;  //quantity of connections
		for (uint32_t j = 0; j < this->connect_matrix.size(); j++) {
			if (connect_matrix[i][j] == 1)
				conn_quant++;
		}
		if (conn_quant != regularity)
			return false;
	}
	//put the connection to the random nodes
	/*if (number_of_nodes > 1) {
	 for (uint32_t i = 0; i < number_of_nodes; )
	 {
	 uint32_t R = rand() % this->get_number_of_nodes();
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
	 uint32_t R = rand() % this->get_number_of_nodes();
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
	return true;
}

void Connectivity_matrix::resize_connectivity_matrix(uint32_t size)
{
	//resize the connectivity matrix according to the number of nodes in the network
	connect_matrix.resize(size);
	//put the 1 as a default value
	fill_connectivity_matrix(1);
}

std::vector<uint32_t> Connectivity_matrix::shortest_path(uint32_t src, uint32_t dst)
{
	const uint32_t SIZE = (uint32_t)this->connect_matrix.size();
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
		//    Add the minimum weight to the current weight of the vert
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


Connectivity_matrix::~Connectivity_matrix()
{
}
