// 2019029089 GAN SHU YI

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

struct Table {
	vector<string> attrNames;
	vector<vector<string>> data;
	vector<vector<string>> attrList;
};

struct Node {
	int iAttr;
	string attrVal;
	int iTree;
	bool isLeaf = false;
	string classLabel;
	vector<int> child;
};

// global var
vector<Node> dTree;
Table init;

Table read_input (string fname) {

	string word;
	string line;
	Table table;
	vector<string> row;
	bool isAttrName = true;

	// open input file
	ifstream fin(fname);

	if (!fin.is_open()) {
		cout << "Input file cannot be opened." << endl;
                exit(1);
	}
	while(fin) {
		getline(fin, line);
		istringstream iss(line);
		
		while (iss >> word) {
			row.push_back(word);
		}

		if (isAttrName) {
			table.attrNames = row;
			isAttrName = false;
		}
		else {
			table.data.push_back(row);
		}
		row.clear();
	}
	fin.close();
	table.data.pop_back();

	//int tmp = table.data.size() - 1;
	//table.data.resize(tmp);
	
/*	cout << "attr names:" << endl;
	for (int i = 0; i < table.attrNames.size(); i++) {
		cout << table.attrNames.at(i) << '\t';
	}
	cout << endl;
	
	cout << "attr data:" << endl;
	for (int i  = 0; i < table.data.size(); i++) {
		vector<string> tmp;
		tmp = table.data.at(i);
		cout << i << '\t';
		for (int j = 0; j < tmp.size(); j++) {
			cout << tmp.at(j) << '\t';
		}
		cout << endl;
	}
*/	

	return table;
}

// checks whether node is leaf
bool isLeaf (Table table) {

	//cout << "in isLeaf" << endl;
	for (int i = 0; i < table.data.size(); i++) {
		if (table.data[0].back() != table.data[i].back()) 
			return false;
	}
	return true;
}

double getInfo (Table table) {
	double temp = 0.0;
	int count = table.data.size();
	map<string, int> classCount;	

	//cout << "in getInfo" << endl;
	//cout << "table data size: " << table.data.size() << endl;
	
	for (int i = 0; i < table.data.size(); i++) {
		//cout << classCount[table.data[i].back()] << endl; 
		classCount[table.data[i].back()]++;
	}

	for (auto iter = classCount.begin(); iter != classCount.end(); iter++) {
		double a = (double)iter->second / count;
		temp += -1.0 * a * log(a) / log(2);
	}
	//cout << "info: " << temp << endl;
	return temp;	
}

double getInfoAttr (Table table, int iAttr) {
	double temp = 0.0;
	int count = table.data.size();
	map<string, vector<int>> attrValMap;

	//cout << "in getInfoAttr" << endl;
	
	for (int i = 0; i < table.data.size(); i++) {
		attrValMap[table.data[i][iAttr]].push_back(i);
	}

	for (auto iter = attrValMap.begin(); iter != attrValMap.end(); iter++) {
		Table next;
		for (int j = 0; j < iter->second.size(); j++) {
			next.data.push_back(table.data[iter->second[j]]);
		}
		int nextCount = next.data.size();

		temp += (double) nextCount / count * getInfo(next);
	}
	//cout << "info attr: " << temp << endl;
	return temp;
}

double getGain (Table table, int iAttr) {
	//cout << "in getGain" << endl;
	double temp = getInfo(table) - getInfoAttr(table, iAttr);
	//cout << "gain: " << temp << endl;
	return temp;
}

double getSplitInfoAttr (Table table, int iAttr) {
	double temp = 0.0;
	int count = table.data.size();
	map<string, vector<int>> attrValMap;
	
	//cout << "in getSplitAttr" << endl;

	for (int i = 0; i < table.data.size(); i++) {
		attrValMap[table.data[i][iAttr]].push_back(i);
	}
	//cout << "1" << endl;

	for (auto iter = attrValMap.begin(); iter != attrValMap.end(); iter++) {
                Table next;
                for (int j = 0; j < iter->second.size(); j++) {
                        next.data.push_back(table.data[iter->second[j]]);
                }
                int nextCount = next.data.size();

                double a = (double) nextCount / count;
		temp += -1.0 * a * log(a) / log(2);
        }
	//cout << "split info attr: " << temp << endl;
        return temp;
}

double getGainRatio (Table table, int iAttr) {
	//cout << "in getGainRatio" << endl;
	double temp = getGain(table, iAttr) / getSplitInfoAttr(table, iAttr);
	//cout << "gain ratio: " << temp << endl;
	return temp;
}

int getSelectAttr (Table table) {
	int iMaxAttr = -1;
	double maxAttrVal = 0.0;
	
	//cout << "in getSelectAttr" << endl;

	// exclude class label
	for (int i = 0; i < init.attrNames.size() - 1; i++) {
		if (maxAttrVal < getGainRatio(table, i)) {
			maxAttrVal = getGainRatio(table, i);
			iMaxAttr = i;
		}
	}
	//cout << "\n\nselected attr: " << iMaxAttr << endl;
	return iMaxAttr;
}

// returns class label of the majority
pair<string, int> getMajClass (Table table) {
	
	//cout << "in getMajClass" << endl;
	
	string majClass;
	int majCount = 0;

	map<string, int> classCount;
	for (int i = 0; i < table.data.size(); i++) {
		classCount[table.data[i].back()]++;
		//cout << table.data[i].back() << endl;
		if (classCount[table.data[i].back()] > majCount) {
			majCount = classCount[table.data[i].back()];
			majClass = table.data[i].back();
			//cout << i << "\tcnt: " << majCount << ", class: " << majClass << endl;
		}
	}
	//cout << "majority class: " << majClass << ", count: " << majCount << endl;
	return {majClass, majCount};
}


void generate (Table table, int iNode) {

/*	cout << "in generate" << endl;
	for (int i  = 0; i < table.data.size(); i++) {
                vector<string> tmp;
                tmp = table.data.at(i);
                cout << i << '\t';
                for (int j = 0; j < tmp.size(); j++) {
                        cout << tmp.at(j) << '\t';
                }
                cout << endl;
        }*/

	if (isLeaf(table) == true) {
		dTree[iNode].isLeaf = true;
		dTree[iNode].classLabel = table.data.back().back();
		return;
	}	

	int iSelectAttr = getSelectAttr(table);
	map<string, vector<int>> attrValMap;

	for (int i = 0; i < table.data.size(); i++) {
		attrValMap[table.data[i][iSelectAttr]].push_back(i);
	}

	/*for (auto const &pair: attrValMap) {
                cout << "{" << pair.first << ": ";
                for (int i = 0; i < pair.second.size(); i++) {
                        cout << pair.second.at(i) << "\t";
                }
                cout << endl;
        }*/

	
	dTree[iNode].iAttr = iSelectAttr;
	pair<string, int> maj = getMajClass(table);
		
	if ((double)maj.second / table.data.size() > 0.8) {
		dTree[iNode].isLeaf = true;
		dTree[iNode].classLabel = maj.first;
		return;
	}

	/*cout << "init: " << endl;
	for (int i = 0; i < init.attrList.size(); i++) {
                vector<string> tmp;
                tmp = init.attrList.at(i);
                for (int j = 0; j < tmp.size(); j++) {
                        cout << tmp.at(j) << '\t';
                }
                cout << endl;
        }*/
	//cout << "init attr list size: " << init.attrList[iSelectAttr].size() << endl;
	
	for (int i = 0; i < init.attrList[iSelectAttr].size(); i++) {
		string attrVal = init.attrList[iSelectAttr][i];
		//cout << i << "\tattrVal: " << attrVal << endl;
		Table nextT;
		vector<int> cand = attrValMap[attrVal];
		//cout << "candi size: " << tmp.size() << endl;
		
		for (int j = 0; j < cand.size(); j++) {
			//cout << tmp.at(j) << '\t';
			nextT.data.push_back(table.data[cand[j]]);
		}
		//cout << endl;
		/*for (int k  = 0; k < nextT.data.size(); k++) {
	                vector<string> tmp;
        	        tmp = nextT.data.at(k);
        	        cout << k << '\t';
        	        for (int l = 0; l < tmp.size(); l++) {
                	        cout << tmp.at(l) << '\t';
               		}
                	cout << endl;
        	}*/


		Node nextN;
		nextN.attrVal = attrVal;
		//cout << "attr value: " << attrVal << endl;
		nextN.iTree = (int)dTree.size();
		//cout << "next node tree i: " << dTree.size() << endl;
		dTree[iNode].child.push_back(nextN.iTree);
		dTree.push_back(nextN);
		//cout << "next table data size: " << nextT.data.size() << endl;
		
		// empty table
		if (nextT.data.size() == 0) {
			//cout << "in if" << endl;
			nextN.isLeaf = true;
			nextN.classLabel = getMajClass(table).first;
			dTree[nextN.iTree] = nextN;
		}
		else {
		/*	cout << "-----generating next table-----" << endl;
			for (int k  = 0; k < nextT.data.size(); k++) {
                	        vector<string> tmp;
                        	tmp = nextT.data.at(k);
                        	cout << k << '\t';
                        	for (int l = 0; l < tmp.size(); l++) {
                                	cout << tmp.at(l) << '\t';
                        	}
                        	cout << endl;
                	}*/
			generate(nextT, nextN.iTree);
		}
	}
}
/*
void printDT(int iNode, string branch) {
	
	//cout << "in printDT" << endl;
	//cout << tree[iNode].isLeaf << endl;	
	if (dTree[iNode].isLeaf == true) {
		cout << branch << "Class: " << dTree[iNode].classLabel << endl;
	}

	for (int i = 0; i < dTree[iNode].child.size(); i++) {
		int iChild = dTree[iNode].child[i];
		string attrName = init.attrNames[dTree[iNode].iAttr];
		string attrVal = dTree[iChild].attrVal;
		string tmp = branch + attrName + " = " + attrVal + ", ";

		printDT(iChild, tmp);
	}
}*/

// generate decision tree
void decisionTree (Table table) {
	init = table;
	Node root;
	root.iTree = 0;
	dTree.push_back(root);

	//cout << "in decisionTree" << endl;

	// extracting attribute values
        init.attrList.resize(init.attrNames.size());
	for (int i = 0; i < init.attrNames.size(); i++) {
                map<string, int> value;
                for (int j = 0; j < init.data.size(); j++) {
			//cout << init.data[j][i] << endl;
                	value.insert(make_pair(init.data[j][i], 1));
		}

                for (auto iter = value.begin(); iter != value.end(); iter++) {
                        init.attrList[i].push_back(iter->first);
                }
        }
/*
        for (int i = 0; i < init.attrList.size(); i++) {
                vector<string> tmp;
                tmp = init.attrList.at(i);
                for (int j = 0; j < tmp.size(); j++) {
                        cout << tmp.at(j) << '\t';
                }
                cout << endl;
        }

*/
	generate(init, 0);
	//printDT(0, "");

	//cout << "decision tree generation: END\n" << endl;
}

// for output
string addTab (vector<string> line) {
	string temp;
	for (int i = 0; i < line.size(); i++) {
		temp += line[i];
		if (i != line.size() - 1)
			temp += '\t';
	}
	return temp;
}

int depthFirstSearch (vector<string>& line, int currentNode) {
	if (dTree[currentNode].isLeaf) {
		return currentNode;
	}

	int iAttr = dTree[currentNode].iAttr;

	for (int i = 0; i < dTree[currentNode].child.size(); i++) {
		int nextNode = dTree[currentNode].child[i];

		if (line[iAttr] == dTree[nextNode].attrVal) {
			return depthFirstSearch(line, nextNode);
		}
	}
	return -1;
}

// predict class label
string predict (vector<string> line) {
	string classLabel;
	int leaf = depthFirstSearch(line, 0);
	if (leaf == -1) {
		return "FAILED";
	}	
	classLabel = dTree[leaf].classLabel;
	return classLabel;
}

void output_result (string fname, Table test, Table train) {
	ofstream fout;
	fout.open(fname);
	if (!fout.is_open()) {
		cout << "Output file cannot be opened." << endl;
		exit(1);
	}
	else {
		string res = addTab(train.attrNames);
		fout << res << endl;

		for (int i = 0; i < test.data.size(); i++) {
			vector<string> out = test.data[i];
			string label = predict(test.data[i]);
			out.push_back(label);
			string res2 = addTab(out);
			fout << res2 << endl;
		}
	}
	fout.close();	
}


int main (int argc, char * argv[]) {
	if (argc != 4) {
		cout << "The number of arguments must be 3." << endl;
                exit(1);
	}

	string train_fin(argv[1]);
	string test_fin(argv[2]);
	string fout(argv[3]);
	
	//cout << train_fin << endl;
	Table train_ = read_input(train_fin);
	//cout << train_.data.size() << endl;

	//cout << test_fin << endl;
	Table test_ = read_input(test_fin);
	//cout << test_.data.size() << endl;

	decisionTree(train_);
	output_result(fout, test_, train_);

	// output 
	return 0;
}
