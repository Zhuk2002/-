#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include <conio.h>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <Windows.h>
#include <fstream>

using namespace std;

const int M = 256;

float entropy = (float)(0);
float midlength = (float)(0);

struct record
{
	char a[30];
	short int b;
	char c[22];
	char d[10];
};

struct queue
{
    int number;
    queue* next;
};

struct Vertex {
    int number;
    Vertex *left;
    Vertex *right;
};

int Up(int n, float q, float *P);
void Down(int n, int j, int *L, int **C);
void Huffman(float *P, int *L, int **C, int n);
void Encoding();
void swap(int* a, int* b);
void QuickSortV2(int** A, int L, int R);
void addVertex(Vertex*& p, int data, record* tt);
void A1(Vertex*& p, int size, int** VW, record* tt);
void DeleteTree(Vertex* p);
void Obhod(Vertex* p, record* tt);
int SizeTree(Vertex* p);
int TreeSearch(Vertex* p, record* tt, short int &X, bool& flag);
int BSearch(int A[], char X[], record* tt);
void PrintBSearch(int A[], record* tt);
void PrintQueue(queue* head, record* tt);
void BSearchQueue(int A[], record* tt, queue*& head);
int Compare(record* tt, int A, int B, int n);
void Heap(int A[], int L, int R, record* tt);
void HeapSort(int A[], int N, record* tt);
void PrintDataBase(int A[], record* tt);
void PrintDataBaseBy20(int A[], record* tt);

int main()
{
	SetConsoleCP(1251);
    FILE* fp;
    fp = fopen("testBase2.dat", "rb");
    record* tt = new record[4000];
    if(fread((record*)tt, sizeof(record), 4000, fp));
    system("color 03");
    fclose(fp);
    
    Vertex* Root = NULL;
    bool flag = 0, tree_flag = 0;
    int A[4000], i1=0;
    short int key;
    int** VW;
    VW = new int*[2];
    for (int i = 0; i < 2; i++)
        VW[i] = new int[4000];
    char switcher;
    queue* head = NULL;
    queue* temp;
    for(int i=0; i<4000; i++){
    	A[i]=i;
	}
    HeapSort(A,4000,tt);
    while(true){
    	system("CLS");
    	if(flag)
    		cout << "Press 1 for full list" << 
			endl << "Press 2 for print by 20" << 
			endl << "Press 3 for search" << 
			endl << "Press 4 for code" << 
			endl <<"Press 5 for tree search" << endl << ">:";
    	else
    		cout << "Press 1 for full list" << 
			endl << "Press 2 for print by 20" << 
			endl << "Press 3 for search" << 
			endl << "Press 4 for code" << endl << ">:";
    	switcher = getch();
    	cout << endl;
   		switch(switcher){
   			case '1':
   				PrintDataBase(A,tt);
            	break;
            case '2':
            	PrintDataBaseBy20(A,tt);
            	break;
            case '3':
            	BSearchQueue(A,tt,head);
            	DeleteTree(Root);
            	Root = NULL;
            	temp = head;
            	i1 = 0;
            	while (temp != NULL) {
					VW[0][i1] = temp->number;
        			temp = temp->next;
        			i1++;
    			}
    			for (int i = 0; i < i1; i++)
        			VW[1][i] = rand() % 100+1;
        		A1(Root, i1, VW, tt);
            	flag = 1;
            	break;
            case '5':
            	i1 = 1;
            	system("CLS");
            	cout << "Tree:" << endl;
            	Obhod(Root,tt);
            	cout << "Size = " << SizeTree(Root) << endl;
            	cout << "Enter search number of department:";
            	cin >> key;
            	TreeSearch(Root, tt, key, tree_flag);
            	if(!tree_flag)
					cout << "Nothing found!" << endl;
				tree_flag = 0;
				cout << "Press any button";
            	getch();
            	break;
            case '4':
            	system("CLS");
            	Encoding();
            	cout << "Press any button";
            	getch();
            	break;
            default:
            	delete[] tt;
    			return 0;
		}
	}
}

//Functions

int Up(int n, float q, float *P)
{
	int j = n-1;
	for(int i = n-1; i >= 2;i--){
		if(P[i-1] < q){
			P[i] = P[i-1];
		} 
		else {
			j = i;
			P[j] = q;
			return j;
		}
	}
	j = 1;
	P[j] = q;
	return j;
}

void Down(int n, int j, int *L, int **C)
{
	int S[n+1];
	for(int i = 1; i <= n; i++) S[i] = C[j][i];
	int l = L[j];
	for(int i = j; i <= n-2; i++){
		for(int t = 1; t <= n; t++){
			C[i][t] = C[i+1][t];
		}
		L[i] = L[i+1];
	}
	for(int i = 1; i <= n; i++) C[n-1][i] = S[i];
	for(int i = 1; i <= n; i++) C[n][i] = S[i];
	C[n-1][l+1] = 0;
	C[n][l+1] = 1;
	L[n-1] = l+1;
	L[n] = l+1;
	
}

void Huffman(float *P, int *L, int **C, int n)
{
	if(n == 2){
		C[1][1] = 0;
		L[1] = 1;
		C[2][1] = 1;
		L[2] = 1;
	}else{
		float q = P[n-1]+P[n];
		int j = Up(n, q, P);
		Huffman(P, L, C, n-1);
		Down(n, j, L, C);
	}
}

void Encoding()
{
	SetConsoleCP(1251);
	int i, j = 1, sum = 0;
	float Lm = 0, H = 0;
	long int n = 0;
	char temp_char;
	FILE *f;
	f = fopen("testBase2.dat", "rb");
	float** char_freq = new float*[2];
	for (i = 0; i < 2; i++) {
		char_freq[i] = new float[M];
	}
	for (i = 0; i < M; i++){
		char_freq[0][i] = i;
		char_freq[1][i] = 0;
	}
	fseek(f , 0 , SEEK_END);
	long lSize = ftell(f);
	rewind (f);
	unsigned char * buffer = (unsigned char*) malloc(sizeof(char) * lSize);
	if(fread(buffer, 1, lSize, f));
	for(i = 0; i < sizeof(char) * lSize; i++){
		char_freq[1][buffer[i]] += 1;
	}
	for (i = 0; i < M; i++){
		if(char_freq[1][i] > 0){
			n++;
		}
		sum += char_freq[1][i];
	}
	float P[n+1], P1[n+1];
	for (i = 0; i < M; i++){
		if(char_freq[1][i] > 0){
			P[j] = char_freq[1][i] / sum;
			j++;
		}
	}
	float tmp;
	for (i = 1; i<n-1; i++){
		for (j=n-1; j>i; j--){
			if (P[j] > P[j-1]) {
				tmp = P[j-1];
				P[j-1] = P[j];
				P[j] = tmp;
				
			}
		}
	}
	for (i = 1; i<=n; i++){
		P1[i] = P[i];
	}
	for (i = 0; i<n-1; i++){
		for (j=n-1; j>i; j--){
			if(char_freq[1][j] > char_freq[1][j-1]){
				tmp = char_freq[0][j-1];
				char_freq[0][j-1] = char_freq[0][j];
				char_freq[0][j] = tmp;
				tmp = char_freq[1][j-1];
				char_freq[1][j-1] = char_freq[1][j];
				char_freq[1][j] = tmp;
			}
		}
	}
	int **C = new int* [n+1];
	for(int i=0;i<n+1;i++){
		C[i] = new int[n+1];
	}
	int* L = new int[n+1];
	Huffman(P, L, C, n);
	j = 1;
	for(int i = 1; i <= n; i++){
		cout << j << ": " << (char)char_freq[0][i-1] << "\t" << P1[i-1] << "\t";
		j++;
		for(int j = 1; j <= L[i]; j++){
			cout << C[i][j] << " ";
		}
		cout << endl;
	}
	for(int i = 1; i <= n; i++){
		Lm += P1[i] * L[i];
	}
	for(int i = 1; i <= n; i++){
		H -= P1[i] * log2(P1[i]);
	}
	cout << "Average length = " << Lm << endl << "Entropy = " << H << endl << endl;
}

void swap(int* a, int* b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void QuickSortV2(int** A, int L, int R)
{
	while (L < R)
	{
		int x = A[1][L];
		int i = L;
		int j = R;
		while (i <= j)
		{
			while (A[1][i] > x)
				i++;
			while (A[1][j] < x)
				j--;
			if (i <= j)
			{
				swap(&A[0][i], &A[0][j]);
				swap(&A[1][i], &A[1][j]);
				i++;
				j--;
			}
		}
		if (j - L > R - i) {
			QuickSortV2(A, i, R);
			R = j;
		}
		else {
			QuickSortV2(A, L, j);
			L = i;
		}
	}
}

void addVertex(Vertex*& p, int data, record* tt)
{
	if (p == NULL)
	{
		p = new Vertex;
		p->number = data;
		p->left = NULL;
		p->right = NULL;
	}
	else if (tt[data].b <= tt[p->number].b) {
		addVertex(p->left, data, tt);
	}
	else if (tt[data].b > tt[p->number].b){
		addVertex(p->right, data, tt);
	}
}

void A1(Vertex*& p, int size, int** VW, record* tt)
{
	QuickSortV2(VW, 0, size-1);
	for (int i = 0; i < size; i++){
		addVertex(p, VW[0][i], tt);
	}
}

void DeleteTree(Vertex* p)
{
	if(p){
		DeleteTree(p->right);
        DeleteTree(p->left);
        delete p;
	}
}

void Obhod(Vertex* p, record* tt) 
{
    if (p != NULL) 
	{
        Obhod(p->left,tt);
        cout<<tt[p->number].a<<setw(4)<<"|"<<tt[p->number].b<<"|\t"<<tt[p->number].c<<"|"<<tt[p->number].d<<"|"<<p->number<<endl;
        Obhod(p->right,tt);
    }
}

int SizeTree(Vertex* p) 
{
    int n;
    if (p == NULL) n = 0;
    else
        n = 1 + SizeTree(p->left) + SizeTree(p->right);
    return n;
}

int TreeSearch(Vertex* p, record* tt, short int &X, bool& flag)
{
	if (p) 
	{
		if(tt[p->number].b > X){
			TreeSearch(p->left, tt, X, flag);
		}
        else if(tt[p->number].b < X){
        	TreeSearch(p->right, tt, X, flag);
		}
		else if(tt[p->number].b == X){
			TreeSearch(p->left, tt, X, flag);
			cout<<tt[p->number].a<<setw(4)<<"|"<<tt[p->number].b<<"|\t"<<tt[p->number].c<<"|"<<tt[p->number].d<<"|"<<p->number<<endl;
			flag = 1;
		}
    }
}

int BSearch(int A[], char X[], record* tt)
{
	int L = 0, R = 3999, m;
	while (L < R) {
		m = (L+R)/2;
		if (strncmp(tt[A[m]].d,X,2)>0)
			L = m+1;
		else
			R = m;
	}
	if (strncmp(tt[A[R]].d,X,2) == 0)
		return R;
	return -1;
}

void PrintQueue(queue* head, record* tt){
	queue* temp = head;
	int j = 1;
	while (temp != NULL) {
		cout<<j<<".\t"<<tt[temp->number].a<<setw(4)<<"|"<<tt[temp->number].b<<"|\t"<<tt[temp->number].c<<"|"<<tt[temp->number].d<<"|"<<endl;
		temp = temp->next;
		j++;
	}
	cout << "Press any button";
	getch();
}

void BSearchQueue(int A[], record* tt, queue*& head){
	head = NULL;
	queue* temp = head;
	system("CLS");
	int key, element, j=1;
	char char_key[2];
	cout << "Enter birthday:";
	cin >> key;
	
	if(key < 100){
		if (key < 10){
			itoa(key, char_key, 10);
			char_key[1] = char_key[0];
			char_key[0] = '0';
			element = BSearch(A, char_key, tt);
		}
		else{
			itoa(key, char_key, 10);
			element = BSearch(A, char_key, tt);
		}
	}
	else{
		element = -1;
	}	
	if (element == -1){
		cout << "Key was not found!" << endl << "Press any button";
		getch();
		return;
	}
	while(strncmp(tt[A[element]].d,char_key,2)==0){
		temp = new queue;
		temp->number = A[element];
		temp->next = head;
		head = temp;
		j++;
		element++;
		if ((element == 4000) or (element == -1))
			break;
	}
	PrintQueue(head,tt);
}

int Compare(record* tt, int A, int B, int n){
	if (strncmp(tt[A].d,tt[B].d,2)<0) return 1;
	if (strncmp(tt[A].d,tt[B].d,2)>0) return 0;
	if (strcmp(tt[A].a,tt[B].a)<0) return 1;
	if (strcmp(tt[A].a,tt[B].a)>0) return 0;
	return n;
}

void Heap(int A[], int L, int R, record* tt){
	int X = A[L], j, i=L;
	while(1){
		j = 2*i;
		if(j > R){
			break;
		}
		if(j < R){
			if(Compare(tt, A[j+1], A[j], 1)) j++;
		}
		if(Compare(tt, X, A[j], 1)){
			break;
		}
		A[i]=A[j];
		i=j;
	}
	A[i]=X;
}

void HeapSort(int A[], int N, record* tt){
	N--;
	int L=N/2, R=N, t;
	while(L>=0){
		Heap(A,L,N,tt);
		L--;
	}
	while(R>0){
		t=A[0];
		A[0]=A[R];
		A[R]=t;
		R--;
		Heap(A,0,R,tt);
	}
}

void PrintDataBase(int A[], record* tt){
	system("CLS");
	int i, j=1;
   	for (i=3999; i >= 0; i--){
	   cout<<j<<".\t"<<tt[A[i]].a<<setw(4)<<"|"<<tt[A[i]].b<<"|\t"<<tt[A[i]].c<<"|"<<tt[A[i]].d<<"|"<<endl;
	   j++;
	}
	cout << "Press any button";
	getch();
}

void PrintDataBaseBy20(int A[], record* tt){
	int i = 3999, lim = 3979, j=1;
	char switcher;
    while(true){
        system("CLS");
      	for (; i > lim ; i--){
      	  	cout<<j<<".\t"<<tt[A[i]].a<<setw(4)<<"|"<<tt[A[i]].b<<"|\t"<<tt[A[i]].c<<"|"<<tt[A[i]].d<<"|"<<endl;
      	  	j++;
		}
		lim -= 20;
        cout << "1 - next 20/0 - exit:";
        while(true){
        	switcher = getch();
        	if((switcher == '1') or (switcher == '0'))
				break;
		}
        if ((switcher == '0') or (i<19))
        	break;
    }
}

//void PrintBSearch(int A[], record* tt){
//	system("CLS");
//	int key, element, j=1;
//	char char_key[2];
//	cout << "Enter birthday:";
//	cin >> key;
//	if(key < 100){
//		if (key < 10){
//			itoa(key, char_key, 10);
//			char_key[1] = char_key[0];
//			char_key[0] = '0';
//			element = BSearch(A, char_key, tt);
//		}
//		else{
//			itoa(key, char_key, 10);
//			element = BSearch(A, char_key, tt);
//		}
//	}
//	else{
//		element = -1;
//	}	
//	if (element == -1){
//		cout << "Key was not found!" << endl << "Press any button";
//		getch();
//		return;
//	}
//	while(strncmp(tt[A[element]].d,char_key,2)==0){
//		cout<<j<<".\t"<<tt[A[element]].a<<setw(4)<<"|"<<tt[A[element]].b<<"|\t"<<tt[A[element]].c<<"|"<<tt[A[element]].d<<"|"<<endl;
//		j++;
//		element++;
//		if ((element == 4000) or (element == -1))
//			break;
//	}
//	cout << "Press any button";
//	getch();
//}
