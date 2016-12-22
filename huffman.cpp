#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<map>
#include<iterator>
using namespace std;
map<char, string> HuffmanCode;
long size_str;
struct node{
	char ch;
	int freq;
	node *lchild, *rchild;
	node(char c = 0, int freq = 0, node *l = NULL, node *r = NULL) :
		ch(c), freq(freq), lchild(l), rchild(r){}
};
struct cmp{
	bool operator()(node*& a, node*& b)
	{
		return a->freq > b->freq;
	}
};
node* CreateTree(string str, map<char, int> charSet);
void TreeInfo(node *Huffman, string code);
void Compress(string filename);
node* test(string file);
void Decompress(string filename);

int main()
{
	string filename = "Aesop_Fables.txt";
	char choose = '0',next_choose='0';
	cout << "0. Compress     1. Decompress" << endl;
	cout << "please input 0 or 1:" << endl;
	cin >> choose;
	long length_0, length_1;
	if (choose == '0')
	{
		cout << "******get the file to compress:******"<<endl;
		cout << " 0. Aesop_Fables.txt    1.graph.txt" << endl;
		cin >> choose;
		if (choose == '1') { filename = "graph.txt"; }
		Compress(filename);
		//test(filename);
		cout << "compress ok!" << endl;
	}
	else if (choose == '1')
	{
		cout << "Please choose the file to decompress:" << endl;
		cout << " 0. Aesop_Fables.txt    1.graph.txt" << endl;
		cin >> next_choose;
		if (next_choose=='1') filename = "graph.txt";
		std::ifstream t(filename, ios::binary);
		t.seekg(0, ios::end);
		length_0 = t.tellg();
		t.close();
		filename = "compressed_file.bin";
		std::ifstream t1(filename, ios::binary);
		t1.seekg(0, ios::end);
		length_1 = t1.tellg();
		t1.close();
		Decompress(filename);
		cout << "decompress ok!" << endl;
		cout << "the rate is:" <<(float)length_0 / length_1 << endl;
	}
	return 0;
}
node* CreateTree( map<char, int> charSet)
{
	std::priority_queue<node*, vector<node*>, cmp> que;
	for (map<char, int>::iterator p = charSet.begin(); p != charSet.end(); p++)//将节点放入优先队列里
	{
		que.push(new node(p->first, p->second));
	}
	while (que.size() > 1)//每次取出频率最小的两个节点，合并之后重新入队
	{
		node *l = que.top(); que.pop();
		node *r = que.top(); que.pop();
		node * newnode = new node(0, l->freq + r->freq, l, r);
		que.push(newnode);
	}
	return que.top();
}
void TreeInfo(node *Huffman, string code)
{
	node * tree = Huffman;
	if (tree->lchild == NULL&&tree->rchild == NULL)
	{
		//cout << tree->ch << ":" << code << endl;
		HuffmanCode[tree->ch] = code;
		return;
	}
	if (tree->lchild != NULL) TreeInfo(tree->lchild, code + '0');
	if (tree->rchild != NULL) TreeInfo(tree->rchild, code + '1');
}
void Compress(string filename)
{
	std::ifstream myfile(filename);
	char ch;
	long length;
	string code;
	//cout << str;
	//统计字符频率
	std::map<char, int> charSet;
	myfile.seekg(0, ios::end);
	length = myfile.tellg();
	myfile.clear();
	myfile.seekg(0, ios::beg);
	while (length != myfile.tellg())
	{
		ch = myfile.get();
		charSet[ch]++;
	}
/*	for (map<char, int>::iterator p = charSet.begin(); p != charSet.end(); p++)
		cout << p->first << ":" << p->second << endl;
*/	
	//huffman编码建树
	node* HuffmanTree = CreateTree(charSet);
	//存储huffmancode
	TreeInfo(HuffmanTree, code);
	//压缩文件
	int num = 0;
	unsigned char str_tmp = 0x00;
	std::ofstream outfile("compressed_file.bin",ios::binary);
	//将charSet保存到文本中
	outfile << "###";
	for (map<char, int>::iterator p = charSet.begin(); p != charSet.end(); p++)
	{
		outfile << p->first;
		outfile << " ";
		outfile << p->second;
		outfile << " ";
	}
	outfile << "###";
	//指针回到文件开始
	myfile.clear();
	myfile.seekg(0, ios::beg);
	//cout << myfile.tellg() << endl;
	while (length!=myfile.tellg())
	{   
		ch = myfile.get();
		const char* ptr = HuffmanCode[ch].c_str();
		while (*ptr != '\0')
		{
			num += 1;
			if (*ptr == '0')   str_tmp = str_tmp << 1;
			if (*ptr == '1')   str_tmp = (str_tmp << 1) | 0x01;
			if (num == 8)//一个字符完成写入文件
			{
				num = 0;
				outfile << str_tmp;
			}
			ptr++;
		}
	}
	if (num != 0){
		while (num != 8)//补齐最后一个不够8位的编码
		{
			str_tmp=str_tmp << 1;
			num += 1;
		}
		outfile << str_tmp;
	}
	outfile << (size_str%8);
	//cout << (size_str % 8)<<endl;
	outfile.close();
}
node* test(string filename)//测试使用
{
	std::ifstream myfile(filename);
	std::string str((std::istreambuf_iterator<char>(myfile)),
		std::istreambuf_iterator<char>());//将文件一次性读入string
	string code;

	//统计字符频率
	const char* ptr = str.c_str();
	std::map<char, int> charSet;
	while (*ptr != '\0')
	{
		charSet[*ptr++]++;
	}
	for (map<char, int>::iterator p = charSet.begin(); p != charSet.end(); p++)
		cout << p->first << " " << p->second << endl;

	//huffman编码建树
	node* HuffmanTree = CreateTree(charSet);
	//存储huffmancode
	TreeInfo(HuffmanTree, code);
	/*for (map<char, string>::iterator p = HuffmanCode.begin(); p != HuffmanCode.end(); p++)
		cout << p->first << " " << p->second << endl;
		*/
	int num = 0;
	unsigned char str_tmp = 0x00;
	const char* ptr1 = str.c_str();
	std::ofstream outfile("compressed_file.bin",ios::binary);
	//将charSet保存到文本中
	outfile << "###";
	for (map<char, int>::iterator p = charSet.begin(); p != charSet.end(); p++)
	{
		outfile << p->first;
		outfile << " ";
		outfile << p->second;
	}
	outfile << "###";
	//压缩
	while (*ptr1 != '\0')
	{
		const char *ptr = HuffmanCode[*ptr1].c_str();
		while (*ptr != '\0')
		{
			num += 1;
			if (*ptr == '0')	 str_tmp = str_tmp << 1;// , cout << "0";
			if (*ptr == '1')   str_tmp = (str_tmp << 1) | 0x01;// , cout << "1";
			if (num == 8)//一个字符完成写入文件
			{
				num = 0;
				outfile << str_tmp;
				//cout<<endl;
			}
			ptr++,size_str++;
		}
		ptr1++;
	}
	//cout << size_str << endl;
	if (num != 0){
		while (num != 8)//补齐最后一个不够8位的编码
		{
			str_tmp=str_tmp << 1;
			num += 1;
			//cout << "0";
		}
		outfile << str_tmp;
		//std::cout <<size_str << endl;
		//cout << endl<<"decompress";
	}
	outfile << (size_str%8);
	outfile.close();
	return HuffmanTree;
}
void Decompress(string file)
{
	std::ifstream t(file,ios::binary);
	unsigned char ch, ch1, flag, split;
	int tail=0;
	long freq;
	int num=0;
	long length = 0;
	t.seekg(0,ios::end);
	long filelen = t.tellg();
	t.seekg(0, ios::beg);
	std::ofstream outfile("Decompressed_file.txt");
	std::map<char, int> charSet;
	for (int i = 0; i < 3; i++)//判断头###
	{
		flag=t.get();
		if (flag != '#') 
		{
			cout << i<<"th error occurred" << endl;
			return;
		}
	}
	while (1)//读取存入的charSet
	{
		flag = t.get();
		if (flag == '#')//判断编码的尾
		{
		    flag=t.get();
			//cout << flag << endl;
			if (flag == '#')
			{
				t >> flag;
				break;
			}
			else
			{
				flag = '#';
				t>>freq;
				charSet.insert(pair<char, int>(flag, freq));
				flag=t.get();//抛弃末尾的空格
			    flag=t.get();//获得下一个开始的字符
			}
		}
		split=t.get();
		t >> freq;
		charSet.insert(pair<char,int>(flag, freq));
		flag=t.get();//抛弃末尾的空格
		
	}
	
/*	for (map<char, int>::iterator p = charSet.begin(); p != charSet.end(); p++)
	{
		cout << p->first;
		cout << ":";
		cout << p->second<<endl;
	}
	*/
	node *Huffman = CreateTree(charSet);//建树
	node* tree = Huffman;
	//cout << "tree create ok" << endl;
	while (1)
	{
		ch = t.get();
		if (filelen - t.tellg() == 0)//已经读到了最末尾了
		{
			t >> tail;
			tail = 8 - tail;
		}
		for (int i = 0; i < 8-tail; i++)
		{
			ch1 = ch << i & 0x80;
			if (tree->lchild == NULL && tree->rchild == NULL)
			{
				outfile << tree->ch;
				tree = Huffman;
             }
			if (ch1 == 0x00)  tree = tree->lchild;// , cout << "0";
			if (ch1 == 0x80)  tree = tree->rchild;// cout << "1";
		}
		if (-1== t.tellg()) break;
	} 
	outfile.close(); 

}