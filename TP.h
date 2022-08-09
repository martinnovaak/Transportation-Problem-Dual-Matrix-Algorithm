#pragma once

class TP 
{
private:
    unsigned int M, N;   // Number of rows and cols
	int** C;	// Cost tableau
	int** X;	// Tableau of solutions
	int psi;	// Result

	// The following variables are named as in the article: A dual-matrix approach to the transportation problem
	int* A;		
	int* u;
	int* v;
	int** Gamma;
	int** D;	// DUAL MATRIX

    bool feasible; 

	void calcObjective();
	int minInRow(int column);
	int indexMin(int column);
public:
    TP(unsigned int M, unsigned int N,int* c, int* a, int* b);
	~TP();
	void solve();
	int operator()(unsigned int i, unsigned int j) { return X[i][j]; };
    int getRows(){return M;};
    int getColumns(){return N;};
    int getRes(){return psi;};
    bool getFeasible(){return feasible;};
};
