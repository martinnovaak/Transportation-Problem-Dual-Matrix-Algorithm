#include <iostream>
#include "TP.h"

void printResult(TP & tp)
{
	if (!tp.getFeasible()) {
		std::cout << "No feasible solution" << std::endl;
		return;
	}
	for (int i = 0; i < tp.getRows(); i++) {
		for (int j = 0; j < tp.getColumns(); j++) {
			std::cout << tp(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "Result: " << tp.getRes();
}

// Simple test
void test1() {
	int rows = 3, cols = 4;
	int C[] = { 2,3,4,5,6,7,8,9,1,4,5,8 };
	int A[] = { 30,35,30 };
	int B[] = { 25,25,20,25 };
	TP tp(rows, cols, C, B, A);
	tp.solve();
	printResult(tp);
}

// Demand != Supply
void test2() {
	int rows = 3, cols = 5;
	int Cost[] = { 5,8,6,6,3,4,7,7,6,5,8,4,6,6,4 };
	int Supply[] = { 8,5,9 };
	int Demand[] = { 4,4,5,4,8 };
	TP tp(rows, cols, Cost, Demand, Supply);
	tp.solve();
	printResult(tp);
}

// Lesser than m + n - 1 nonzero cells 
void test3() {
	int rows = 3, cols = 3;
	int Cost[] = { 8,5,6,15,10,12,3,9,10 };
	int Supply[] = { 120,80,80 };
	int Demand[] = { 150,80,50 };
	TP tp(rows, cols, Cost, Demand, Supply);
	tp.solve();
	printResult(tp);
}

int main()
{
	test1();
	//test2();
	//test3();
}