// http://stackoverflow.com/questions/2895342/java-how-can-i-split-an-arraylist-in-multiple-small-arraylists?lq=1

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class QuickSort {
  public void quickSort(List<Integer> array) {
    if (array.size() == 1) return;

    //@BeforeRecursion
    Integer p = choosePivot(array);
    Integer boundary = partition(array, p);

    //@Recursion
    quickSort(array.subList(0, boundary));

    if (boundary != array.size())
      quickSort(array.subList(boundary, array.size()));

    //@AfterRecursion
    // Nothing
  }

  private Integer choosePivot(List<Integer> array) {
    //return array.size()-1;  // no work
    return 0;
  }
  private Integer partition(List<Integer> array, Integer idxPivot) {
    Integer size = array.size();
    Integer i = idxPivot+1;
    Integer pivot = array.get(idxPivot);
    for (Integer j = i; j < size; j++) {
      if (array.get(j) < pivot) {
        swap(array, j, i);
        i++;
      }
    }
    swap(array, idxPivot, i-1);
    return i;
  }

  private void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
