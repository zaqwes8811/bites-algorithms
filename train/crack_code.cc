
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
// похож на жадный? Greedy?
// информация есть обо всех - они в начале массива, и мы удалим если будет второе вхождение
// Инвариант - все до тайла уникальные
// Если сдвигать и идти до конца то похое - O(n^3)
void unique_n2(char* str, const unsigned size) {
  // сортировать вроде бы нельзя
  //for ()
  // DANGER: precond. check
  assert(str);

  // work
  int end(1);  // нужно одновременно удалять, т.е. сдвигать?
  for (int current = 1; current < size; ++current) {
    
    int finded = 0;
    const char elem = str[current];

    // ищем в нашей псевдотаблице
    while (finded < end) {
      if (str[finded] == elem) 
        break;  // нашли. просто переходим к следующему элементу
      ++finded;
    }

    // не нашел. добавить
    if (finded == end) {
      str[end] = elem;
      ++end;
    }

  }
  str[end] = 0;  // все что за ним уже мусор
}

// 1.4

// 1.6
struct Pixel {
  char r;
  char g;
  char b;
  char i;
};

void rotate_img_90() {
  //
  Pixel tmp[4];  // очередь из 4 пикселей  
}

// 1.7 - однуления столбцов
// будет какое-то ускорение - часть удалиться... 
// все может начать зависеть от пути обхода! нужно пройти несколько раз

}

int main() {
  string s("abcde");
  arr_and_strings::reverse_string(&s[0]);
  assert(s == "edcba");

  char str[] = "aabbcc";
  //char* str = "aabbcc";  // Seg. fault
  arr_and_strings::unique_n2(str, strlen(str));
  assert(string(str) == "abc");

  return 0;
}