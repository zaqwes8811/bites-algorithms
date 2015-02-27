//http://stackoverflow.com/questions/12561439/c-inmemory-cache-library
//
// http://stxxl.sourceforge.net/

#include <gtest/gtest.h>

#include <functional>
#include <map>  // small caches
#include <list>
#include <algorithm>

// FIXME: look like bad domain model!
//
// FIXME: http://www.careercup.com/question?id=5120338362892288
// FIXME: how make exception safe
// FIXME: May be better store value in list, not in K/V
//   then may be easer up performane on exception-safe condition.
//
// FIXME: API - https://code.google.com/p/guava-libraries/wiki/CachesExplained
//
// http://codereview.stackexchange.com/questions/4806/an-lru-cache-template
//
// Impl: http://timday.bitbucket.org/lru.html
//
// FIXME: looks like remove must be consistent - put can on next get operation
// http://openmymind.net/Writing-An-LRU-Cache/
template <typename K
          , typename V  // copy ctor/assign operator; default ctor not needed
          >
class lru_cache {
public:
  typedef std::function<V(K)> CacheLoader;

  explicit lru_cache(CacheLoader op)
    : m_load(op)
    , k_maxSize(2) { }

  // It's bad return from setter by value
  //V
  // Must be O(1)/O(1) cpu/mem
  // FIXME: bad performance and memory usige (not O(1) space and big calc)
  void get(const K& key, V& r_v)
  {
    // if contain - easy
    if (m_store.find(key) != m_store.end()) {
      r_v = m_store[key];  // O(log n) or O(1)

      // Need update dll
      auto key_pos = std::find(m_dll.begin(), m_dll.end(), key);  // O(n)

      // Move key to top.
      // swap is bad, need move to back - list::splice
      //assert(key_pos != m_dll.end());
      //assert(!m_dll.empty());
      std::swap(*key_pos, m_dll.back());
    }

    // not contain
    V val = m_load(key);

    // "eviction strategy" - evict not by key
    // FIXME: Logic ERROR
    {
      // without tmp too hard - much roolbacks

      // may throw
      Store tmp_store = m_store;
      std::list<K> tmp_dll = m_dll;

      // if it can rollback then easy
      // Need value to roolback - Ok. Need put K/V back - can throw!
      if (tmp_store.size() == k_maxSize) {
        // may throw
        K steal(tmp_dll.front());

        // non throw
        auto p = tmp_store.find(steal);
        tmp_store.erase(p);  // Trouble: how rollback. It no throw if compare no throw
        tmp_dll.pop_front();
      }

      {
        tmp_dll.push_back(key);  // looks like we can safe roolback
        //tmp_store[key] = val; // Trouble if put key then put value throw. What happened?
        //  need default ctor
        tmp_store.insert(std::make_pair(key, val)); // if throw - state unchanged!
      }

      // non throw
      std::swap(m_store, tmp_store);
      std::swap(m_dll, tmp_dll);
    }

    r_v = val;
  }

private:
  typedef std::map<K,
    V
    // FIXME: if store ref. how about exception safty?
  > Store;  // we can know elem iterator

  CacheLoader m_load;
  const size_t k_maxSize;

  // DLL
  std::list<K> m_dll;  // last accessed - first?

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
