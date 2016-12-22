#include<iostream>
#include<fstream>
#include<string>
using namespace std;

#define N 100000
#define M 10
int array_temp[N];
long long InversionCount = 0;
long long MergeandCount(int a[N],int l,int r)
{
	long long Rc = 0;
	int i = 0,j = 0,k=0;
	int m = (l + r) / 2;
	i = l;
	j = m + 1;
	for (k = l; k <= r;k++)
	{
		if (i>m)//���ұߵ������������
		{
			//cout << "j" <<j<< endl;
			while (j <= r) array_temp[k++] = a[j++];
			break;
		}
		if (j > r)
		{
			while (i <= m)	array_temp[k++] = a[i++];
			break;
		}

		if (a[i]>a[j])
		{
			array_temp[k] = a[j];
			j++;
			Rc += m+1-i;
			//cout << Rc << endl;
		}
		else
		{
			array_temp[k] = a[i];
			i++;
		}

	}
	//�ı�ԭ���������
	for (k = l; k <= r; k++)
	{
		a[k] = array_temp[k];
		//cout << a[k] << ":";
	}
	//cout << endl;
	//cout << Rc<<endl;
	return Rc;
}
long long SortandCount(int a[N],int l,int r)
{
	long long count_l, count_r,count_num = 0;
	if (l == r) return 0;
	int m = (l + r) / 2;
	count_l=SortandCount(a, l,m );
	count_r=SortandCount(a, m + 1, r);
	//cout << count_l << ";" << count_r << endl;
	count_num += MergeandCount(a, l, r)+count_l+count_r;
	return count_num;
}


void merge_count(int *data, int & splitter, int left, int right){
	if (left > splitter || splitter > right)
		return;
	int pivot = data[splitter];
	int *tempLL = NULL, *tempLR = NULL, *tempRL = NULL, *tempRR = NULL;
	int ll = 0, lr = 0, rl = 0, rr = 0;
	int i;
	bool flag1 = false, flag2 = false;

	//��������������,����˳���splitter����
	if (left <= splitter){
		flag1 = true;
		tempLL = (int *)malloc(sizeof(int)* (splitter - left));
		tempLR = (int *)malloc(sizeof(int)* (splitter - left));
		for (i = splitter - 1; i >= left; i--)
		if (data[i] < pivot)
			tempLL[ll++] = data[i]; //llΪleft����Ŀǰ��splitterС����splitter����ĵ�ĸ���
		else{
			tempLR[lr++] = data[i];
			InversionCount += ll + 1; //ll+1 Ϊ�õ����left���ֱ�splitterС�����ڸõ��ұߵĵ��������
		}
	}
	//�ұ������������,����˳���splitter����
	if (right>splitter){
		flag2 = true;
		tempRL = (int *)malloc(sizeof(int)* (right - splitter));
		tempRR = (int *)malloc(sizeof(int)* (right - splitter));
		for (i = splitter + 1; i <= right; i++)
		if (data[i] > pivot)
			tempRR[rr++] = data[i]; //rrΪright����Ŀǰ��splitter����splitter����ĵ�ĸ���
		else{
			tempRL[rl++] = data[i];
			InversionCount += rr + 1; //rr+1Ϊ�õ����right���ֱ�splitter�󵫴��ڸõ���ߵĵ��������
		}
	}
	//����left��right֮������������
	InversionCount += lr * rl;

	//���µ����п�����ԭ����,ע�⿽����˳��
	int k = left;
	for (i = ll - 1; i >= 0; i--)
		data[k++] = tempLL[i];
	for (i = 0; i<rl; i++)
		data[k++] = tempRL[i];

	splitter = k;
	data[k++] = pivot;
	for (i = lr - 1; i >= 0; i--)
		data[k++] = tempLR[i];
	for (i = 0; i<rr; i++)
		data[k++] = tempRR[i];

	if (flag1)
		delete tempLL, tempLR;
	if (flag2)
		delete tempRL, tempRR;
}

void quicksort_count(int *data, int left, int right){

	if (left >= right)
		return;
	int splitter = left;
	merge_count(data, splitter, left, right);
	quicksort_count(data, left, splitter - 1);
	quicksort_count(data, splitter + 1, right);
}

int main()
{
	//�������ݵ�����a[N]��
	int a[N];
	//int b[M];
	ifstream fin;
	fin.open("Q8.txt");
	for (int i = 0; i < N; i++)
		fin >> a[i];
	fin.close();
	//������
	/*for (int i = 0; i < M; i++)
	{
		b[i] = a[i];
		cout << b[i] << endl;
	}*/

	//���������
	//cout << "�鲢����"<<SortandCount(a, 0, N-1) << endl;
	//�������
	quicksort_count(a, 0, N-1);
	cout << "��������" << InversionCount<< endl;
	return 0;
}

