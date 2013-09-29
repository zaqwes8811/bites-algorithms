// http://stackoverflow.com/questions/2895342/java-how-can-i-split-an-arraylist-in-multiple-small-arraylists?lq=1

import java.util.List;

public class QuickSort {
  public void quickSortFirst(List<Integer> array) {
    if (array.size() == 1) return;

    //@BeforeRecursion
    Integer p = choosePivot(array);
    Integer boundary = partition(array, p);

    //@Recursion
    if (boundary != 0)
      quickSortFirst(array.subList(0, boundary));
    if (boundary != array.size())
      quickSortFirst(array.subList(boundary, array.size()));

    //@AfterRecursion
    // Nothing
  }



  public Integer choosePivot(List<Integer> array) {
    return array.size()-1;  // no work
  }

  public Integer partition(List<Integer> array, Integer idxPivot) {
    // Пока зависит от выбора опоры
    Integer size = array.size();
    Integer pivot = array.get(idxPivot);
    Integer i = 0;

    // Проходим все, но проскакиваем указатель на опору
    for (Integer j = 0; j < size; j++) {
      if (j == idxPivot) continue;

      if (pivot > array.get(j)) {
        if (i == idxPivot) i++;
        swap(array, j, i);
        i++;
      }
    }


    // В зависимости от того в какой часте находится
    if (idxPivot < i) swap(array, idxPivot, i-1);
    else swap(array, idxPivot, i);

    return i;
  }

  private void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
