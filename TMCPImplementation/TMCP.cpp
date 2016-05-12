#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <string.h>
#include <cstdlib>

using namespace std;

#define MAX 1010
#define RANGE 105
#define NCHANNELS 8
#define logon 0


typedef struct {
	vector<int> conn;
	int id;
	vector<int> p;
	int c;
	int i;
	int x,y,z;
	int nexthop;
} node;

typedef struct {
	node nodes[MAX];
	int d[MAX];
	int s[MAX];
} graph;

int compare(const void* a, const void* b) {
	node n1 = *(node*)a;
	node n2 = *(node*)b;
	return n1.p.size() - n2.p.size();
}


void initGraph(graph &g) {
	for(int i = 0; i < MAX; i++) {
		g.nodes[i].p.clear();
		g.nodes[i].c = -1;
		g.nodes[i].i = 0;
		g.nodes[i].x = g.nodes[i].y = g.nodes[i].z = 0;
		g.nodes[i].conn.clear();
		g.d[i] = g.s[i] = 0;
		g.nodes[i].nexthop = 0;
	}
}

void insertNode(graph &g, int id, int x, int y, int z) {
	g.nodes[id].x = x;
	g.nodes[id].y = y;
	g.nodes[id].z = z;
	g.nodes[id].id = id;
}

double calculateDistance(node src, node dst) {
	double dist = sqrt(pow((src.x-dst.x),2.0) + pow((src.y-dst.y),2.0) + pow((src.z-dst.z),2));
	return dist;
}

void insertConnection(graph &g, int src, int dst) {
	g.nodes[src].conn.push_back(dst);
}

bool hasParentinTree(vector<int> parents, vector<int> tree) {
	for(int p = 0; p < parents.size(); p++) {
		int id = parents[p];
		for(int i = 0; i < tree.size(); i++) {
			if(id == tree[i]) return true;
		}
	}
	return false;
}

int calculateInterference(node n, vector<int> tree) {
	int cont = 0;
	for(int i = 0; i < n.conn.size(); i++) {
		for(int j = 0; j < tree.size(); j++) {
			if(n.conn[i] == tree[j]) cont++;
		}
	}
	return cont;
}

bool isParent(node n, int p) {
	for(int i = 0; i < n.p.size(); i++) {
		if(p == n.p[i]) return true;
	}
	return false;
}
//BFS from the sink node (node 0)
void BFS(graph &g) {
	deque<int> fila;
	g.d[0] = 1;
	g.s[0] = 0;
	fila.push_front(0);
	while(fila.size() > 0) {
		int x = fila.back();
		fila.pop_back();
		for(int i = 0; i < g.nodes[x].conn.size(); i++) {
			if(!g.d[g.nodes[x].conn[i]]) {
				g.d[g.nodes[x].conn[i]]=1;
				fila.push_front(g.nodes[x].conn[i]);
				g.nodes[g.nodes[x].conn[i]].p.push_back(x);
				g.s[g.nodes[x].conn[i]] = g.s[x]+1;
			}
			else { //search the shortest path
				if(g.s[g.nodes[x].conn[i]] > g.s[x]+1) {
					g.s[g.nodes[x].conn[i]] = g.s[x]+1; //update distance
					g.nodes[g.nodes[x].conn[i]].p.clear();
					g.nodes[g.nodes[x].conn[i]].p.push_back(x); //update parent
				}
				else if(g.s[g.nodes[x].conn[i]] == g.s[x]+1) {
					g.nodes[g.nodes[x].conn[i]].p.push_back(x); //add new parent
				}
			}
		}
	}
}

int main() {
	int N; //number of nodes
	int x,y,z; //to read the position
	graph g; //graph that models the network
	vector<int> trees[NCHANNELS];	
	initGraph(g);
	
	cin >> N;
	
	cin >> x >> y >> z; //read the position of the sink node
	insertNode(g, 0, x, y, z);
	
	for(int i = 1; i < N; i++) {
		cin >> x >> y >> z; //read the position of the node i
		insertNode(g, i, x, y, z);
	}
	
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(i == j) continue;
			if(calculateDistance(g.nodes[i], g.nodes[j]) <= RANGE) {
				insertConnection(g,i,j);
				if(logon) { 
					cout << "Distance: " << calculateDistance(g.nodes[i], g.nodes[j]) << " - "; 
					if(i == 0)
						cout << "Inserting conecction between " << i << " and " << j+1 << endl;
					else if(j == 0)
						cout << "Inserting conecction between " << i+1 << " and " << j << endl;
					else
						cout << "Inserting conecction between " << i+1 << " and " << j+1 << endl;
				}
					
				//g.nodes[j].i++; //one more interference in node j
			}
		}
	}
	
	BFS(g); //perform a search to find parents and define the level of the nodes in the tree
	for(int i = 0; i < NCHANNELS; i++) {
		trees[i].push_back(0); //add the sink node in all trees
	}
	int level = 1; //init the analyzes from the nodes nearest the sink
	int h = 0;
	for(int i = 0; i < N; i++) { //finding the height of the tree
		if(h < g.s[i]) h = g.s[i];
	}
	int k=0;
	do {
		node list[MAX];
		k=0;
		for(int i = 0; i < N; i++) { //creating the list of nodes of the current level that are without channel
			if(g.s[i] == level && g.nodes[i].c == -1) {
				list[k] = g.nodes[i];
				k++;
			}
		}
		qsort(list,k, sizeof(node), compare); //sort the list according to the number of parents
		int mininter = N+1;
		int besttree = 0;
		for(int i = 0; i < k; i++) { //for each node in the list
			if(logon) cout << "Defining channel for node " << list[i].id << endl;
			mininter = N+1;
			besttree = 0;
			for(int t = 0; t < NCHANNELS ; t++) { //for each tree
				if(logon) cout << "Analyzing tree " << t << endl;
				//the node has parent in this tree?
				bool hasParent = hasParentinTree(list[i].p, trees[t]);
				//if yes: what is the interference this node causes?
				if(hasParent) {
					int interf = calculateInterference(list[i], trees[t]);
					if(logon) cout << "Interference in the tree: " << interf << endl;
					//minimize interference (define the best tree)
					if(mininter > interf) {
						mininter = interf;
						besttree = t;
					}
					else if(mininter == interf) {
						if(trees[besttree].size() > trees[t].size()) {
							mininter = interf;
							besttree = t;
						}
					}
				}
			}
			int bestparent = 0;
			int minparent = N+1;
			//inside the best tree, what is the parent with less connections in the final tree?
			for(int j = 0; j < trees[besttree].size(); j++) {
				if(isParent(list[i],trees[besttree][j])) {
					if(logon) cout << "Interference of " << trees[besttree][j] << ": " << g.nodes[trees[besttree][j]].i << endl;
					if(minparent > g.nodes[trees[besttree][j]].i) {
						minparent = g.nodes[trees[besttree][j]].i;
						bestparent = trees[besttree][j];
					}
				}
			}
			if(logon) cout << "Node " << list[i].id << " with parent: " << bestparent << " and channel: " << besttree << endl;
			//define channel and parent
			g.nodes[list[i].id].nexthop = bestparent;
			g.nodes[list[i].id].c = besttree;
			g.nodes[bestparent].i++; //one more interference in this parent
			trees[besttree].push_back(list[i].id); //add node in the tree
		}	
		level++;
	}while(level <= h);
	
	if(logon) { 
		for(int i = 0; i < N; i++) {
			cout << g.nodes[i].id << " " << g.s[g.nodes[i].id] << " " << g.nodes[i].i << endl;
			for(int j = 0; j < g.nodes[i].p.size(); j++) {
				cout << g.nodes[i].p[j] << " ";
			}
			cout << endl;
			cout << "Next hop: " << g.nodes[i].nexthop << endl;
			cout << "Channel: " << g.nodes[i].c << endl;
		
		}
	}
	
	cout << "# Coordenadas dos sink nodes" << endl;
	for(int i = 0; i < NCHANNELS; i++) {
		cout << "SN.node["<<i<<"].xCoor = " << g.nodes[0].x <<endl;
		cout << "SN.node["<<i<<"].yCoor = " << g.nodes[0].y <<endl;
		cout << "SN.node["<<i<<"].zCoor = " << g.nodes[0].z <<endl;
		cout << endl;
	}
	cout << endl;
	cout << "# Coordenadas dos end nodes/routers" << endl;
	
	for(int i = 1; i < N; i++) {
		cout << "SN.node["<<i+NCHANNELS-1<<"].xCoor = " << g.nodes[i].x <<endl;
		cout << "SN.node["<<i+NCHANNELS-1<<"].yCoor = " << g.nodes[i].y <<endl;
		cout << "SN.node["<<i+NCHANNELS-1<<"].zCoor = " << g.nodes[i].z <<endl;
		cout << endl;
	}
	cout << endl;
	cout << "# Frequência (canal) dos sink nodes" << endl;
	for(int i = 0; i < NCHANNELS; i++) {
		cout << "SN.node["<<i<<"].Communication.Radio.carrierFreq = " << i*5+2405.0 << endl;
	}
	cout << endl;
	cout << "# Frequência (canal) dos end nodes/routers" << endl;
	for(int i = 1; i < N; i++) {
		cout << "SN.node["<<i+NCHANNELS-1<<"].Communication.Radio.carrierFreq = " << g.nodes[i].c*5+2405.0 << endl;
	}
	cout << endl;
	cout << "#Matriz de conexões " << endl; ;
	cout << "int nchannels = " << NCHANNELS << ";"<<endl;
	cout << "int matriz["<<N+NCHANNELS+1<<"]["<<N+NCHANNELS+1<<"] = {";
	
	//sink nodes don't have parents
	for(int i = 0; i < NCHANNELS; i++) {
		cout << "{";
		for(int j = 0; j < N+NCHANNELS+1; j++) {
			if(j != 0) cout << ",";
			cout << " 0";
		}
		cout << "},";
		cout << endl;
	}
	for(int i = NCHANNELS; i < N+NCHANNELS+1; i++) {
		cout << "{";
		for(int j = 0; j < N+2; j++) {
			if(j == 0) {
				for(int a = 0; a < NCHANNELS; a++) {
					if(a != 0) cout <<",";
					if(g.nodes[i-NCHANNELS+1].c == a && g.nodes[i-NCHANNELS+1].nexthop == j) {
						cout << " 1";
					}
					else {
						cout << " 0";
					}
				}
			}
			else if(g.nodes[i-NCHANNELS+1].nexthop == j) {
				cout << ", 1";
			}
			else {
				cout << ", 0";
			}
		}
		if(i < N+NCHANNELS) cout << "},";
		else cout << "}};";
		cout << endl;
	}
	return 0;
}