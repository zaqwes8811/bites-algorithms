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
	// заполняем - начальная инициализация
	int i, id[N];
	for ( i = 0; i < N; i++)
		id[i] = i;
		
	ViIt at = vpq.begin(), end = vpq.end();
	// шагаем подва
	for ( ; at != end; ++(++at) ) {
		int p, q;
		
		// читаем пары
		ViIt copy_at;
		copy_at = at;
		p = *copy_at;
		q = *(++copy_at);
		
		// find
		if (id[p] == id[q]) {
			print(id, N);
			continue;	// есть в наборе(ах?) - не выводим и не соединяем
		}
			
		// union
		int t = id[p];
		for ( int i = 0; i < N; i++ )
			// меняем местами
			if (id[i] == t)
				id[i] = id[q];
		
		// отчетность
		print(id, N);
		print(p, q);
	}
}

void quick_union( Vi vpq ) {
	// заполняем - начальная инициализация
	int i, id[N];
	for ( i = 0; i < N; i++)
		id[i] = i;
		
	ViIt at = vpq.begin(), end = vpq.end();
	// шагаем подва
	for ( ; at != end; ++(++at) ) {
		int p, q;
		
		// читаем пары
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
		
		// отчетность
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
	
	// запускаем
	//quick_find(vpq);
	quick_union(vpq);
	
	// формальность для g++
	return 0;
}

