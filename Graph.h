#include <vector>
using namespace std;

class Node{

private:
	char label;
	vector<Node*> friends;

public:
	Node(char);
	void AddFriend(Node*);
	vector<Node*> GetFriends();
	Node* GetFriend(int);
	char GetLabel();
	vector<char> GetFriendsLabels();
	~Node();
};


class Graph{

private:
	vector<Node*> allNodes;

public:
	Node* GetRoot();
	Node* AddNode(char);
	void AddEdge(Node*, Node*);
	void DoBFS(Node*);
	Node* GetNodeFromLabel(char);
	Node* GetNodeFromIndex(int);
	vector<char> GetAllLabels();
	int GetSize();
	~Graph();
};