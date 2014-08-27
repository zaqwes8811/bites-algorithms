// Передает пакеты и prodecer thread to consimer
// Так похоже правильнее было реализовать поток данных
//   из интерфейса в базу данных
//
//
// Deamonds:
//   Task parallelize
//   каждый пакет должн быть обработан
//   io-thread развязан от потока обработки
//   correct concurent read form database
//
// Info:
// http://web.stanford.edu/class/cs140/cgi-bin/lecture.php?topic=process
// http://programmers.stackexchange.com/questions/206962/should-multi-threading-be-used-for-tasks-which-does-not-involve-io-operation
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

// C
#include <pthread.h>

// C++
#include <list>

// C++11
#include <mutex>

// 3rdparty
#include <gtest/gtest.h>

using std::list;

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
    _list.push_back(value);
    pthread_mutex_unlock(&_lock);
  }
  
  // вообще из сетторов по значению возвращать не стоит
  // но операция должна быть атомарной
  T pop() {
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
  
private:
  list<T> _list;
  pthread_mutex_t _lock;
};

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

// TODO:
// thread-safe copy ctor and assign
// http://stackoverflow.com/questions/5070161/designing-a-thread-safe-copyable-class
// http://home.roadrunner.com/~hinnant/mutexes/locking.html
// Summary:
//   лучше сперва вообще запретить.
// http://stackoverflow.com/questions/13030272/how-to-implement-an-atomic-thread-safe-and-exception-safe-deep-copy-assignment

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

