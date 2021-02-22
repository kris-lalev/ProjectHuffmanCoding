#include <iostream>
#include "huffman.h"
#include  <unordered_map>
//#pragma comment(linker, "/STACK:20000000")
//#pragma comment(linker, "/HEAP:20000000")
using namespace std;


int main(int argc, char* argv[]) {
	/*huffman h(argv[3], argv[5]);
	cout << "The compression rate is " << h.get_comp_rate() <<endl;
	return 0;*/


	/*huffman h(argv[3], argv[5]);
	h.coding_show();
	return 0;*/


	if (!(argv[2][0] == '-') && (argv[2][1] == 'i') && !(argv[2][0] == '-') && (argv[2][1] == 'o'))
	{
		cout << "usage:\n\t HuffmanAlgorithm [-c[ompress] /- d[ecompress]] -i <inputfile>  -o <outputfile>" << endl;
		exit(1);
	}
	if ((argv[1][0] == '-') && (argv[1][1] == 'c'))
	{

		try {
			huffman h(argv[3], argv[5]);
			h.create_pq();
			h.create_huffman_tree();
			h.calculate_huffman_codes();
			h.coding_save();
			cout << "The compression rate is " << h.get_comp_rate(argv[3], argv[5]) << endl;
		}
		catch (invalid_argument& e)
		{
			cerr << e.what() << endl;
			return -1;
		}
	}
	else if ((argv[1][0] == '-') && (argv[1][1] == 'd'))
	{
		try {
		huffman h(argv[3], argv[5]);
		h.recreate_huffman_tree();
		h.decoding_save();
}
		catch (invalid_argument& e)
		{
			cerr << e.what() << endl;
			return -1;
		}
	}
	else {
		cout << "usage:\n\t HuffmanAlgorithm [-c[ompress] /- d[ecompress]] -i <inputfile>  -o <outputfile>" << endl;
		exit(1);
	}

	return 0;
}