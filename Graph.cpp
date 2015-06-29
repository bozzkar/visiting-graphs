
#include <iostream>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include "Graph.h"

using namespace std;


Node::Node(char x){
	this->label = x;
}

void Node::AddFriend(Node* friendNode){
	this->friends.push_back(friendNode);
}

vector<Node*> Node::GetFriends(){
	return this->friends;
}

Node* Node::GetFriend(int i){
	return friends[i];
}

char Node::GetLabel(){
	return this->label;
}

vector<char> Node::GetFriendsLabels(){
	vector<char> friendsLabels;
	for (Node* eachNode : this->GetFriends()){
		friendsLabels.push_back(eachNode->GetLabel());
	}
	return friendsLabels;
}

Node::~Node(){

}

Node* Graph::GetRoot(){
	if (this!=NULL && this->allNodes.size() > 0)
		return this->allNodes[0];
	else
		return NULL;
}

Node* Graph::AddNode(char label){
	Node* X = new Node(label);
	this->allNodes.push_back(X);
	cout << "Adding node:" << label << endl;
	return X;
}

void Graph::AddEdge(Node* X, Node* Y){
	X->AddFriend(Y);
	Y->AddFriend(X);
	cout << "Adding Edge:" << X->GetLabel() << "--" << Y->GetLabel() <<endl;
}

void Graph::DoBFS(Node* root){
	cout << endl;
	cout << "Performing Breadth First Search"<<endl;
	//cout << "--------------------------------" << endl;
	cout << "Visiting nodes:";
	if (root == NULL)
		return;
	queue<Node*> q;
	Node* friendNode;
	unordered_map<Node*, bool> visited;
	visited[root] = true;
	q.push(root);

	while (!q.empty()){
		Node* current = q.front();
		q.pop();
		cout <<current->GetLabel() << "->";

		for (int i = 0; i < current->GetFriends().size(); i++){
			friendNode = current->GetFriend(i);
			if (!visited[friendNode]){
				visited[friendNode] = true;
				q.push(friendNode);
			}

		}
	}
	cout << endl;
}

Node* Graph::GetNodeFromLabel(char label){
	for (int i = 0; i < this->allNodes.size(); ++i){
		if (this->allNodes[i]->GetLabel()==label)
			return allNodes[i];
	}
	return NULL;
}

Node* Graph::GetNodeFromIndex(int index){
	if (this->allNodes[index] != NULL)
		return this->allNodes[index];
	else
		return NULL;
}

int Graph::GetSize(){
	return this->allNodes.size();
}

vector<char> Graph::GetAllLabels(){
	vector<char> allLabels;
	for (Node* eachNode : this->allNodes){
		allLabels.push_back(eachNode->GetLabel());
	}
	return allLabels;
}

Graph::~Graph(){
	cout << "Deleting Graph" << endl;
	//cout << "--------------" << endl;
	//cout << "Deleting nodes: ";
	for (int i = 0; i < this->allNodes.size(); ++i){
		//cout <<this -> allNodes[i]->GetLabel()<<",";
		delete this->allNodes[i];
	}
}

Graph* CloneGraph(Graph* origGraph){
	cout <<endl<< "Cloning graph" << endl;
	//cout << "-------------" << endl;

	Node* root = origGraph->GetRoot();
	Graph* clonedGraph = new Graph();
	if (root == NULL)
		return NULL;
	queue<Node*> q;
	unordered_map<Node*, Node*> mapping;
	Node* friendNode;

	Node* clonedRoot = clonedGraph->AddNode(root->GetLabel());
	//cout << "Cloned new Nodes:" << clonedRoot->GetLabel() << ",";
	mapping[root] = clonedRoot;
	q.push(root);

	while (!q.empty()){
		Node* current = q.front();
		q.pop();

		for (int i = 0; i < current->GetFriends().size(); i++){
			friendNode = current->GetFriend(i);
			if (mapping[friendNode]){
				clonedGraph->AddEdge(mapping[current], mapping[friendNode]);
			}
			else{
				Node* newNode = clonedGraph->AddNode(friendNode->GetLabel());
				//cout << newNode->GetLabel()<<",";
				mapping[friendNode] = newNode;
				clonedGraph->AddEdge(mapping[current], newNode);
				q.push(friendNode);
			}

		}
	}
	return clonedGraph;
}

bool AreIdenticalGraphs(Graph* g1, Graph* g2){

	// Graphs are not same if number of nodes differ
	if (g1->GetSize() != g2->GetSize())
		return false;

	// Graphs are not same if a node is absent in the other graph
	vector<char> g1Nodes = g1->GetAllLabels();
	vector<char> g2Nodes = g2->GetAllLabels();
	for (char eachNode : g1Nodes){
		if (find(g2Nodes.begin(), g2Nodes.end(), eachNode) == g2Nodes.end())
			return false;
	}
	
	// Check for friends
	for (int i = 0; i < g1->GetSize(); i++){
		
		// Graphs are not same if the friends of identical nodes differ
		char currentg1Node = g1->GetNodeFromIndex(i)->GetLabel();
		vector<char> g1FriendNodes = g1->GetNodeFromIndex(i)->GetFriendsLabels();
		vector<char> g2FriendNodes = g2->GetNodeFromLabel(currentg1Node)->GetFriendsLabels();
		
		for (char eachNode : g1FriendNodes){
			if (find(g2FriendNodes.begin(), g2FriendNodes.end(), eachNode) == g2FriendNodes.end())
				return false;
		}
		for (char eachNode : g2FriendNodes){
			if (find(g1FriendNodes.begin(), g1FriendNodes.end(), eachNode) == g1FriendNodes.end())
				return false;
		}

	}

	// Atlast.. phew!
	return true;
}

void RunTestCase(vector<char> nodes,vector<pair<char,char>> edges){
	if (nodes.size() == 0)
	{
		cout << "Nothing to create" << endl;
		cout << "Test case result: " << "FAIL" << endl;
		return;
	}
	cout << endl;
	string result;
	Graph *origGraph, *clonedGraph;


	cout << "Creating Original Graph" << endl;
	origGraph = new Graph();
	for (char label : nodes){
		origGraph->AddNode(label);
	}
	
	for (pair<char,char> edge : edges){
		origGraph->AddEdge(origGraph->GetNodeFromLabel(edge.first), origGraph->GetNodeFromLabel(edge.second));
	}
	
	cout << endl;// << "Traversing original graph" << endl;
	origGraph->DoBFS(origGraph->GetRoot());

	clonedGraph = CloneGraph(origGraph);
	

	cout << endl;// << "Traversing cloned graph" << endl;
	clonedGraph->DoBFS(clonedGraph->GetRoot());
	
	cout <<endl<< "Checking if cloned graph is identical to its parent" << endl;
	result = (AreIdenticalGraphs(origGraph, clonedGraph) == 1) ? "PASS" : "FAIL";
	cout << "Test case result: " << result << endl;
	delete clonedGraph;
	delete origGraph;

}


int main(int argc,char** argv){
	
	if (argc != 2){
		cout << "Please supply the test case file" << endl;
		cout << "Format: ./a.out testcase_file" << endl;
		return -1;
	}

	int count = 1;
	int testcaseCount = 0;
	int nodesCount = 0;
	int edgesCount = 0;
	vector<char> nodes;
	vector<pair<char,char>> edges;
	char label, edgePoint1, edgePoint2;
	ifstream inputFile(argv[1],ios::in);

	if (inputFile.is_open()){
		inputFile >> testcaseCount;
		while (testcaseCount>0){
			cout << endl;
			inputFile >> nodesCount;
			inputFile >> edgesCount;
			while (nodesCount > 0){
				inputFile >> label;
				nodes.push_back(label);
				--nodesCount;
			}
			while (edgesCount > 0){
				inputFile >> edgePoint1>>edgePoint2;
				edges.push_back(make_pair(edgePoint1,edgePoint2));
				--edgesCount;
			}
			cout << "------------------" << endl;
			cout << "Running test case:" << count++<<endl;
			cout << "------------------" << endl;
			RunTestCase(nodes,edges);
			cout << endl;
			nodes.clear();
			edges.clear();
			--testcaseCount;
		}
		inputFile.close();
	}
	else{
		cout << "Error opening test case file" << endl;
		return -1;
	}
	
	return 0;
}
