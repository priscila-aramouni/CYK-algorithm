#pragma once
#include <string>
#include <vector>

using namespace std;

template <typename T>
class Matrix {
private:
	T* p;
	int rows, cols;
public:
	Matrix(int n, int m) :rows(n), cols(m) {
		/* more efficient to create
			a 1-d array that represents 
			a 2-d matrix
			*/
		p = new T[n*m]();
	}

	T* operator[](int i) {
		return &p[i*cols];
	}

	int getRows() {
		return rows;
	}

	int getCols() {
		return cols;
	}
};

//Return the union of two sets
template <typename T>
set<T> merge(set<T>& ls, set<T>& rs) {
	set<T> newSet (ls);

	for (T t : rs) {
		newSet.insert(t);
	}
	return newSet;
}

//Returns the cartesian 
//	product of two sets
template <typename T>
set<T> cartesian(set<T>& s1, set<T>& s2) {
	set<T> products;

	if (s1.find("*") != s1.end() || s2.find("*") != s2.end()) {
		return emptySet;
	}

	for (T s1_variable : s1) {
		for (T s2_variable : s2) {
			products.insert(s1_variable + s2_variable);
		}
	}
	return products;
}
