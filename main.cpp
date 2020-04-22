#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "utilities.h"
using namespace std;


vector<pair<string, string>> productions;
set<string> emptySet{ " " };

//Given a symbol "terminal" returns a set 
//	of all symbols that can produce it
set<string> search(string terminal) {
	set<string> leftProductionRule;
	vector <pair<string, string>>::iterator productionsItr;

	for (productionsItr = productions.begin(); productionsItr != productions.end(); ++productionsItr) {
		if (productionsItr->second == terminal) {
			leftProductionRule.insert(productionsItr->first);
		}
	}
	return leftProductionRule;
}

//Generalization of the above: returns a set 
//	of symbols that can produce a set of symbols
set<string> search(set<string> cartesianProd) {
	set<string> leftProductionRule;
	vector <pair<string, string>>::iterator productionsItr;

	for (string product : cartesianProd) {
		for (productionsItr = productions.begin(); productionsItr != productions.end(); ++productionsItr) {
			if (product == productionsItr->second) {
				leftProductionRule.insert(productionsItr->first);
			}
		}
	}
	return leftProductionRule;
}

//Splits a string into all possible substr combinations
//	e.g. aab is split into a,ab and aa,b
set<pair<string,string>> split(string s) {
	set<pair<string, string>> combinations;
	pair<string, string> strPair;
	string leftStr, rightStr;

	for (int delimiter = 1; delimiter < s.length() ; delimiter++) {
		leftStr = s.substr(0, delimiter);
		rightStr = s.substr(delimiter, string::npos);

		strPair.first = leftStr;
		strPair.second = rightStr;
		combinations.insert(strPair);
	}
	return combinations;
}

void solve(Matrix<set<string>>& m, string input) {
	//Filling up the 1st row
	set<string> leftProductionRule;
	string::iterator inputItr = input.begin();

	for (int j = 0; j < m.getCols(); j++) {
		leftProductionRule = search({ *inputItr });

		if (!leftProductionRule.empty()) {
			m[0][j] = leftProductionRule;
		}
		else {
			m[0][j] = emptySet;
		}
		inputItr++;
	}

	//Filling up the rest of the rows 
	set<pair<string,string>> substringPairs;
	set<string> productionSet_Y, productionSet_Z, productionSet_YZ, cartesianProduct;
	string inputSubstring;

	int startingPosition = 0;
	int matrixCol = m.getCols() - 1; 	
	
	for (int row = 1; row < m.getRows(); row++) {	
		for (int col = 0; col < matrixCol; col++) {

			//Create a substring out of "input" based on the current row and col
			inputSubstring = input.substr(startingPosition, row + 1); 
			startingPosition++;		

			//Split the obtained substring into combinations of substrings (Y_[ik]Z_[k+1j])
			substringPairs = split(inputSubstring);
	
			//For every Y and Z, look for the set of variables (productionSet) that produce them
			set<string> fullProductionSet{};
			for (pair<string, string> YZ : substringPairs) {			
				productionSet_Y = m[YZ.first.length() -	1][input.find(YZ.first)];
				productionSet_Z = m[YZ.second.length() - 1][input.find(YZ.second)];
				cartesianProduct = cartesian(productionSet_Y, productionSet_Z);
				
				//Search the production rules for variables that produce the cartesian product
				productionSet_YZ = search(cartesianProduct);

				//Merge the production sets resulting from Y_[ik]Z_[k+1j]
				fullProductionSet = merge(fullProductionSet, productionSet_YZ);
			}

			if (!fullProductionSet.empty()) {
				m[row][col] = fullProductionSet;
			}
			else {
				m[row][col] = emptySet;
			}
		}
		//When a new row is being filled, reset the starting position. Also, 
		//	decrease the amount of columns to be filled (e.g. in a 3x3
		//	matrix, row 0 fills col 0, 1, and 2, but row 1 only fills col 0 and 1 ...)
		startingPosition = 0;
		matrixCol--;
	}
}

int main() {
	cout << "Input a string: ";
	string input;
	cin >> input;
	Matrix<set<string>> table(input.length(), input.length()) ;
	
	std::ifstream file;
	file.open("cfg.txt");
	std::string pr;
	std::string left, right;

	//Read the cfg from file "cfg.txt"
	//	if the grammar is CNF (Assumes starting 
	//	symbol  is "S")
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

		//Read the file and save the productions
		//	in a vector of pairs. For example
		//	S->AB is saved as pair("S","AB");
		left = pr.substr(0, 1);
		right = pr.substr(itr + 2, 2);
		pair<string, string> p(left, right);
		productions.push_back(p);
	}

	solve(table, input);
	file.close();

	//Print the matrix
	for (int i = input.length(); i >= 0; i--) {
		for (int j = 0; j < input.length() - i; j++) {
			cout << "{ ";
			for (auto t : table[i][j]) {
				cout << t;
			}
			cout << " }" << "\t\t";
		}
		cout << endl;
	}

	//The starting symbol is assumed
	//	to be S
	int i = input.length() - 1;
	if (table[i][0].find("S") != table[i][0].end()) {
		std::cout << "\n" << input << " belongs to language.\n";
	}
	else
		std::cout << "\n" << input << " does not belong to language.\n";
}
