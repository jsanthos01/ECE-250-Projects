// #include <iostream>
// #include <fstream>
// #include <cstdlib>
// #include <string>
// #include "Graph.h"


// void create_graph(vector<string> &files) {
// 	// create a graph 
// 	Graph<string> graph;


// 	// Add each file as nodes into the graph
// 	int index = 0;
// 	while(index <files.size()){
// 		// if it includes "#include" in the string, remove it and add_node
// 		// else add name of file to node
// 		if (files.at(index).substr(0, 8) == "#include"){
// 			std::string file = files.at(index).substr(10, files.at(index).length() -11);
// 			graph.add_node(file);
// 		} else {
// 			graph.add_node(files.at(index));
// 		}
// 		index++;
// 	}


// 	index = 0;
	
// 	while(index <files.size()){

// 		// If it is the main file, find its index in the graph
// 		if(files.at(index).length() < 8 || files.at(index).substr(0, 8) != "#include"){
// 			int node_index = graph.add_node(files.at(index));
// 			// add the neighbors (edges) of each node
// 			for(int i = index+1; i < files.size(); i++){
// 				if (files.at(i).substr(0, 8) == "#include"){
// 					std::string file = files.at(i).substr(10, files.at(i).length() -11);
// 					int dependency_index = graph.add_node(file);
// 					graph.add_neighbors(node_index, dependency_index);
// 				} else {
// 					break;
// 				}
// 			}

// 		}
		
// 		index++;
// 	}

// 	// call topological sort to sort the function
// 	graph.topologicalSort();
// }


// int main(int argc, char** argv){
	
// 	// Store all filenames and send it to dependency graph
// 	vector<string> files;

// 	// first input argument is always the program name...
// 	char* fileName =  argv[1];
	
// 	// char* fileName = "inputTest.txt";
	
// 	//open the file
// 	std::ifstream fin(fileName);
	
// 	if(!fin)
// 	{
// 		std::cout<< "File not found" << std::endl;
// 		return EXIT_FAILURE;
// 	}
	
// 	std::string line;
// 	while(std::getline(fin,line))
// 	{
// 		// store all the filenames into the "files" vector
// 		files.push_back(line);
// 	}

// 	// Function to fill the graph with all the filenames in the test files
// 	create_graph(files);
	
// 	return EXIT_SUCCESS;
// }

//ECE 250 Project 4 By Luca Perrone #20846280

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>
using namespace std;
 
template <typename Type>
class DependancyGraph {
private:
//everything up until line 48 is directly taken from turoial #12
    struct Node 
	{
        Type data;
        bool isVisited; 
        vector<Node *> neighbors;

        Node(Type data_, vector<Node *> v = {}) : 
		data(data_) ,
		neighbors(v), 
		isVisited(false){}
    };

    vector<Node*> v;

    void topologicalSortHelper(Node *curr_v, stack<Node*> &ordered_neighbors)
	{
        curr_v->isVisited = true; 
        for (auto i:curr_v->neighbors){
            if (!(i->isVisited)) topologicalSortHelper(i, ordered_neighbors);
        }
        ordered_neighbors.push(curr_v);
    }

public:
    int addVertex(Type data_){

		for (int j{0}; j<v.size(); j++){
			if (v[j]->data == data_){
	 			return j;
			}
		
		}       
        v.push_back(new Node(data_));
        return v.size()-1;
    }

	//this functioin adds the neighbors of a node
	//basically stores the neigbors of a node in the vecotr
    void addNeighbor(int a, int b) {
        v[a]->neighbors.push_back(v[b]);
    }
	

    void topologicalSort(){
        // this is a stack that holds all the neigbors in order to make sure we can sort later
        stack<Node *> ordered_neighbors_list;

        //this will add it to the stack but we first 
		//check if its been visited, and if not then we visit it then add it to the stack
		int j{0};
		while(j<v.size()){
			if (v.at(j)->isVisited != true)
			{
 				topologicalSortHelper(v.at(j), ordered_neighbors_list);
			}
			j++;
		}

        //create a vector
        vector<Type> created_vector;
        //print the values of the nodes onto the stack 
		int i{0};
        while (ordered_neighbors_list.empty() == false) 
		{
            created_vector.push_back(ordered_neighbors_list.top()->data);
            ordered_neighbors_list.pop();
        }
		//this will print out our output but we put it outside the first while loop as it 
		//must print reverse order as otherwise it will print from back to front
		int ind = created_vector.size()-1;
		while(ind>=0)
		{
        std::cout << created_vector.at(ind) << std::endl;
		ind--;
        }
    }
};



int main(int argc, char** argv){

	//variabel to store #include
	string name = "#include";
	
	// Store all filenames and send it to dependency graph
	vector<string> fileNames;
	
	char* fileName = "inputTest.txt";
	
	//open the file
	std::ifstream fin(fileName);
	
	if(!fin)
	{
		std::cout<< "These are not the files your looking for" << std::endl;
		return EXIT_FAILURE;
	}
	
	std::string line;
	while(std::getline(fin,line))
	{
		fileNames.push_back(line);
	}

	// Function to fill the graph with all the filenames in the test files
	DependancyGraph<string> dependancy_graph;
	//first we make a loop that iterates trhough how big our file size has lines
	for (int t{0}; t< fileNames.size(); t++)
	{
		//next we check if the line contains #include
		if(fileNames.at(t).substr(0, 8) !=name||fileNames.at(t).length() < 8)
		{
			//if it doesnt then we loop through and will fill the graph with the files making sure to not include the #include part
			int j{t+1};
			while(j<fileNames.size())
			{
				if (fileNames.at(j).substr(0, 8) == name)
				{
					std::string include_file = fileNames.at(j).substr(10, fileNames.at(j).length() -11);
					int dependency_index = dependancy_graph.addVertex(include_file);
					dependancy_graph.addNeighbor(dependancy_graph.addVertex(fileNames.at(t)), dependency_index);
				} 
				else break;
				j++;
			}
		}
	}

	// This block of code will add each file as nodes t our graph
	for (int i{0}; i< fileNames.size(); i++)
	{
		//check to see if it is an #include line if
		//if its not we can just add it imidiatly
		//if it is then we have to take the #include away before we add it as a node=
		if (fileNames.at(i).substr(0, 8) != name)
		{
			dependancy_graph.addVertex(fileNames.at(i));
		} else 
		{
			std::string include_file = fileNames.at(i).substr(10, fileNames.at(i).length() -11);
			dependancy_graph.addVertex(include_file);
		}
	}
	dependancy_graph.topologicalSort();
	
	return EXIT_SUCCESS;
}
