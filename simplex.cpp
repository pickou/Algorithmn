#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;
#define N 7
#define M 4
#define INF 999999
float Matrix[M+1][N+1];
int basic[N]; //M<N
class problem{
public:
	problem(string filename);//构造函数
	int GetMinCIndex(int flag);
	int GetMinLamda(int index);
	void printf(char ch);
	void pivot(int falg,int basic[N], int l, int e);
	int Simplex(int flag);
	int InitSimplex();
	void BasicX(int basic[N],float x[N]);//计算基向量对应的顶点
private:
	float A[M][N+1];
	float b[M];
	float c[N];
	float obj;
	float NewC[N+1];
	float z;
	float x[N];//vertex
};

problem::problem(string filename)//构造函数
{
	//txt文件，格式如下，[obj;c;b;a]
	float tmp;
	int i = 0;
	z = 0;
	std::ifstream file(filename);
	while (!file.eof())
	{
		file >> tmp;
		if (i == 0) obj = tmp;
		else if (i < N + 1) c[i - 1] = tmp;
		else if (i < N + M + 1) b[i - N - 1] = tmp;
		else A[(i - N - M - 1) / N][(i - N - M - 1) % N] = tmp;
		i++;
	}

}

int problem::GetMinCIndex(int flag=0)
{
	int min_index = 0;
	float minC = INF;
	if (flag == 0)//求解最优解时
	{
		for (int i = 0; i < N; i++)
		if (c[i] < minC){
			minC = c[i];
			min_index = i;
		}
	}
	else//求解初始解时
	{
		for (int i = 0; i < N+1; i++)
		if (NewC[i] < minC){
			minC = NewC[i];
			min_index = i;
		}
	}
	if (minC>0) return -1;
	else	return min_index;
}
int problem::GetMinLamda(int index)
{
	float minLamda = INF;
	int min_index = -1;
	for (int i = 0; i < M; i++)
	{
		if (A[i][index]>0)
		if (minLamda > b[i] / A[i][index])
		{
			minLamda = b[i] / A[i][index];
			min_index = i;
		}	
	}
	return min_index;
}

void problem::printf(char ch)
{
	if (ch == 'o') cout << obj << endl;
	if (ch == 'a'){
		cout << "A:" << endl;
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
				cout << A[i][j] << " ";
			cout << endl;
		}
	}
	if (ch == 'b')
	{
		cout << "b:" << endl;
		for (int i = 0; i < M; i++)
			cout << b[i] << " ";
		cout << endl;
	}
	if (ch == 'c')
	{
		cout << "c:" << endl;
		for (int i = 0; i < N; i++)
			cout << c[i] << " ";
		cout << endl;
	}
	if (ch == 'n')
	{
		cout << "NewC:" << endl;
		for (int i = 0; i < N; i++)
			cout << NewC[i] << " ";
		cout << endl;
	}
	if (ch == 'z')
		cout << "z:" << endl;
}

void problem::pivot(int flag,int basic[N], int e, int l)
{
	int Column_N=N;
	if (flag == 1) Column_N = N + 1;
	b[e] /= A[e][l];
	float origin = A[e][l];
	for (int i = 0; i < Column_N; i++)
		A[e][i] /= origin;
	//other lines
	for (int i = 0; i < M; i++)//对b
		if (i != e)
			b[i] = b[i] - b[e] * A[i][l];
	for (int i = 0; i < Column_N; i++)//对A
	{
		if (i != l)
		{
			origin = A[e][i];
			for (int j = 0; j < M; j++)
			if (j != e)
				A[j][i] = A[j][i] - origin*A[j][l];
		}
	}
	if (flag == 0)
	{
		origin = c[l];
		for (int i = 0; i < Column_N; i++)//对c
		if (i != l)
			c[i] = c[i] - A[e][i] * origin;
		obj = obj - origin*b[e];//对obj
	}
	else
	{
		origin = NewC[l];
		for (int i = 0; i < Column_N; i++)//对NewC
		if (i != l)
			NewC[i] = NewC[i] - A[e][i] * origin;
		z = z - origin*b[e];//对z
	}
	//基向量（e出基，l入基）
	basic[e] = 0;
	basic[l] = 1;

	printf('a');
	printf('b');
	printf('c');
	printf('o');
	printf('n');
	printf('z');
}
int problem::Simplex(int flag=0)
{
	if (flag==0)
	if (InitSimplex() == -1)//infeasible
	{
		cout << "The problem is infeasible!" << endl;
		return 0;
	}

	int column = GetMinCIndex(flag);
	while (column != -1)
	{
		int row = GetMinLamda(column);
		if (row == -1) return -1;
		pivot(flag,basic, column, row);
		column = GetMinCIndex(flag);
		//BasicX(basic, x);
	}
	
	if (flag == 0)
	{
		cout << "The feasible solution is:" << endl;
		for (int i = 0; i < N; i++)
			cout << "x" << i << ":" << x[i] << " ";
		cout << endl;
		cout << "The objective value is:" << endl;
		cout << obj << endl;
	}
	
	return 1;
}
int problem::InitSimplex()
{
	//find min_index of b
	int BIndex = 0;
	float minB = INF;
	for (int i = 0; i < M;i++)
	if (b[i] < minB)
	{
		minB = b[i];
		BIndex = i;
	}
	//cout <<"minBIndex:"<< BIndex << endl;

	//BasicX--->slack variables<the last M>
	for (int i = 0; i < N;i++)
	if (i < N - M) basic[i] = 0;
	else basic[i] = 1;

	if (minB >= 0) return 1;
	//construct Laux by adding x0 to constraint and use x0 as the objective function
	for (int i = 0; i < N; i++)
		x[i] = 0;
	for (int i = 0; i < N ; i++)
		NewC[i] = 0;
	NewC[N] = 1;
	for (int i = 0; i < M; i++)
		A[i][N] = -1;
	//pivot to make all bi positive
	for (int i = 0; i < M; i++)
	{
		if (i != BIndex)
		{
			for (int j = 0; j < N+1; j++)//对A
			{
				A[i][j] -= A[BIndex][j];
			}
		}	
	}
	for (int i = 0; i < N;i++)//对C
		NewC[i] -= A[BIndex][i];
	NewC[N] -= A[N][BIndex];
	z -= b[BIndex];//对z
	for (int i = 0; i < M; i++)//对b
	if (i != BIndex)
		b[i] -= b[BIndex];
	for (int j = 0; j < N; j++)//mutiply -1 to become positive
	{
		A[BIndex][j] *= -1;
	}
	b[BIndex] *= -1;
	/*
	printf('a');
	printf('b');
	printf('c');
	*/
	/*print the basic*/
	for (int i = 0; i < N;i++)
	if (basic[i])
		cout << i + 1 << endl;
	cout << "Z:" << z<<endl;
	//find the optimal solution to Laux
	if (Simplex(1) == -1||z!=0) return -1;
	else  return 1;
}
void problem::BasicX(int basic[N],float x[N])
{
	for (int j = 0; j < N; j++)
	{
		if (basic[j] == 0)//non-basic variables
			x[j] = 0;
		else
		{
			for (int i = 0; i < M; i++)
			if (A[i][j] == 1)
				x[j] = b[i];
		}
	}
}

int main()
{
	problem P("test1.txt");
	P.printf('a');
	P.printf('b');
	P.printf('c');
	cout << P.InitSimplex() << endl;;
	return 0;
}