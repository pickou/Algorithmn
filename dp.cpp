#include<iostream>
using namespace std;
#define N 10
float max(float a, float b)
{
	if (a > b) return a;
	else return b;
}
float GetMaxProfit(float Stock_price[N])
{
	float min_price=Stock_price[0];
	float max_price = Stock_price[N - 1];
	float opt_L[N],opt_R[N];
	float profit = 0;
	for (int i = 0; i < N; i++) 
	{
		opt_L[i] = 0; 
		opt_R[i] = 0;
	}
	for (int i = 1; i < N; i++)
	{
		opt_L[i] = max(opt_L[i - 1], Stock_price[i] - min_price);
		if (Stock_price[i] < min_price) min_price = Stock_price[i];
	}
	for (int i = N - 2; i >= 0; i--)
	{
		opt_R[i] = max(opt_R[i + 1], max_price-Stock_price[i]);
		if (Stock_price[i]>max_price) max_price = Stock_price[i];
	}
	for (int i = 0; i < N; i++)
		profit=max(profit,opt_L[i] + opt_R[i]);
	return profit;
}
int main()
{
	float Stock_price[N] = { 1.3, 2.2, 1, 3, 6.8, 3.2, 4, 6, 9, 11.2 };
	cout << "Max_Profit:" << GetMaxProfit(Stock_price) << endl;
	return 0;
}