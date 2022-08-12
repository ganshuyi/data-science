// 2019029089 GAN SHU YI

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

struct Object {
        double x;
        double y;
        int objCnt;
        int cluster;
};

// global vars
int n;
int eps;
int minPts;
vector<Object> objects;
vector<vector<int>> neighbors;
vector<vector<int>> cls;

void read_input (string fname) {
	
	int id;
	double x,y;
	
	// open input file
	ifstream fin(fname);

	if (!fin.is_open()) {
		cout << "Input file cannot be opened." << endl;
		exit(1);
	}
	while (!fin.eof()) {
		fin >> id >> x >> y;
		objects.push_back({x, y, 0, -1});
	}
	objects.pop_back();
	fin.close();
	
	/*
	for (int i = 0; i < objects.size(); i++) {
                        Object tmp = objects.at(i);
			cout << fixed;
                        cout << i << '\t' << tmp.x << "\t" << tmp.y << "\t" << tmp.objCnt;
		        cout << '\t' << tmp.cluster << endl;
                } */
}

double distance(Object & a, Object & b) {
	double tmp = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y- b.y));
	return tmp;
}

void findNeighbors() {
	neighbors.resize(objects.size());

	for (int i = 0; i < (int)objects.size(); i++) {
		for (int j = 0; j < (int)objects.size(); j++) {
			if (i == j) continue;
			double tmp = distance(objects[i], objects[j]);
			if (tmp <= eps) {
				objects[i].objCnt++;
				neighbors[i].push_back(j);
			}
		}
	}
}

bool isCoreObj (int id) {
	return objects[id].objCnt >= minPts;
}	

void depthFirstSearch (int id, int c) {
	objects[id].cluster = c;
	if (!isCoreObj(id)) return;

	for (auto & next : neighbors[id]) {
		if (objects[next].cluster != -1) continue;
		depthFirstSearch(next, c);
	}
}

void dbscan() {
	//cout << "in dbscan" << endl;
	int cId = -1;

	findNeighbors();

	for (int i = 0; i < (int)objects.size(); i++) {
		if (objects[i].cluster != -1) continue;

		if (isCoreObj(i)) {
			depthFirstSearch(i, ++cId);
		}
		else {
			objects[i].cluster = -2;
		}
	}

	cls.resize(cId + 1);
	for (int i = 0; i < (int)objects.size(); i++) {
		if (objects[i].cluster != -2) {
			cls[objects[i].cluster].push_back(i);
		}
	}

	sort(cls.begin(), cls.end(), [&](const vector<int> i, const vector<int> j) {
                return (int)i.size() > (int)j.size();
        });
}

void output_result (string fname) {
	// cout << "in output" << endl;
	if (fname.size() < 4) {
                cout << "Wrong input file name format." << endl;
                exit(1);
        }
	for (int i = 0; i < 4; i++) 
		fname.pop_back();

	ofstream fout;
	for (int i = 0; i < n; i++) {
		string tmp = fname + "_cluster_" + to_string(i) + ".txt";
		//cout << "fout: " << tmp << endl;
		fout.open(tmp);
		for (int j = 0; j < (int)cls[i].size(); j++) {
			fout << cls[i][j] << endl;
		}
		fout.close();
	}
}

int main (int argc,char * argv[]) {
	if (argc != 5) {
		cout << "The number of arguments must be 4." << endl;
		exit(1);
	}

	string fin(argv[1]);
	string n_str(argv[2]);
	string eps_str(argv[3]);
	string min_pts_str(argv[4]);
	
	read_input(fin);
	
	n = stoi(n_str);
        eps = stoi(eps_str);
        minPts = stoi(min_pts_str);
	
	dbscan();

	output_result(fin);

	return 0;
}
