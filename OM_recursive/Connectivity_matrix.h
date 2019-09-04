#include <vector>
#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

class Connectivity_matrix
{
	void fill_connectivity_matrix(uint32_t value);

public:
	std::vector<std::vector<uint32_t>>  connect_matrix;  //connectivity matrix to detect connectivity of each node

	void connect_matrix_from_file(); //upload connectivity matrix from file
	bool p_regular_check(uint32_t regularity);                //checking is the network p-regular
	void resize_connectivity_matrix(uint32_t size);
	std::vector<uint32_t> shortest_path(uint32_t src, uint32_t dst); //define the shortest path for sending messages

	~Connectivity_matrix();
};

#endif // CONNECTIVITY_H
