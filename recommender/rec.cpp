// 2019029089 GAN SHU YI

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Entry {
	int user;
	int item;
	int rating;
};

// global vars
int maxItem = 0;
int maxUser = 0;
int itemSize, userSize;
vector<vector<double>> item_item, user_item;
double rmse = 0.0;

vector<Entry> read_input (string fname) {

	int user, item, rating, time_stamp;
	vector<Entry> input;

	// open input file
	ifstream fin(fname);

	if (!fin.is_open()) {
		cout << "Input file cannot be opened." << endl;
		exit(1);
	}
	else {
		while (!fin.eof()) {
			fin >> user >> item >> rating >> time_stamp;
			input.push_back({user, item, rating});

			maxUser = max(user, maxUser);
			maxItem = max(item, maxItem);
		}
		fin.close();
		input.pop_back();
		
		/* cout << "user: " << maxUser << "\nitem: " << maxItem << endl;
		cout << "size: " << input.size() << endl;
		for (int i = 0; i < input.size(); i++) {
			Entry tmp = input.at(i);
			cout << i << '\t' << tmp.user << '\t' << tmp.item << '\t' << rating << endl;
		} */	
	}
	return input;
}

void get_rm (vector<Entry> input) {
	
	int user, item, rating;

	user_item = vector<vector<double>> (maxUser + 1, vector<double> (maxItem + 1, -1.0));
        for (int i = 0; (unsigned)i < input.size(); i++) {
		user = input[i].user;
                item = input[i].item;
                rating = input[i].rating;
                user_item[user][item] = rating;
        }
}

void calc_similarity() {
	
	userSize = (int)user_item.size() - 1;
	itemSize = (int)user_item.back().size() - 1;
	
	item_item = vector<vector<double>> (itemSize + 1, vector<double> (itemSize + 1));
	
	for (int i = 1; i <= itemSize; i++) {
		for (int j = 1; j <= itemSize; j++) {
			double a = 0;
			double bL = 0;
			double bR = 0;
			int count = 0;

			for (int k = 1; k <= userSize; k++) {
				if (user_item[k][i] == -1.0) continue;
				if (user_item[k][j] == -1.0) continue;

				count++;
				a += user_item[k][i] * user_item[k][j];
				bR += user_item[k][i] * user_item[k][i];
				bL += user_item[k][j] * user_item[k][j];
			}

			if (count < 1) {
				item_item[i][j] = 0;
			} else {
				item_item[i][j] = a / (sqrt(bL) * sqrt(bR));
			}
		}
	}
}

// predict movie rating
int gen_rating (int user, int item) {
	
	double a = 0.0;
	double b = 0.0;
	
	if (item > itemSize) {
		return 3;
	}

	for (int i = 1; i<= itemSize; i++) {
		if (user_item[user][i] == -1.0) continue;

		b += abs(item_item[item][i]);
		a += item_item[item][i] * user_item[user][i];
	}

	if (b == 0) {
		return 2;
	}

	double rating = a / b;
	return rating + 0.5;
}


void output_result (string fname, vector<Entry> test_input) {
	
	// open output file
	ofstream fout;
	fout.open(fname);
	if (!fout.is_open()) {
		cout << "Output file cannot be opened." << endl;
		exit(1);
	}
	else {
	        for (int i = 0; (unsigned)i < test_input.size(); i++) {
        	        int user = test_input[i].user;
        	        int item = test_input[i].item;
        	        int rating = test_input[i].rating;
	
        	        double pred = gen_rating(user, item);
	
        	        rmse += (pred - rating) * (pred - rating);
        	        fout << user << '\t' << item<< '\t' << pred << endl;
        	}
	}
	fout.close();
}

int main (int argc, char * argv[]) {
	if (argc != 3) {
		cout << "The number of arguments must be 2." << endl;
		exit(1);
	}

	string train_fin(argv[1]);
	string test_fin(argv[2]);
	
	// read training file
	vector<Entry> train_input = read_input(train_fin);
	
	// rating matrix
	get_rm(train_input);

	// read test file
	vector<Entry> test_input = read_input(test_fin);
	
	// calculate similarity between items
	calc_similarity();
	
	// generate output file name 
	string fout = test_fin.substr(0, 2) + ".base_prediction.txt";
	//cout << fout << endl;	
	
	// predict & print output
	output_result(fout, test_input);
	
	// check error
	rmse /= (double)test_input.size();
	rmse = sqrt(rmse);
	cout << "RMSE: " << rmse << endl;

	return 0;
}
