/// Concurent core info:
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2406.html
//
// coupled - Queue
// https://www.quantnet.com/threads/c-multithreading-in-boost.10028/
// http://stackoverflow.com/questions/10139251/shared-queue-in-c
//
// concurent queue
// http://stackoverflow.com/questions/16275112/boost-c-lock-free-queue-vs-shared-queue
// http://www.alexey-martynov.pp.ru/index.php?data=articles&article=mt-queue
// http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
// http://stackoverflow.com/questions/6959023/c-producer-consumer-queue-with-very-fast-and-reliable-handover
// http://blog.ruslans.com/2013/08/introduction-to-high-level.html
//
//
// Finded tut.:
//   https://computing.llnl.gov/tutorials/pthreads/
//   https://computing.llnl.gov/tutorials/parallel_comp/
//
//
// Concurent code style
//   http://www.codingstandard.com/section/18-concurrency/

// C
#include <pthread.h>

// C++
#include <list>
#include <ostream>

// C++11
#include <mutex>
#include <thread>

// 3rdparty
#include <gtest/gtest.h>


// !!
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_algorithms.hpp>

using std::list;

class Fake {
public:
  Fake() {
    std::cout << "ctor\n";
  }
  Fake(const Fake& rhs) {
    std::cout << "copy ctor\n";
  }
  Fake& operator=(const Fake& rhs) {
    std::cout << "assign operator\n";
    return *this;
  }
private:
  Fake(Fake&&) = delete;
  Fake& operator=(Fake&&) = delete;
};

// Part1
// http://www.ibm.com/developerworks/ru/library/au-multithreaded_structures1/
// No exception safe
namespace concurent {
template <typename T>
class QueuePthread {
public:
  QueuePthread() {
    pthread_mutex_init(&_lock, NULL);
  }
  ~QueuePthread() {
    pthread_mutex_destroy(&_lock);
  }
  
  void push(const T& value) {
    pthread_mutex_lock(&_lock);
    _list.push_back(value);  // лучше врезка нового списка и вне критической секции
    pthread_mutex_unlock(&_lock);
  }
  
  // вообще из сетторов по значению возвращать не стоит
  // но операция должна быть атомарной
  T pop(/*T& r - тоже плохо, т.к. возвращаем хэндл, нет это возвращение ссылки на то чего быть не должно*/) {
    // TODO: а то что не под локом это нормально?
    if (_list.empty()) {
      throw "Elem not founded.";
    }
    
    pthread_mutex_lock(&_lock);
    T _temp = _list.front();
    _list.pop_front();
    pthread_mutex_unlock(&_lock);
    return _temp;
  }
  
  //  Плохо, т.к. возвращаем хэндл - нет - может не так и плохо
  // нет это возвращение ссылки на то чего быть не должно - это из Саттера, но не про то - T& pop();
  // 
  // похоже этот вариант лучший
  void pop(T& r) {
    // TODO: а то что не под локом это нормально?
    if (_list.empty()) {
      throw "Elem not founded.";
    }
    
    pthread_mutex_lock(&_lock);
    T _temp = _list.front();
    r = _temp;
    _list.pop_front();
    pthread_mutex_unlock(&_lock);
  }
  
private:
  // DANGER: !!!
  QueuePthread(const QueuePthread&);
  QueuePthread& operator=(const QueuePthread&);
  
  list<T> _list;
  pthread_mutex_t _lock;
};

// No correct:
// http://codereview.stackexchange.com/questions/41604/thread-safe-concurrent-fifo-queue-in-c
template <typename T>
class QueuePthreadV2 {
public:
  QueuePthreadV2() {
    pthread_mutex_init(&_rlock, NULL);
    pthread_mutex_init(&_wlock, NULL);
  }
  ~QueuePthreadV2() {
    pthread_mutex_destroy(&_rlock);
    pthread_mutex_destroy(&_wlock);
  }
  
  void push(const T& value) {
    pthread_mutex_lock(&_wlock);
    _list.push_back(value);
    pthread_mutex_unlock(&_wlock);
  }
  
  // вообще из сетторов по значению возвращать не стоит
  // но операция должна быть атомарной
  T pop() {
    // TODO: а то что не под локом это нормально?
    if (_list.empty()) {
      throw "Elem not founded.";
    }
    
    pthread_mutex_lock(&_rlock);
    T _temp = _list.front();
    _list.pop_front();
    pthread_mutex_unlock(&_rlock);
    return _temp;
  }
  
private:
  list<T> _list;
  
  // DANGER: !!!
  QueuePthreadV2(const QueuePthreadV2&);
  QueuePthreadV2& operator=(const QueuePthreadV2&);
  
  // http://hardclub.donntu.edu.ua/projects/qt/qq/qq11-mutex.html
  pthread_mutex_t _rlock, _wlock;  // TODO: а как так? А не должен быть один?
};
}

// Part2
// http://www.ibm.com/developerworks/aix/library/au-multithreaded_structures2/index.html?ca=d
template <typename T>
class SList {
private:
  typedef struct Node {
    T data;
    Node *next;
    Node(T& value) : data(value), next(NULL) { }
  } Node;
  
  pthread_mutex_t _lock;
  Node *head, *tail;
  
public:
  void push_back(T& value) {
    pthread_mutex_lock(&_lock);
    
    
    pthread_mutex_unlock(&_lock);
  }
  void insert_after(T& previous, T& value);
  void remove(const T& value);
  bool find(const T&value);
  SList();
  ~SList();
};

// TODO: очередь от Шена
// http://channel9.msdn.com/Events/GoingNative/2013/Cpp-Seasoning
//
// В вопросах кто-то не лестно отозвался о реализации, но что он сказал?
template <typename T>
class concurent_queue 
{
  std::mutex mutex_;
  list<T> q_;
public:
  void enqueue(T x) {
    // allocation here
    list<T> tmp;
    tmp.push_back(move(x));
    
    // threading
    {
      std::lock_guard<std::mutex> lock(mutex_);
      // вроде бы константное время
      q_.splice(end(q_), tmp);
      
      // для вектора может неожиданно потреб. реаллокация
    }
  }
  
  // ...
};

void foo(Fake& fake) {
  Fake tmp;
  fake = tmp;  // assign
}
TEST(STL, LRefThinking) {
  Fake f;
  foo(f);
}
 
// TODO:
// thread-safe copy ctor and assign
// http://stackoverflow.com/questions/5070161/designing-a-thread-safe-copyable-class
// http://home.roadrunner.com/~hinnant/mutexes/locking.html
// Summary:
//   лучше сперва вообще запретить.
// http://stackoverflow.com/questions/13030272/how-to-implement-an-atomic-thread-safe-and-exception-safe-deep-copy-assignment
class A
{
  mutable std::mutex  mut_;
  std::vector<double> data_;

public:
  A() {}
  // ...

  A& operator=(const A& rhs)
  {
    if (this != &rhs)  // важно, чтобы не было самоблокировки
    {
      // Так захватывает он лок или нет?  
      std::unique_lock<std::mutex> lhs_lock(mut_,     std::defer_lock);
      std::unique_lock<std::mutex> rhs_lock(rhs.mut_, std::defer_lock);
      std::lock(lhs_lock, rhs_lock);  // похоже правильно обрабатывает a = a
      
      // V0
      //std::unique_lock<std::mutex> lhs_lock(mut_);
      //std::unique_lock<std::mutex> rhs_lock(rhs.mut_);  // deadlock!
        // assign data ...
      data_ = rhs.data_;
    }
    return *this;
  }

  
private:
  A(const A&);  // !!
    // ...
};
void task1(A* a, A* b)
{
    //std::cout << "task1 says: " << msg;
    for (int i = 0; i < 100000; ++i) {
      *a = *b;  // deadlock - v0
      ;
    }
}

class B {
  typedef boost::shared_mutex mutex_type;
  typedef boost::shared_lock<mutex_type> SharedLock;
  typedef boost::unique_lock<mutex_type> ExclusiveLock;
  
  // TODO: shared_lock = read_lock?
  
  mutable mutex_type mut_;
  std::vector<double> data_;

public:
  B& operator=(const B& rhs)
    {
        if (this != &rhs)
        {
            // assign data ...
            // expensive code here ... !!!
            ExclusiveLock lhs_lock(mut_, boost::defer_lock);  // свой эксклюзивный
            SharedLock rhs_lock(mut_, boost::defer_lock);
            boost::lock(lhs_lock, rhs_lock);
            data_ = rhs.data_;
        }
        return *this;
    }
//private:
  B(const B& rhs) {
    SharedLock _(rhs.mut_);
    data_ = rhs.data_;
  }
};

TEST(DS, Mutex) {
  std::mutex mut;
  
  // http://stackoverflow.com/questions/20516773/stdunique-lockstdmutex-or-stdlock-guardstdmutex
  std::unique_lock<std::mutex> lk(mut, std::defer_lock);
  assert(lk.owns_lock() == false);
  
  A a, b;
  
  std::thread t1(task1, &a, &b);
  std::thread t2(task1, &b, &a);
  std::thread t3(task1, &a, &a);
  std::thread t4(task1, &a, &a);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}

// TODO: Try all
// http://www.boost.org/doc/libs/1_55_0/doc/html/thread/synchronization.html

// TODO: из толков от яндекса
// https://tech.yandex.ru/events/yagosti/cpp-user-group/talks/1798/
// Для контейнеров нужна внешняя синхронизация.
//
// ref base and value base - похоже не то же самое что смартпоинтеры в контейнерах
//
// Нельзя зывать чужой код под "замком"!!
/*
void push(const T& t){
  //node* p_node = new node(t);  // TODO: сделать безопасным в плане искл.
  // http://www.gotw.ca/publications/using_auto_ptr_effectively.htm
  auto_ptr<node> p_node(new node(t));
  lock_guard lck(mtx);
  
  p_node->next = head;
  
  head = p_node.release();  // not get!!
}
// когда забираем из стека, то тоже можно захватить auto_ptr'ом
*/

// TODO: интерфейс С++11 потокобезопасных контейнеров.
// http://stackoverflow.com/questions/15278343/c11-thread-safe-queue

