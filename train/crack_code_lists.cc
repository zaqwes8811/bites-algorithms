 #include <gtest/gtest.h>

#include <iostream>
#include <string>
#include <algorithm>  // hm...
#include <vector>
#include <memory>
#include <stdexcept>
#include <list>

#include <string.h>
#include <assert.h>

using namespace std;


namespace lists {
class test
{
  public:
    // не нужне конструктор по умолчанию
    test(int, int) {}
    test(const test& rhs) {}
  private:
    test& operator=(const test&);
};
// FIXME: какие требования на T?
// - ctor() - No
// - copy_ctor() - Yes
// - ~dtor() nothrow() - Yes
// - !! безопасный оператор присваивания - No - не используется нигде
template <typename T>
class single_linked_list {
public:
  struct node
  {
    node(const T& _elem, node* const _next) : elem(_elem), next(_next) { 
      //throw runtime_error(""); 
    }

    T elem;
    node* next;  
    ~node() {
      //cout << "node dtor\n";
    }
  };

public:
  typedef T element_type;

  // construction/destruction
  single_linked_list() : head_(0), tail_(0)/*, attached_(true)*/ {}
  ~single_linked_list() {
    // удаляеть нужно!
    // http://stackoverflow.com/questions/3840582/still-reachable-leak-detected-by-valgrind
    //if (attached_) {
      node* ptr = head_;
      if (ptr) {
        while (ptr->next) {
          auto_ptr<node> _(ptr);  // удалит текущий при выходе из блока
          ptr = ptr->next;
        }

        // удалит последний
        auto_ptr<node> _(ptr);
      }
    //}
  }

  node* end() {
    // слудующий за - нулевой next
    return 0;  // ?
  }

  int size() const {
    // FIXME: сделать за линейное время, константное мешает врезке
    throw logic_error("Non impl. yet");
    return 0;
  }

  void push_back(const T& elem) {
    // FIXME: как быть с конструктором кидающим исключения? он сам должен как-то обрабатывать это?
    //   в противном случае есть ограничения на элементы, хранимые в контейнере
    // http://ders.stml.net/cpp/xcppcomm/xcppcomm.html#149
    //
    //auto_ptr<node> tmp(new node(elem, 0));
    // в принципе умный указатель не нужен, если есть конструктор
    //tmp->next = 0;
    //tmp->elem = elem;  // может бросить исключение

    // change state
    // если конструктор бросит искл. то new ...
    // FIXME: а что будет то? Деструктор может и не вызваться. Если и бросит, то состояние списка не изменится
    node* const ptr = 
        new node(elem, 0);
        //tmp.release();
    if (head_) {
      tail_->next = ptr;
      
      // соединяем голову с остальным, иначе будет разрыв
      // нет не будет, т.к. изначально head and tail указывают на один динамический элемент
      //   и что происходит с хвостом, то происходит и с головой
      //if (!head_->next)
        //head_->next = tail_;

      tail_ = ptr;
    } else {
      // создаем первый элемент
      head_ = tail_ = ptr;
      //head_->next = tail_;  // connect - при траверсе может быть ошибка - зациклится если элемент один
    }
  }

  // 2.2
  // список односвязный, поэтому можно двигаться только вперед
  node* get_nth_to_last(const int nth) const {
    assert(nth > 1);
    if (!head_)
      return 0;

    // двигаем окно
    node* p1 = head_;
    node* p2 = head_;

    int win = nth;
    // длинна списка - O(n)
    // лучше for - мы знаем сколько посторений!  сперва был while и плохая проверка гр. условий
    for (int i = 0; i < nth - 1; ++i) {
      if (!p2->next)
        return 0;  // нет столько элементов
      p2 = p2->next;  // указывает на nth+1
    }

    //if (win != 0)  // BAD: где локальность переменных?
    //  return 0;

    // двигаем полнстью раздвинутое окно [i, i+N]
    while (p2->next) {
      p2 = p2->next;
      p1 = p1->next;
    }
    return p1;
  }

private:
  // для реализации move - false - не владеет, и поэтому можно перемещать указатели
  // BAD: но как по другому?
  //void off_ownership_() {
  //  attached_ = false;
  //}

  // управление ресурсами - можно было бы сделать статически, но уже есть список, работающий с ними
  // удаляет цепочку
  // думаю поможет сделать безопасным конструктор копирования
  //static void delete_chain_(node* const head) /*nothrow()*/;
  // FIXME: create by copy chain
  // нужно вернуть два указателя
  //static node* create_new(const node* const src_head);

  template <typename U>
  friend ostream& operator<<(ostream& o, const single_linked_list<U>&);

public:
  // no copy and assign - по суте реализует move-семантику
  single_linked_list<T>& operator=(
    single_linked_list<T> rhs
    //const single_linked_list<T>& rhs
    ) {
    // http://stackoverflow.com/questions/12015156/what-is-wrong-with-checking-for-self-assignment-and-what-does-it-mean
    //if (this != &rhs) {
      //single_linked_list<T>(rhs)
      rhs.swap(*this);  // накопитель

      // nothrow()
      //tmp.off_ownership_();
      //head_ = tmp.head_;  // если бы это были автопоинтеры, то обнулять бы не пришлось
      //tail_ = tmp.tail_;

      //tmp.head_ = 0;
      //tmp.tail_ = 0;
    //}
    return *this;
  }

  void swap(single_linked_list<T>& rhs) {
    std::swap(head_, rhs.head_);
    std::swap(tail_, rhs.tail_);
  }

  // FIXME: сделать exception-safe - проблема с том, что если возникнет искл. при конструировании, то деструктор не вызовется
  //   и память утечет. Может catch(...) throw; Похоже не всегда катит. Если члены - values - то вообще проблема, см. у Саттера
  // это обычный конструктор по сути то.
  single_linked_list<T>(const single_linked_list<T>& rhs) : head_(0), tail_(0)/*, attached_(true)*/ {
    single_linked_list<T> tmp;  // накопитель    
    node* it = rhs.head_;
    if (it) {
      while (it->next) {
        tmp.push_back(it->elem);
        it = it->next;
      }
      // last
      tmp.push_back(it->elem);
    } 

    // FIXME: МОЖНО СДЕЛАТЬ И swap!!
    /*
      Matrix & operator = (const Matrix & m)
      {
        Matrix(m).swap(*this);
        return *this;
      }

      void swap(Matrix & m)
      {
        std::swap(dim, m.dim);
        std::swap(data, m.data);
      }
    */
    // nothrow()
    //tmp.off_ownership_();
    tmp.swap(*this);
    //head_ = tmp.head_;
    //tail_ = tmp.tail_;

    //tmp.head_ = 0;
    //tmp.tail_ = 0;
  }

private:
  // лучше сделать с конструктором, тогда можно будет положиться на new если возникнет исключение
  
  // пока не вставим первый соеденить их нельзя
  // Это указатели, исключения не кидаются!
  // FIXME: если использовать auto_ptr, то легче будет реализовать move семантику 
  //   кажется нужно только для этих указателей, а не для всех частей цепочки - но вообще стремно
  //   проблемы возникнут, т.к. некоторое время оба указателя ссылаются на один и тот же объект, хотя, по сути нам нужен только head
  //   Кажется ради удаления пары строчек, не стоит изголятся.
  node* head_;
  node* tail_;  // без него вставка за O(n)
  
  // просто обнуляем указатели и прочие данные
  //bool attached_;  // владеем ресурсами или нет - нужно для копирования и как бы move-семантики
};

template <typename T>
ostream& operator<<(ostream& o, const single_linked_list<T>& l) {
  typename single_linked_list<T>::node* it = l.head_;
  if (it) {
    while (it->next) {
      cout << it->elem << ", ";
      it = it->next;
    }
    cout << it->elem;
  }
  cout << endl;
  return o;
}

TEST(Crack, LinkedList) {
  single_linked_list<int> l;
  l.push_back(9);
  l.push_back(10);
  l.push_back(11);
  cout << l;

  single_linked_list<int> l_copy(l);
  cout << l_copy;

  single_linked_list<int> l_copy_assign;
  l_copy_assign = l_copy;
  cout << l_copy_assign;

  single_linked_list<test> l_t;
  l_t.push_back(test(0, 0));
}

TEST(Crack, Nth) {
  single_linked_list<int> l;
  l.push_back(9);
  l.push_back(10);
  l.push_back(11);
  l.push_back(13);
  l.push_back(14);
  cout << l;

  single_linked_list<int>::node* p = l.get_nth_to_last(2);
  assert(p);

  cout << p->elem << endl;

}

// Doubled linked list
// http://www.youtube.com/watch?v=k0pjD12bzP0&list=PLTxllHdfUq4dp5Hrsw3FJzI1nnlXBre0v

template <typename T>
std::ostream& operator<<(std::ostream& os, const list<T>& l)
{
  os << "list: ";
  for (typename list<T>::const_iterator forward = l.begin(); forward != l.end(); ++forward) {
    os <<  *forward << ", "; 
  }
  return os << endl;
}

// 2.1
// O(n^2)
TEST(Crack, List2_1) {
  int arr[] = {1, 3, 6, 7, 7, 8, 3, 9};
  list<int> l(arr, arr + sizeof arr / sizeof arr[0]);
  cout << l;

  list<int>::iterator finded_boundary = l.begin();

  // проходим все и удаляем..
  // если нашли, то удаляем! важный момент!! см. в Effective STL!!
  for (list<int>::iterator forward = l.begin(); forward != l.end();) {
    list<int>::iterator finded = find(l.begin(), forward, *forward);
    if (finded == forward) {
      ++forward;
    } else {
      forward = l.erase(forward);
    }
  }

  cout << l;

}

// 2.2 - nth-elem - Selection problem - нет - поиск nth c конца
//   DaC - наврядли сработает, т.к. нужен произвольный доступ

// 2.3 - не понял - похоже указатель уже дан

}