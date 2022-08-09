#include "TP.h"

TP::TP(unsigned int M, unsigned int N, int* c, int* a, int* b): M(M), N(N), feasible(true)
{
	this->C = new int* [this->M];
	this->X = new int* [this->M];
    for (int i = 0; i < this->M; i++)
	{
		this->C[i] = new int[this->N];
		this->X[i] = new int[this->N];
	}

    for (int i = 0; i < M; i++)
	{
        for (int j = 0; j < N; j++)
		{
			this->C[i][j] = c[i * N + j];
			this->X[i][j] = 0;
		}
	}

    //inicialization
	//STEP 0:
	//0.1
	A = new int[M + N];
	for (int i = 0; i < M; i++)
		A[i] = b[i];
	for (int i = M; i < M + N; i++)
		A[i] = -a[i - M];
	//0.2
	this->u = new int[M];
	for (int i = 0; i < M; i++)
		u[i] = minInRow(i);
	this->v = new int[N];
	for (int j = 0; j < N; j++)
		v[j] = 0;
	//0.3
	this->Gamma = new int* [2];
	for (int i = 0; i < 2; i++)
		Gamma[i] = new int[M+N];
	for (int i = 0; i < M; i++)
	{
		Gamma[0][i] = i;
		Gamma[1][i] = indexMin(i);
	}
	for (int i = M; i < M + N; i++)
	{
		Gamma[0][i] = -1;
		Gamma[1][i] = i - M;
	}


	//0.4
	this->D = new int* [M + N];
	for (int i = 0; i < M + N; i++)
		D[i] = new int[M + N];

	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < M + N; j++)
		{
			if (i == j)
				D[i][j] = 1;
			else
				D[i][j] = 0;
		}
	}
	for (int i = M; i < M+N; i++)
	{
		for (int j = 0; j < M + N; j++)
		{
			if (i == j)
				D[i][j] = -1;
			else
				D[i][j] = 0;
		}
	}
	for (int i = 0; i < M; i++)
	{
		D[i][Gamma[1][i] + M] = -1;
	}
	calcObjective();
}

TP::~TP()
{
	for (int i = 0; i < M; i++)
	{
		delete[] this->C[i];
		delete[] this->X[i];
	}
	delete[] this->C;
	delete[] X;

	delete[] A;
	delete[] u;
	delete[] v;

	for (int i = 0; i < 2; i++)
		delete[] this->Gamma[i];
	delete[] Gamma;

	for (int i = 0; i < M + N; i++)
		delete[] this->D[i];
	delete[] this->D;
}

int TP::minInRow(int row)
{
	int indexMin = 0;
	for (int i = 1; i < N; i++)
		if (C[row][i] < C[row][indexMin])
			indexMin = i;
	return C[row][indexMin];
}

int TP::indexMin(int row)
{
	int indexMin = 0;
	for (int i = 1; i < N; i++)
		if (C[row][i] < C[row][indexMin])
			indexMin = i;
	return indexMin;
}

void TP::calcObjective()
{
	int sum = 0;
	for (int i = 0 ; i < M + N; i++)
	{
		if (i < M)
		{
			sum += A[i] * u[i];
		}
		else
		{
			sum += A[i] * v[i-M];
		}
	}
	psi = sum;
}

void TP::solve()
{
	int* Y = new int[M + N];
	int* P = new int[M];
	int* Q = new int[N];
	int sum = 0;
	int count = 0;
	int theta;
	int s = 0;
	int t = 0;
	int pom;
	while (true)
	{	
		theta = 2147483647; // int max
		sum = 0;
		//1.1
		for (int i = 0; i < M + N; i++) // A * D, ie. vector x matrix
		{
			for (int j = 0; j < M + N; j++)
				sum += A[j] * D[j][i];
			Y[i] = sum;
			sum = 0;
		}
		//1.2
		int k = 0;
		for (int i = 1; i < M + N; i++)
			if (Y[i] < Y[k])
				k = i;
		//1.3
		if (Y[k] >= 0) break;

		//2.1
		for (int i = 0; i < M ; i++)
			P[i] = D[i][k];
		for (int j = 0; j < N; j++)
			Q[j] = D[j + M][k];
		//2.2
		count = 0;
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				if (Q[j] - P[i] > 0)
				{
					count++;
					pom = C[i][j] - u[i] + v[j];
					if (pom < theta)
					{
						theta = pom;
						s = i;
						t = j;
					}
				}
			}
		}
		if (count == 0)
		{
			// no feasible solution
            feasible = false;
            break;
		}

		//3.1.1
		for (int l = 0; l < M + N; l++)
		{
			D[l][k] = -D[l][k];
		}
		//3.1.2
		for (int r = 0; r < M + N; r++)
		{
			if (r != k)
			{
				pom = D[s][r];
				for (int l = 0; l < M + N; l++)
					D[l][r] += (D[t + M][r] - pom) * D[l][k];
			}
		}
		Gamma[0][k] = s;
		Gamma[1][k] = t;

		for (int i = 0; i < M; i++)
			u[i] = u[i] - theta * P[i];
		for (int j = 0; j < N; j++)
			v[j] = v[j] - theta * Q[j];
		calcObjective();
	}

    if(feasible)
    {
        for (int i = 0; i < M + N; i++)
        {
            if (Gamma[0][i] == -1) // virtual cell
                continue;
            X[Gamma[0][i]][Gamma[1][i]] = Y[i];
        }
    }

	delete[] P, Q, Y;
}

int TP::operator()(unsigned int i, unsigned int j)
{
    return X[i][j];
}