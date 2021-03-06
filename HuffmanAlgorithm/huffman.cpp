#include "huffman.h"

 
void print2DUtil(node_ptr root, int space)
{
	
	if (root == NULL) {
		for (int i = 0; i < space; i++)
			cout << " ";
		cout << "NULL" << "\n";
		return;
	}
	space += 5;
	print2DUtil(root->right, space);
	cout << endl;

	for (int i = 5; i < space; i++)
		cout << " ";

	cout << root->id << "||" << root->code << "\n";
	print2DUtil(root->left, space);
}

void print2D(node_ptr root)
{
	print2DUtil(root, 0);
}

void huffman::create_node_array()
{
	for (int i = 0; i < 256; i++)
	{
		node_array[i] = new huffman_node;
		node_array[i]->id = i;
		node_array[i]->freq = 0;
	}
}

void huffman::traverse(node_ptr node, string code)
{
	if (node->left == NULL && node->right == NULL)
	{
		node->code = code;
		//std::cout << "Gave code" << code << "To" << node->id << endl;
	}
	else
	{
		traverse(node->left, code + '0');
		traverse(node->right, code + '1');
	}
}

int huffman::binary_to_decimal(const string& in)
{
	int result = 0;
	for (int i = 0; i < in.size(); i++)
		result = result * 2 + in[i] - '0';
	return result;
}

string huffman::decimal_to_binary(int in)
{
	string temp = "";
	string result = "";
	while (in)
	{
		temp += ('0' + in % 2);
		in /= 2;
	}
	result.append(8 - temp.size(), '0');
	for (int i = temp.size() - 1; i >= 0; i--)
	{
		result += temp[i];
	}
	return result;
}

huffman::huffman(string in, string out)
{
	in_file_name = in;
	out_file_name = out;
	in_file.open(in_file_name, ios::in | ios::binary);
	//out_file.open(in_file_name, ios::in | ios::binary);
	if(!in_file.is_open() /*|| !in_file.is_open()*/)
	{
		
		throw invalid_argument("One or both files do not exist!");
	}

	in_file.close();
	//out_file.close();
	create_node_array();
}

void huffman::create_pq()
{
	in_file.open(in_file_name, ios::in);

	in_file.get(id);
	while (!in_file.eof())
	{
		if (id < 0) {
			in_file.get(id);
			continue;
		}
		node_array[id]->freq++;
		in_file.get(id);
	}
	in_file.close();
	for (int i = 0; i < 256; i++)
	{
		if (node_array[i]->freq)
		{
			pq.push(node_array[i]);
		}
	}
}

void huffman::create_huffman_tree()
{
	priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
	while (temp.size() > 1)
	{
		root = new huffman_node;
		root->id = temp.top()->id;
		root->freq = 0;
		root->left = temp.top();
		root->freq += temp.top()->freq;
		temp.pop();
		root->id += temp.top()->id;
		root->right = temp.top();
		root->freq += temp.top()->freq;
		temp.pop();
		temp.push(root);
	}
}

void huffman::calculate_huffman_codes()
{
	traverse(root, "");
}

void huffman::coding_save()
{
	in_file.open(in_file_name, ios::in);
	out_file.open(out_file_name, ios::out | ios::binary);
	string in = "", s = "";
	//print2D(root);
	in += (char)pq.size();
	priority_queue<node_ptr, vector<node_ptr>, compare> toMap(pq);
	std::unordered_map<char, node_ptr> letters;

	while (!toMap.empty())
	{
		letters[toMap.top()->id] = toMap.top();
		toMap.pop();

	}
	priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);

	while (!temp.empty())
	{
		node_ptr current = temp.top();
		in += current->id;
		s.assign(31 - current->code.size(), '0'); 										
		s += '1';
		s.append(current->code);
		in += (char)binary_to_decimal(s.substr(0, 8));
		for (int i = 0; i < 3; i++)
		{
			s = s.substr(8);
			in += (char)binary_to_decimal(s.substr(0, 8));
		}
		temp.pop();

	}
	s.clear();

	in_file.get(id);
	while (!in_file.eof())
	{
		if (id < 0) {
			in_file.get(id);
			continue;
		}
		s += letters[id]->code;
		while (s.size() > 8)
		{
			in += (char)binary_to_decimal(s.substr(0, 8));
			s = s.substr(8);	
		}
		in_file.get(id);
		

	}

	int count = 8 - s.size();
	if (s.size() < 8)
	{
		s.append(count, '0');
	}
	in += (char)binary_to_decimal(s);														
	in += (char)count;

	out_file.write(in.c_str(), in.size());
	in_file.close();
	out_file.close();
}

void huffman::recreate_huffman_tree()
{
	in_file.open(in_file_name, ios::in | ios::binary);
	unsigned char size;																			
	in_file.read(reinterpret_cast<char*>(&size), 1);
	root = new huffman_node;
	for (int i = 0; i < size; i++)
	{
		char r_id;
		unsigned char h_code_c[4];																
		in_file.read(&r_id, 1);     
		in_file.read(reinterpret_cast<char*>(h_code_c), 4);
		
		string h_code_s = "";
		for (int i = 0; i < 4; i++)
		{										
			h_code_s += decimal_to_binary(h_code_c[i]);
		}
		int j = 0;
		while (h_code_s[j] == '0')
		{
			j++;
		}
		h_code_s = h_code_s.substr(j + 1);
		build_tree(h_code_s, r_id);
	}
	
	in_file.close();
}

void huffman::build_tree(string& path, char r_id)
{
	node_ptr current = root;
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == '0')
		{
			if (current->left == NULL)
				current->left = new huffman_node;
			current = current->left;
		}
		else if (path[i] == '1')
		{
			if (current->right == NULL)
				current->right = new huffman_node;
			current = current->right;
		}
	}
	current->id = r_id;																	//attach id to the leaf
}

void huffman::decoding_save()
{
	in_file.open(in_file_name, ios::in | ios::binary);
	out_file.open(out_file_name, ios::out);

	unsigned char size;																		
	in_file.read(reinterpret_cast<char*>(&size), 1);
	in_file.seekg(-1, ios::end);															
	char count0;
	in_file.read(&count0, 1);
	in_file.seekg(1 + 5 * size, ios::beg);			

	vector<unsigned char> text;
	unsigned char textseg;
	in_file.read(reinterpret_cast<char*>(&textseg), 1);
	while (!in_file.eof())
	{
		text.push_back(textseg);
		in_file.read(reinterpret_cast<char*>(&textseg), 1);
	}

	node_ptr current = root;
	string path;
	for (int i = 0; i < text.size() - 1; i++)
	{
		path = decimal_to_binary(text[i]);
		if (i == text.size() - 2)
			path = path.substr(0, 8 - count0);
		for (int j = 0; j < path.size(); j++)
		{
			
			if (path[j] == '0')
				current = current->left;
			else
				current = current->right;
			if (current->left == NULL && current->right == NULL)
			{
				out_file.put(current->id);
				current = root;
			}
		}
	}
	in_file.close();
	out_file.close();
}

double huffman::get_comp_rate(string orig_file, string comp_file)
{
	out_file.open(orig_file, ios::in);
	in_file.open(comp_file, ios::in | ios::binary);
	
	double orig_size = 0;
	double comp_size = 0;

	if (!in_file.is_open() || !in_file.is_open())
	{

		throw invalid_argument("One or both files do not exist!");
	}

	unsigned char size;																		
	in_file.read(reinterpret_cast<char*>(&size), 1);
	in_file.seekg(-1, ios::end);															
	char count0;
	in_file.read(&count0, 1);
	in_file.seekg(1 + 5 * size, ios::beg);												
	
	vector<unsigned char> text;
	char textseg;
	in_file.read(&textseg, 1);
	while (!in_file.eof())
	{
		text.push_back(textseg);
		in_file.read(&textseg, 1);
	}
	comp_size = text.size();
	

	out_file.read(&textseg, 1);
	while (!out_file.eof())
	{
		text.push_back(textseg);
		out_file.read(&textseg, 1);
	}
	orig_size = text.size();
	out_file.close();
	out_file.close();
	return comp_size / orig_size;
}

void huffman::see_comp_input()
{
	create_pq();
	create_huffman_tree();
	calculate_huffman_codes();
	coding_show();
}

void huffman::coding_show()
{
	in_file.open(in_file_name, ios::in);
	string in = "", s = "";
	//print2D(root);
	in += (char)pq.size();															
	priority_queue<node_ptr, vector<node_ptr>, compare> toMap(pq);
	std::unordered_map<char, node_ptr> letters;

	while (!toMap.empty())
	{
		letters[toMap.top()->id] = toMap.top();
		toMap.pop();

	}
	priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);


	in_file.get(id);
	while (!in_file.eof())
	{
		if (id < 0) {
			in_file.get(id);
			continue;
		}
		s += letters[id]->code;
		while (s.size() > 8)
		{
			cout << binary_to_decimal(s.substr(0, 8)) << " ";
			s = s.substr(8);

		}
		in_file.get(id);
	}

	int count = 8 - s.size();
	if (s.size() < 8)
	{
		s.append(count, '0');
	}
	cout << binary_to_decimal(s.substr(0, 8)) << " ";														
	in_file.close();
	
}