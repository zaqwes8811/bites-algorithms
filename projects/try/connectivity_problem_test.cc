#include "gtest/gtest.h"
#include <boost/lexical_cast.hpp>

#include <map>
#include <vector>
#include <string>

typedef std::vector<int> Vi;
typedef std::vector<int>::iterator ViIt;

using namespace std;
static const int N = 20;

string quick_find( Vi vpq ) {
	string result;
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
            //print(id, N);
			continue;	// есть в наборе(ах?) - не выводим и не соединяем
		}
			
		// union
		int t = id[p];
		for ( int i = 0; i < N; i++ )
			// меняем местами
			if (id[i] == t)
				id[i] = id[q];
		
		// отчетность
        //print(id, N);
        //print(p, q);
        result += boost::lexical_cast<string>(p)+"-";
        result += boost::lexical_cast<string>(q)+" ";
	}
	return result;
}

string quick_union( Vi vpq ) {
	string result;
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
        //print("\nPair", p, q);
		
		// find
		int i, j;
		for (i = p; i != id[i]; i = id[i]) {
            //print( "i", i, "id[i]", id[i], "id[id[i]]", id[id[i]] );
		}
		for (j = q; j != id[j]; j = id[j]) {
            //print( "j", j, "id[j]", id[j], "id[id[j]]", id[id[j]] );
		}
		if( i == j ) {
            //print("Coupled\n");
			continue;
		}
        //print("No coupled\n");
        result += boost::lexical_cast<string>(p)+"-";
        result += boost::lexical_cast<string>(q)+" ";
		id[i] = j;
		
		// отчетность
        //print(id, N);
	}
	return result;
}

Vi g_getInVector() {
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
	
	//vector<string> fileContent = file2list("test-data.txt");
	//print(fileContent);
	
	return vpq;
}

TEST(ConnectivityTest, QuickUnionBase) {
	Vi inData = g_getInVector();
	string expect_result = "3-4 4-9 8-0 2-3 5-6 5-9 7-3 4-8 6-1 ";
	EXPECT_EQ( expect_result, quick_union(inData) );
}

TEST(ConnectivityTest, QuickFindBase) {
	Vi inData = g_getInVector();
	string expect_result = "3-4 4-9 8-0 2-3 5-6 5-9 7-3 4-8 6-1 ";
	EXPECT_EQ( expect_result, quick_find(inData) );
}
