#include <map>
#include <vector>
#include "utils.h"
#include "gtest/gtest.h" 

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

int main(int argc, char* argv[]) {
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
	
	testing::InitGoogleTest(&argc, argv);
	// Принудительно печатаем время работы тестов.
	testing::GTEST_FLAG(print_time) = true;
	//return 
	RUN_ALL_TESTS();
  
	// формальность для g++
	return 0;
}

