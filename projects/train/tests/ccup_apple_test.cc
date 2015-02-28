
#include <gtest/gtest.h>

// "in pursuit of" - non ring
template<typename T>
class write_and_read
{
public:
  // don't own buffer?
  // Need place elements in raw buffer
  write_and_read(T* s, int n) : m_start(s)
    , m_producer_ptr(s)
    , m_consumer_ptr(s)
    , m_max_bytes(n)
    , m_num_of_elems(0) {}

  T* get_start_ptr(){ return m_start; }
  T* get_wr_ptr(){ return m_producer_ptr; }
  T* get_rd_ptr(){ return m_consumer_ptr; }

  // bad interface!
  bool write(T* in_ptr, int n)
  {
    if (in_ptr == NULL && n > 0)
      return false;

    if (m_max_bytes - m_num_of_elems < n * sizeof(T) / sizeof(char))
      return false;

    m_num_of_elems += n * sizeof(T) / sizeof(char);

    for (int i = 0; i < n; ++i) {
      *m_producer_ptr = *in_ptr;
      ++m_producer_ptr;
      ++in_ptr;
    }

    return true;
  }

  // bad interface!
  T* read(int n) {
    if (n <= 0)
      return NULL;

    T* res = (T*) malloc(n);  // leak if user call... what call? how know how alloc memory?

    T* t = res;

    int count = 0;
    while (m_producer_ptr != m_consumer_ptr) {
      *t = *m_consumer_ptr;
      m_consumer_ptr += sizeof(T);
      ++t;
      ++count;
    }

    if (count < n)
      m_consumer_ptr = m_start;

    return res;
  }

private:
  T* m_start, *m_producer_ptr, *m_consumer_ptr;

  size_t m_max_bytes, m_num_of_elems;
};


TEST(Apple, RingBuffer) {
  // http://www.careercup.com/question?id=15190678
  // FIXME: don't understood some conditions
}
