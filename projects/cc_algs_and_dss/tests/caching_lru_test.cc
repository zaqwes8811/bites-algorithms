//http://stackoverflow.com/questions/12561439/c-inmemory-cache-library
//
// http://stxxl.sourceforge.net/

#include <gtest/gtest.h>

#include <functional>
#include <map>  // small caches
#include <list>

// FIXME: how make exception safe
template <typename K, typename V>
class LRUCache {
public:
  typedef std::function<V(K)> Operation;

  explicit LRUCache(Operation op) :
    m_op(op)
    , k_maxSize(2) { }

  // It's bad return from setter by value
  //V
  void get(const K& key, V& r_v) {
    // if contain
    if (m_store.find(key) != m_store.end())
      r_v = m_store[key];

    // not contain
    V val = m_op(key);
    m_store[key] = val;

    try {
      // if size to big remove
      if (m_store.size() > k_maxSize) {
        K steal = m_dll.front();
        m_store.erase(steal);
        m_dll.pop_front();
      }
    } catch(...) {
      throw;
    }

    // just push
    m_dll.push_back(key);

    r_v = val;
  }

private:
  Operation m_op;
  const size_t k_maxSize;

  // DLL
  std::list<K> m_dll;

  // Key-Value store
  std::map<K, V> m_store;

  // non-copyable
  LRUCache(const LRUCache&);
  LRUCache& operator=(const LRUCache&);
};

int get(int key) {
  return key;
}

TEST(DSS, Cache) {
  LRUCache<int, int> v(&get);

  int val = 0;
  v.get(0, val);
}
