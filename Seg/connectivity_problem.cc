#include <iostream>
#include <map>
#include <vector>
void print( int* array, int N );
void print( int, int );
typedef std::vector<int> Vi;
typedef std::vector<int>::iterator ViIt;

using namespace std;
static const int N = 20;

void quick_find( Vi vpq ) {
	// ��������� - ��������� �������������
	int i, id[N];
	for ( i = 0; i < N; i++)
		id[i] = i;
		
	ViIt at = vpq.begin(), end = vpq.end();
	// ������ �����
	for ( ; at != end; ++(++at) ) {
		int p, q;
		
		// ������ ����
		ViIt copy_at;
		copy_at = at;
		p = *copy_at;
		q = *(++copy_at);
		
		// find
		if (id[p] == id[q]) {
			print(id, N);
			continue;	// ���� � ������(��?) - �� ������� � �� ���������
		}
			
		// union
		int t = id[p];
		for ( int i = 0; i < N; i++ )
			// ������ �������
			if (id[i] == t)
				id[i] = id[q];
		
		// ����������
		print(id, N);
		print(p, q);
	}
}

void quick_union( Vi vpq ) {
	// ��������� - ��������� �������������
	int i, id[N];
	for ( i = 0; i < N; i++)
		id[i] = i;
		
	ViIt at = vpq.begin(), end = vpq.end();
	// ������ �����
	for ( ; at != end; ++(++at) ) {
		int p, q;
		
		// ������ ����
		ViIt copy_at;
		copy_at = at;
		p = *copy_at;
		q = *(++copy_at);
		
		// find
		int i, j;
		for (i = p; i != id[i]; i = id[i]);
		for (j = q; j != id[j]; j = id[j]);
		if( i == j ) continue;
		id[i] = j;
		
		// ����������
		print(id, N);
		print(p, q);
	}
}

void print( int* array, int N ){
	//for (int i = 0; i < N; ++i)	cout << array[i] << " ";
	//cout << endl;
}
void print( int p, int q ) {
	cout << "Pair: " << p << " "<< q << endl;
}

int main() {
	Vi vpq;
	vpq.push_back(3);
	  vpq.push_back(4);
	vpq.push_back(4);
	  vpq.push_back(9);
	vpq.push_back(8);
	  vpq.push_back(0);
	vpq.push_back(2);
	  vpq.push_back(3);
	vpq.push_back(5);
	  vpq.push_back(6);
	vpq.push_back(2);
	  vpq.push_back(9);
	vpq.push_back(5);
	  vpq.push_back(9);
	vpq.push_back(7);
	  vpq.push_back(3);
	vpq.push_back(4);
	  vpq.push_back(8);
	vpq.push_back(5);
	  vpq.push_back(6);
	vpq.push_back(0);
	  vpq.push_back(2);
	vpq.push_back(6);
	  vpq.push_back(1);
	
	// ���������
	//quick_find(vpq);
	quick_union(vpq);
	
	// ������������ ��� g++
	return 0;
}

