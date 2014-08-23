#include <gtest/gtest.h>

#include <vector> 
#include <ostream>
#include <string>

// inner
#include "visuality/view.h"

using namespace std;
using view::operator<<;

namespace {
pair<int, vector<pair<int, int> > > get_test_items(const string& filename) 
{
  int W = 6;
  vector<pair<int, int> > items;  // (vi, wi)
  items.push_back(make_pair(0, 0));
  items.push_back(make_pair(3, 4));
  items.push_back(make_pair(2, 3));
  items.push_back(make_pair(4, 2));
  items.push_back(make_pair(4, 3));
  return make_pair(W, items);
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
  pair<int, vector<pair<int, int> > > tmp = get_test_items("NoFile");
  vector<pair<int, int> > items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    
    // Значения объема может быть любым с точностью до единицы
    for (int x = 0; x <= W; ++x) {
      int w_boundary = x - items[i].second;
      int v = items[i].first;
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
  // Очень большой объем данных
  // - Рекурсивная реализация
  // - Использовать только 2 колонки - скорости это врядли прибавит
  // - Образать веса которых не может быть - как в задаче из 1 части
  // - Хэш таблица похоже безсмысленна, она реализована на основе массива - хотя...
  // - Сортировка
  pair<int, vector<pair<int, int> > > tmp = get_test_items("NoFile");
  vector<pair<int, int> > items = tmp.second;
  int W = tmp.first;
  
  
  // A[0,0]
  int N = items.size();
  vector<vector<int> > A(N, vector<int>(W+1, 0)); 
  for (int i = 1; i < N; ++i) {
    const int v = items[i].first;
    const int w = items[i].second;
    
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

// 
TEST(W3, Genome) 
{
  
}

TEST(W3, OptBST) 
{
  // Скорее всего тольков в теории
}