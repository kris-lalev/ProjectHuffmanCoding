#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include  <unordered_map>
using namespace std;

struct huffman_node
{
	char id;																				//character 
	int freq;																				//frequency of the character
	string code;																			//huffman code for the character
	huffman_node* left;
	huffman_node* right;
	huffman_node()
	{//constructer
		left = right = NULL;
	}
};
typedef huffman_node* node_ptr;

class huffman
{
protected:
	node_ptr node_array[256];																//array for 128characters in the Ascii Table
	std::unordered_map<char, node_ptr> letters;      //map for characters in the Ascii Table
	fstream in_file, out_file;
	node_ptr child, parent, root;
	char id;
	string in_file_name, out_file_name;
	class compare
	{//a object funtion to set comparing rule of priority queue
	public:
		bool operator()(const node_ptr& c1, const node_ptr& c2) const
		{
			
			if (c1->freq == c2->freq)
				return c1->id > c2->id;
			return c1->freq > c2->freq;
		}
	};
	priority_queue<node_ptr, vector<node_ptr>, compare> pq;									//priority queue of frequency from high to low
	void create_node_array();
	void traverse(node_ptr, string);														//traverse the huffman tree and get huffman code for a character
	int binary_to_decimal(const string&);															//convert a 8-bit 0/1 string of binary code to a decimal integer 
	string decimal_to_binary(int);															//convert a decimal integer to a 8-bit 0/1 string of binary code
	inline void build_tree(string&, char);													//build the huffman tree according to information from file 

public:
	huffman(string, string);
	void create_pq();
	void create_huffman_tree();
	void calculate_huffman_codes();
	void coding_save();
	void coding_show();
	void decoding_save();
	void recreate_huffman_tree();
	double get_comp_rate(string, string);
	void see_comp_input();
};

#endif
