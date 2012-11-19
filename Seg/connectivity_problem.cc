#include <iostream>
#include <map>
#include <vector>
void print( int* array, int N );
void print( int, int );
void print( std::string, int, int );
void print( std::string , int );
void print( std::string );
void print( std::string , int, std::string , int );
void print( std::string , int, std::string , int, std::string , int );
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
		print("\nPair", p, q);
		
		// find
		int i, j;
		for (i = p; i != id[i]; i = id[i]) {
			print( "i", i, "id[i]", id[i], "id[id[i]]", id[id[i]] );
		}
		//print("\n");
		for (j = q; j != id[j]; j = id[j]) {
			print( "j", j, "id[j]", id[j], "id[id[j]]", id[id[j]] );
		}
		if( i == j ) {
			print("Coupled\n");
			continue;
		}
		print("No coupled\n");
		id[i] = j;
		
		// отчетность
		print(id, N);
	}
}

void print( int* array, int N ){
	//for (int i = 0; i < N; ++i)	cout << array[i] << " ";
	//cout << endl;
}
void print( int p, int q ) {
	cout << "Pair: " << p << " "<< q << endl;
}

void print( std::string name, int i ) {
	cout << name+" : " << i << endl;
}

void print( std::string str ) {
	cout << str;
}

void print( std::string msg, int p, int q ) {
	cout << msg+" : " << p << " "<< q << endl;
}

void print( std::string m1, int a1, std::string m2, int a2 ) {
	cout << m1+" : " << a1 << " " << m2+" : " << a2 << endl;
}
void print( std::string m1, int a1, std::string m2, int a2,
	std::string m3, int a3
 ) {
	cout << m1+" : " << a1 << 
	  " " << m2+" : " << a2 << 
	  " " << m3+" : " << a3 << 
	  endl;
}

int main() {
	Vi vpq;
	vpq.push_back(3); vpq.push_back(4);
	vpq.push_back(4); vpq.push_back(9);
	vpq.push_back(8); vpq.push_back(0);
	vpq.push_back(2); vpq.push_back(3);
	vpq.push_back(5); vpq.push_back(6);
	vpq.push_back(2); vpq.push_back(9);
	vpq.push_back(5); vpq.push_back(9);
	vpq.push_back(7); vpq.push_back(3);
	vpq.push_back(4); vpq.push_back(8);
	vpq.push_back(5); vpq.push_back(6);
	vpq.push_back(0); vpq.push_back(2);
	vpq.push_back(6); vpq.push_back(1);
	
	// запускаем
	//quick_find(vpq);
	quick_union(vpq);
	
	// формальность для g++
	return 0;
}

