#include <gtest/gtest.h>

// C++98
#include <vector> 
#include <ostream>
#include <string>

// C++11
#include <unordered_map>

// inner
#include "visuality/view.h"

using namespace std;
using view::operator<<;

namespace {
  
struct Item {
  Item() : v(0), w(0) {}
  Item(int _v, int _w) : v(_v), w(_w) {}
  
  int v;
  int w;
};
  
pair<int, vector<Item> > get_test_items(const string& filename) 
{
  int W = 6;
  vector<Item> items;  // (vi, wi)
  items.push_back(Item(0, 0));
  items.push_back(Item(3, 4));
  items.push_back(Item(2, 3));
  items.push_back(Item(4, 2));
  items.push_back(Item(4, 3));
  return make_pair(W, items);
}
  
// Returns the maximum value that can be put in a knapsack of capacity W
int knapSackExp(int current_boundary, int n, const vector<Item>& items)
{
  // Base Case
  if (n == 0 || current_boundary == 0)
      return 0;

  // If weight of the nth item is more than Knapsack capacity W, then
  // this item cannot be included in the optimal solution
  cout << current_boundary << " "<< n << endl;
  if (items[n-1].w > current_boundary)
      return knapSackExp(current_boundary, n-1, items);

  // Return the maximum of two cases: (1) nth item included (2) not included
  else {
    int sum_values = std::max( 
      knapSackExp(current_boundary - items[n-1].w, n-1, items) + items[n-1].v, 
      knapSackExp(current_boundary,                n-1, items));

    return sum_values;
    
  }
    
}

int knapSack(int W, int wt[], int val[], int n)
{
   // Base Case
   if (n == 0 || W == 0)
       return 0;
 
   // If weight of the nth item is more than Knapsack capacity W, then
   // this item cannot be included in the optimal solution
   //cout << W << " "<< n << endl;
   if (wt[n-1] > W)
       return knapSack(W, wt, val, n-1);
 
   // Return the maximum of two cases: (1) nth item included (2) not included
   else 
     return std::max( 
      knapSack(W - wt[n-1], wt, val, n-1) + val[n-1],
      knapSack(W,           wt, val, n-1));
}
}

TEST(W3, Wis) 
{
  int arr[] = {0, 1, 4, 5, 4};  // 0 - guard
  vector<int> W(arr, arr + sizeof(arr)/sizeof(arr[0]));
  cout << W;

  // Work
  vector<int> A(W.size(), 0);
  A[1] = W[1]; 
  for (size_t i = 2; i < W.size(); ++i) {
    // верширу либо добавляем либо
    A[i] = std::max(A[i-1], A[i-2] + W[i]);
    cout << A;
  }
  
  // Reconstruction solution 
  int i = W.size() - 1;
  while (i >= 1) {
    // Рассматриваем подзадачи, но сам вес из текущего узла
    // Похоже на рассуждение при разработке алгоритма
    if (A[i-1] >= A[i-2] + W[i]) {
      // Веришина была исключена
      --i;
    } else {
      // Веришина была включена, и нужно перескочить
      cout << W[i] << " " << i << endl;
      i -= 2;
    }
  }
  
}

TEST(W3, KnapsackProblem) 
{
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    
    // Значения объема может быть любым с точностью до единицы
    for (int x = 0; x <= W; ++x) {
      int w_boundary = x - items[i].w;
      int v = items[i].v;
      if (w_boundary < 0) {
        w_boundary = x;
        v = 0;
      }
      
      // Read
      int invalid = A[i-1][x];
      int valid = A[i-1][w_boundary];
      
      // Write
      A[i][x] = std::max(invalid, valid + v);
    }
  }
  
  cout << " " << A;

  // Reconstruction
}

TEST(W3, BigKnapsackProblem) 
{
  // V(i, x) = max(V(i-1, x), v(i) + V(i-1, x-w(i))
  // if w(i) > x 
  //   V(i, x) = V(i-1, x)
  //
  //
  // Очень большой объем данных
  // - Рекурсивная реализация
  // - Использовать только 2 колонки - скорости это врядли прибавит
  // - Образать веса которых не может быть - как в задаче из 1 части - похоже в алгоритме уже есть фильтр
  // - Хэш таблица похоже безсмысленна, она реализована на основе массива - хотя...
  // - Сортировка
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    const int v = items[i].v;
    const int w = items[i].w;
    
    // Значения объема может быть любым с точностью до единицы
    // Итерация по емкостям
    for (int x = 0; x <= W; ++x) {
      int w_boundary = x - w;
      
      if (w_boundary < 0) {
        A[i][x] = A[i-1][x];  // просто копирование
      } else {
        A[i][x] = std::max(A[i][x], A[i-1][w_boundary] + v);
      }
    }
  }
  
  cout << " " << A;
}

TEST(W3, GeeksForGeek) 
{ 
  pair<int, vector<Item > > tmp = get_test_items("NoFile");
  vector<Item> items = tmp.second;
  int W = tmp.first;

  int count = items.size();
  printf("sum(v(i)) = %d \n", knapSackExp(W, count, items));
}

TEST(W3, GeeksForGeekSrc) {
    int val[] = {60, 100, 120};
    int wt[] = {10, 20, 30};
    int  W = 50;
    int n = sizeof(val)/sizeof(val[0]);
    printf("%d", knapSack(W, wt, val, n));

}


// 
TEST(W3, Genome) 
{
  
}

TEST(W3, OptBST) 
{
  // Скорее всего тольков в теории
}