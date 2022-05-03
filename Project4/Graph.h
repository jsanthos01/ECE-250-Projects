// Graph Class
// Student Name: Joanna Santhosh
// Student Number: 20839779

#include <vector>
#include <stack>
using namespace std;

// Main Reference is from TA's tutorial - Tutorial #12  
template <typename Type>
class Graph {
private:
    // Node class which holds the data, detects if node has been visited 
    // and has a vector of all the neighbors associated to that node
    struct Node {
        Type data;
        bool isVisited; 
        vector<Node *> neighbors;

        // Node Constructor
        Node(Type data_d, vector<Node *> v = {}) 
        : data(data_d) , isVisited(false), neighbors(v) {}
    };

    // Vector of nodes in the graph
    vector<Node *> node_v;
    
    void topologicalSortHelper(Node *curr_v, stack<Node *> &dependencies){
        // Sets the current node as visited
        curr_v->isVisited = true;

        // goes through the neighbours of the current node and checks if they have been visited
        // if not visited, recursively calls the topologicalSortHelper 
        for (auto x : curr_v->neighbors){
            if (!(x->isVisited)) topologicalSortHelper(x, dependencies);
        }
        
        // Once visited and all the neighbours are checked, the current node is pushed to the stack
        dependencies.push(curr_v);
    }

public:
    // this function add the node into the graph if it is not already added and return the index
    int add_node(Type data_){
        // go through the nodes in the graph and check if the the passed in filename exists
        // if it exists, return the index
        int index = 0;
        while(index < node_v.size()){
            if (node_v[index]->data == data_) return index;
            index++;
        }
        
        // if it does not exist, add it to the graph. 
        node_v.push_back(new Node(data_));
        return node_v.size()-1;
    }

    // this function stores the neighbors of a specific node 
    void add_neighbors(int a, int b) {
        // for the specific node , store all the neighbouring nodes into the neighbor vector
        node_v[a]->neighbors.push_back(node_v[b]);
    }

    void topologicalSort(){
        // stack that will hold all the dependencies in order
        stack<Node *> dependencies_list;

        // If it is not visited, call the topologicalSort helper
        // then add it to the dependencies_list stack

        for (int i = 0; i < node_v.size() ; i++) {
            if (node_v.at(i)->isVisited == false) topologicalSortHelper(node_v.at(i), dependencies_list);
        }

        // Store all the files from the stack onto the vector until the stack is empty
        vector<Type> final_dependencies;
        while (dependencies_list.empty() != true) {
            final_dependencies.push_back(dependencies_list.top()->data);
            dependencies_list.pop();
        }

        //Print the filenames in the vector  
        for (int i = final_dependencies.size() - 1; i >= 0 ; i--){
            std::cout << final_dependencies.at(i) << std::endl;
        }
    }
};
