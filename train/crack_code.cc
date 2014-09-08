
#include <iostream>
#include <string>
#include <algorithm>  // hm...

#include <string.h>
#include <assert.h>

using namespace std;

namespace arr_and_strings {

// Наверное за линейное время. Ничего быстрее хэш-таблиц нельзя
// Если сортировать входной массив, то будет a*N*logN + b*N*logN
bool there_unique(const string& line) {
  // по идее нужно сохранять
  // 
  // V0:
  //   где хранить - массив размером со строку
  //   но там искать O(n).
  //
  // V1:
  //   в хэштаблице быстро искать - O(1) - в качестве замены можно использовать просто массив по числу байт, а вычислять хэш
  //
  // V2:
  //   в отсортированном массиве - поиск за логарифмическое время - но нужно поддерживать инвариант
  //
  // V3: 
  //   делаем копию входного - сортируем - тогда размер фиксирован - но это пространство
}

// 1.2
// можно воспользоваться rotate
void reverse_string(char* line
  //, const int size
  ) {
  assert(line);
  int size = strlen(line);  // O(n)

  int half = size / 2;
  for (int i = 0; i < half; ++i) {
    swap(line[i], line[size - i - 1]);  // внутри создает переменную для обмена
  }
}

// 1.3 Remove dupl. chapters
// сложнее - нелзя использовать ничего доп. (большого дополнительного) - только локальные переменные
void unique_n2(char* str, const unsigned size) {
  // сортировать вроде бы нельзя
  //for ()
  // DANGER: precond. check
  assert(str);

  // work
  char tmp(str[0]);
  int tail(0);  // нужно одновременно удалять
  for (int i = 0; i < size; ++i) {

  }
}

}

int main() {
  string s("abcde");
  arr_and_strings::reverse_string(&s[0]);
  assert(s == "edcba");


  return 0;
}