//http://stackoverflow.com/questions/12561439/c-inmemory-cache-library
//
// http://stxxl.sourceforge.net/

#include <gtest/gtest.h>

#include <functional>
#include <map>  // small caches
#include <list>

// FIXME: http://www.careercup.com/question?id=5120338362892288
// FIXME: how make exception safe
// FIXME: May be better store value in list, not in K/V
//   then may be easer up performane on exception-safe condition.
//
// FIXME: API - https://code.google.com/p/guava-libraries/wiki/CachesExplained
template <typename K
          , typename V  // copy ctor/assign operator; default ctor not needed
          >
class lru_cache {
public:
  typedef std::function<V(K)> CacheLoader;

  explicit lru_cache(CacheLoader op) :
    m_load(op)
    , k_maxSize(2) { }

  // It's bad return from setter by value
  //V
  void get(const K& key, V& r_v) {
    // if contain - easy
    if (m_store.find(key) != m_store.end())
      r_v = m_store[key];

    // not contain
    V val = m_load(key);

    {
      // without tmp too hard - much roolbacks
      // FIXME: bad performance and memory usige (not O(1) space and big calc)

      // throw part
      Store tmp_store = m_store;
      std::list<K> tmp_dll = m_dll;

      // if it can rollback then easy
      // Need value to roolback - Ok
      // Need put K/V back - can throw!
      if (tmp_store.size() == k_maxSize) {
        K steal(tmp_dll.front());
        tmp_store.erase(steal);
        tmp_dll.pop_front();
      }

      tmp_dll.push_back(key);  // looks like we can safe roolback
      //tmp_store[key] = val; // Trouble if put key then put value throw. What happened?
      //  need default ctor
      tmp_store.insert(std::make_pair(key, val));

      // no throw
      std::swap(m_store, tmp_store);
      std::swap(m_dll, tmp_dll);
    }

    r_v = val;
  }

private:
  typedef std::map<K, V> Store;

  CacheLoader m_load;
  const size_t k_maxSize;

  // DLL
  std::list<K> m_dll;

  // Key-Value store
  Store m_store;

  // non-copyable
  lru_cache(const lru_cache&);
  lru_cache& operator=(const lru_cache&);
};

int get(int key) {
  return key;
}

TEST(DSS, Cache) {
  lru_cache<int, int> v(&get);

  int val = 0;
  v.get(0, val);
}
