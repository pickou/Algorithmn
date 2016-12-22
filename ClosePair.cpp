#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
struct Point{
	float x, y;
};//平面上的点
float min_(float x, float y)
{
	return (x < y) ? x : y;
}
float Distance(Point a, Point b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}
int compareX(const void* a, const void* b)
{
	Point *p1 = (Point *)a, *p2 = (Point *)b;
	return (p1->x - p2->x);
}
int compareY(const void* a, const void* b)
{
	Point *p1 = (Point *)a, *p2 = (Point *)b;
	return (p1->y - p2->y);
}
float BruteForce(Point p[], int n)
{
	float min = INFINITY;
	for (int i = 0; i < n;++i)
	  for (int j = i+1; j < n;++j)
	  if (Distance(p[i], p[j]) < min)
		  min = Distance(p[i], p[j]);
	return min;
}
float StripClosest(Point strip[], int size, float d)
{
	float min = d;
	qsort(strip, size, sizeof(Point), compareY);
	for (int i = 0; i < size;++i)
		for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
			if (Distance(strip[i], strip[j]) < min)
				min = Distance(strip[i], strip[j]);
	return min;
}

float ClosePair(Point p[],int n)
{
	if (n <= 3) return BruteForce(p, n);
	int mid = n / 2;
	Point MidPoint = p[mid];

	float dl = ClosePair(p, mid);
	float dr = ClosePair(p + mid, n - mid);

	float d = min(dl, dr);
	Point *strip;
	strip=(Point *)malloc(sizeof(Point)*n);
	int j = 0;
	for (int i = 0; i < n;i++)
	if (abs(p[i].x - MidPoint.x) < d)
		strip[j] = p[i], j++;
	return min(d, StripClosest(strip, j, d));
}
float Closest(Point p[], int n)
{
	qsort(p, n, sizeof(Point), compareX);
	return ClosePair(p, n);
}
int main()
{
	Point p[] = { { 2, 5 }, { 11, 3 }, { 5, 7 }, { 12, 10 }, { 15, 3 }, { 7, 6 }, { 4, 5 }, { 6, 9 }, {7,7} };
	int n = sizeof(p) / sizeof(p[0]);
	cout << "The Distance of the Closest pair:"<<Closest(p, n) << endl;
	return 0;
}