// 2019029089 GAN SHU YI

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

int iter = 0; // keep track of current iter
int minSup = 0; // minimum support
vector<vector<int>> trans, C, L; // sorted transactions, candidate & freq dataset
vector<tuple<vector<int>, vector<int>, float, float>> assocRules; // association rules


vector<vector<int>> read_input (string fname) {
	
	string line;
	int num;
	vector<int> arr;
	vector<vector<int>> _trans;

	// open input file
	ifstream fin(fname);

	if (!fin.is_open()) {
		cout << "Input file cannot be opened." << endl;
		exit(1);
	}
	else {
		while(fin) {
			getline(fin, line);
			stringstream iss(line);
			
			while (iss >> num) {
				arr.push_back(num);
			}
			_trans.push_back(arr);
			arr.clear();
		}
		fin.close();
	}
	_trans.pop_back();
/*
	cout << _trans.size() << endl;
	cout << "transactions:" << endl;
	for (int i = 0; i < _trans.size(); i++) {
		vector<int> temp;
		temp = _trans.at(i);
		for (int j = 0; j < temp.size(); j++) {
			cout << temp.at(j) << '\t';
		}
		cout << endl;
	}
*/	
	return _trans;
}

// returns unique itemset
vector<int> getElement(vector<vector<int>> itemSet) {
	vector<int> element;
	set<int> s;
	for (auto&row:itemSet) {
		for(auto&col:row) {
			s.insert(col);
		}
	}
	for (auto i = s.begin(); i != s.end(); i++)
		element.push_back(*i);

	return element;
}

// generates (k+1) itemset from k-itemsets by joining each item with itself
vector<vector<int>> join() {
	vector<vector<int>> temp;

	for (int i = 0; i < L.size(); i++) {
		for (int j = i + 1; j < L.size(); j++) {
			int k;
			for (k = 0; k < iter-1; k++) {
				if (L[i][k] != L[j][k]) 
					break;
			}
			if (k == iter-1) {
				vector<int> temp2;
				for (int m = 0; m < iter-1; m++) {
					temp2.push_back(L[i][m]);
				}
				int a = L[i][iter-1];
				int b = L[j][iter-1];
				if (a > b) swap(a, b);
				temp2.push_back(a), temp2.push_back(b);
				temp.push_back(temp2);
			}
		}
	}
	/*
	cout << "in join, result: " << endl;
	for (int i = 0; i < temp.size(); i++) {
                        vector<int> tmp;
                        tmp = temp.at(i);
                        for (int j = 0; j < tmp.size(); j++) {
                                cout << tmp.at(j) << '\t';
                        }
                cout << endl;
	}
	*/
	return temp;
}

// remove infrequent candidate items 
vector<vector<int>> prune (vector<vector<int>> join_res) {
	vector<vector<int>> temp;
	set<vector<int>> tmpSet;
	for (auto&row:L) tmpSet.insert(row);

	for (auto&row:join_res) {
		int i;
		for (i = 0; i < row.size(); i++) {
			vector<int> temp2 = row;
			temp2.erase(temp2.begin() + i);
			if (tmpSet.find(temp2) == tmpSet.end()) break;
		}
		if (i == row.size()) {
			temp.push_back(row);
		}
	}
	return temp;
}

// returns the next candidate itemset 
vector<vector<int>> makeNextC() {
	if (iter == 0) {
		vector<vector<int>> temp;
		vector<int> element = getElement(trans);
		for (auto&i:element) temp.push_back(vector<int>(1, i));

		return temp;
	}
	else {
		return prune(join());
	}
}

float calcSupport(vector<int> item) {
	int temp = 0;
	
	for (auto&row:trans) {
		int i, j;
		if (row.size() < item.size()) continue;
		for (i = 0, j = 0; i < row.size(); i++) {
			if (j == item.size()) break;
			if (row[i] == item[j]) j++;
		}
		if(j == item.size()) {
			temp++;
		}
	}

	//cout << "support: " << temp << endl;
	float val = (float) temp / trans.size() * 100.0;
	
	return val;
}

// returns next frequent itemset
vector<vector<int>> makeNextL() {
	vector<vector<int>> temp;
	
	for (auto&row:C) {
		float sup = calcSupport(row);
		if (sup < minSup) continue;
		temp.push_back(row);
	}
	return temp;
}

// return association rule
void makeAssocRule (vector<int> items, vector<int> A, vector<int> B, int index) {
	if (index == items.size()) {
		if (A.size() == 0 || B.size() == 0) return;
		float ABsup = calcSupport(items);
		float Asup = calcSupport(A);

		if (Asup == 0) return;

		float sup = ABsup;
		float conf = ABsup / Asup * 100.0;
		assocRules.push_back({A, B, sup, conf});
		return;
	}

	A.push_back(items[index]);
	makeAssocRule(items, A, B, index + 1);
	A.pop_back();
	B.push_back(items[index]);
	makeAssocRule(items, A, B, index + 1);
}

// apriori algorithm
void apriori (vector<vector<int>> _trans) {
	vector<vector<vector<int>>> freqSet;
	trans.clear();

	for (auto&row:_trans) {
		sort(row.begin(), row.end());
		trans.push_back(row);
	}
	freqSet.push_back({{}});

	while(1) {
		C = makeNextC();
		/*cout << "C after makeNextC():" << endl;
		for (int i = 0; i < C.size(); i++) {
                        vector<int> temp;
                        temp = C.at(i);
                        for (int j = 0; j < temp.size(); j++) {
                                cout << temp.at(j) << '\t';
                        }
                	cout << endl;
		}
		cout << "size of C: " << C.size() << endl; */
		if (C.size() == 0) break;

		// cout << "in apriori, iter #" << iter << endl;
		iter++;

		L = makeNextL();
		freqSet.push_back(L);
		/*
		cout << "L:" << endl;
        	for (int i = 0; i < L.size(); i++) {
                	vector<int> temp;
                	temp = L.at(i);
                	for (int j = 0; j < temp.size(); j++) {
                	        cout << temp.at(j) << '\t';
                	}
                cout << endl;
        	}*/

	}

	for(auto&kItemSet:freqSet) {
		for(auto&items:kItemSet) {
			makeAssocRule(items, {}, {}, 0);
		}
	}

}

// convert vector to string format
string vectToStr (vector<int> vect) {
	string temp = "{";
	for (int i = 0; i < vect.size(); i++) {
		temp += to_string(vect[i]);
		if (i != vect.size() - 1)
			temp += ",";
	}
	temp += "}";
	return temp;
}

void output_result (string fname) {
	ofstream fout;
	fout.open(fname);
	if (!fout.is_open()) {
		cout << "Output file cannot be opened." << endl;
                exit(1);
	}
	else { 
		for (auto&i:assocRules) {
			fout << vectToStr(get<0>(i)) << "\t";
			fout << vectToStr(get<1>(i)) << "\t";
			
			fout << fixed;
			fout.precision(2);
			fout << get<2>(i) << "\t";
			
			fout.precision(2);
			fout << get<3>(i) << endl;
		}
	
		fout.close();
	}
}


int main (int argc, char * argv[]) {
	if (argc != 4) {
		cout << "The number of arguments must be 3." << endl;
		exit(1);
	}
	
	string min_sup_str(argv[1]);
	string fin(argv[2]);
	string fout(argv[3]);
	
	trans = read_input(fin);
	minSup = stoi(min_sup_str);
	//cout << minSup << endl;
	
	apriori(trans);
	output_result (fout);

	return 0;
}
