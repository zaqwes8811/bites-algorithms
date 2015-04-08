// http://people.cs.clemson.edu/~bcdean/dp_practice/
//
// "Two Ways of Thinking about Dynamic Programming... "
//
// Тренироваться только на решенных, иначе не ясно. Нужно сверяться
//
#include "visuality/view.h"

#include <gtest/gtest.h>

#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;
using namespace boost;
using namespace view;

TEST(Dyn, MaxGrow) {
  // http://e-maxx.ru/algo/longest_increasing_subseq_log
  int arr[] = {5, 2, 8, 6, 3, 6, 9, 7};
  vector<int> v(arr, arr+8);

  //list<int>
  vector<int> d(v.size(), 0);
  for (int i=0; i<v.size(); ++i) {
    d[i] = 1;

    // поиск со стиранием значения
    for (int j = 0; j < i; ++j)
      if (v[j] < v[i]) // включаем в ответ
        d[i] = max (d[i], 1 + d[j]);
  }
  cout << v;
  cout << d;
  // 
}

TEST(Dyn, MaxCont) {
  // http://e-maxx.ru/algo/maximum_average_segment
  //
  // Если на каком-то этапе не возрастает, то не обязательно она потом не возрастет. Это не как в жадном
  //
  // vn in Opt
  //
  // vn not in Opt
  //
  // Решение херня!
  //
  int arr[] = {5, 15, -30, 10, -5, 40, 10, -10, -11};//, 10, -5, 40, 11};
  vector<int> v(arr, arr+sizeof arr / sizeof arr[0]);
  cout << v;

  vector<int> s(v.size(), 0);  // +1
  vector<int> d(v.size()+1, 0);
  partial_sum(v.begin(), v.end(), /*++*/s.begin());  // постоянно считаем их - сейчас считаем за раз
  cout << s;

  // s[r] - s[l-1] -> max -> нужно вычесть минимум -> min(s[0, r-1])
  // Solve
  // значение суммы может потерятья, а оно нужно для поиска мимимуму - не все берется в решение
  // sum += v[r];  // s[r] int sum = 0;
  d[0] = v[0];
  for (int r=0; r < v.size(); ++r) {
    int max_sum_r_elem = s[r] - *min_element(s.begin(), s.begin()+r);  // как считать - все равно. можно и локальными переменными
    d[r+1] = max(d[r], max_sum_r_elem);  
  }

  // Reconstruct - только по dr не вышло
  int ans_l = 0, ans_r = 0;
  int value = d.back();  // не проверяю не пустоту
  for (int r=0; r < d.size() - 1; ++r) {
    // ищем последний перепад - если активности нет, до конца, то последовательность найдена
    if (d[r+1] != d[r]) {
      ans_r = r+1;  // не вкл. )
    }
  }

  ans_l = ans_r-1;
  while (true) {
    value -= v[ans_l];
    if (!value)
      break;
    --ans_l;
  }
  

  // Result
  cout << "Result: " << d.back() << " : ";
  assert(ans_l <= ans_r);
  cout << vector<int>(v.begin()+ans_l, v.begin()+ans_r);

  cout << d;
  // 
}

// cout << x[i] << " " << delta_i << " " << delta_ni << endl;
// FIXME: А корректен ли он? Тут наврное правильнее оптимален
TEST(Dyn, Way) {
  int arr[] = {0, 5, 5, 5, 5, 50, 50, 50, 200, 300}; 
  vector<int> v(arr, arr+sizeof arr / sizeof arr[0]);
  cout << v;

  vector<int> x(v.size(), 0);
  vector<int> cost(v.size(), 0);

  // на жадный похож - смотрим вперед
  for (int i = 1; i < v.size(); ++i) {
    int tmp_ni = (200 - (x[i-1]+v[i]));
    int tmp_i =  (200 - (     v[i]));
    int delta_ni = tmp_ni * tmp_ni;
    int delta_i =  tmp_i  * tmp_i;

    // это мы ищем локальный оптимум?
    cost[i] = std::min(cost[i-1] + delta_i, cost[i-1] + delta_ni);
    
    // разбираемся с путем - включаем ли предыдущую точку в решение или нет
    if (cost[i] == cost[i-1] + delta_i) {
      // текущая точка включена
      x[i] = v[i];
    } else {
      x[i] = x[i-1] + v[i];  
    }
  }

  cout << endl;
  cout << cost;
  cout << x;

  // brutforce
  //for ()
}

// http://habrahabr.ru/post/191498/
// 1D, 2D ...
//
// http://comp-science.narod.ru/WebPage/lesson2.htm
//
// LCS: http://en.wikipedia.org/wiki/Longest_common_subsequence_problem
// Наибольшая общая подпоследовательность
//
// Лекция 7. Динамическое программирование с одним параметром Михаил Густокашин
// http://neerc.ifmo.ru/wiki/index.php?title=%D0%94%D0%B8%D0%BD%D0%B0%D0%BC%D0%B8%D1%87%D0%B5%D1%81%D0%BA%D0%BE%D0%B5_%D0%BF%D1%80%D0%BE%D0%B3%D1%80%D0%B0%D0%BC%D0%BC%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B5
// create
// reconstr

// Coin change
// http://www.algorithmist.com/index.php/Coin_Change
