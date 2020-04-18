#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "utilities.h"
using namespace std;


vector<pair<string, string>> productions;

set<string> search(string symbol) {
	/*If the right side of the production matches the symbol,
		add the left side of the production (variable) to a set*/
	set<string> leftProd;
	vector <pair<string, string>>::iterator productionsItr;
	for (productionsItr = productions.begin(); productionsItr != productions.end(); ++productionsItr) {
		if (productionsItr->second == symbol) {
			leftProd.insert(productionsItr->first);
		}
	}
	return leftProd;
}

/* generalization of the above
   returns a set of symbols that
   can produce a set of symbols
   */
set<string> search(set<string> ss);

void solve(Matrix<set<string>>& m, string input) {
	//Filling up the 1st row
	set<string> leftPr;
	string::iterator inputItr = input.begin();
	for (int j = 0; j < m.getCols(); j++) {
		leftPr= search({ *inputItr });
		m[0][j] = leftPr;
		inputItr++;
	}
}

int main() {
	cout << "Input a string: ";
	string input;
	cin >> input;
	Matrix<set<string>> table(input.length(), input.length()) ;
	
	std::ifstream file;
	file.open("cfg2.txt");
	int np;
	std::string pr;
	std::string left, right;

	/* read the cfg from file "cfg.txt"
		if the grammar is CNF (Assumes starting 
		symbol  is "S")
		*/
	while (file >> pr) {
		if (pr.length() > 5) {
			std::cout << "not cnf\n";
			abort();
		}
		int itr = pr.find("->");
		if (itr != 1) {
			std::cout << "more than one symbol on left\n";
			abort();
		}

		/* read the file and save the productions
			in a vector of pairs. For example
			S->AB is saved as pair("S","AB");
			*/
		left = pr.substr(0, 1);
		right = pr.substr(itr + 2, 2);
		pair<string, string> p(left, right);
		productions.push_back(p);
	}

	solve(table, input);
	file.close();

	/* the startig symbol is assumed
		to be S
		*/
	int j = input.length() - 1;
	if (table[0][j].find("S") != table[0][j].end()) {
		std::cout << input << " belongs to language.\n";
	}
	else
		std::cout << input << " does not belong to language.\n";

}
