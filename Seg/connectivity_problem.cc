#include <iostream>
#include <map>
using namespace std;
static const int N = 10000;

void quick_find( int p, int q ) {}
void quick_union(){}

int main() {
	int i, p, q, id[N];
	map<int, int> vpq;
	vpq.insert(pair<int, int>(3,4));
	vpq.insert(pair<int, int>(4,9));
	vpq.insert(pair<int, int>(8,0));
	vpq.insert(pair<int, int>(2,3));
	vpq.insert(pair<int, int>(5,6));
	vpq.insert(pair<int, int>(2,9));
	vpq.insert(pair<int, int>(5,9));
	vpq.insert(pair<int, int>(7,3));
	vpq.insert(pair<int, int>(4,8));
	vpq.insert(pair<int, int>(5,6));
	vpq.insert(pair<int, int>(0,2));
	vpq.insert(pair<int, int>(6,1));
	
	// заполняем - начальная инициализация
	for ( i = 0; i < N; i++)
		id[i] = i;
		
	while (cin >> p >> q) {
		
		// find
		int t = id[p];
		if (t == id[q])
			continue;	// есть в наборе - не выводим и не соединяем
			
		// union
		for ( i = 0; i < N; i++)
			if (id[i] == t)
				id[i] = id[q];
				
		// рисуем
		cout << " " << p << " "<< q << endl;
	}
	
	return 0;
}
