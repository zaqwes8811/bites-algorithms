// http://stackoverflow.com/questions/2895342/java-how-can-i-split-an-arraylist-in-multiple-small-arraylists?lq=1

import java.util.List;
import java.util.Random;

class PivotDefault implements Pivot {
  @Override
  public Integer choose(List<Integer> array) {
    return 0;
  }
}

public class QuickSort {
  private final Pivot PIVOT_;
  public QuickSort() {
    PIVOT_ = new PivotDefault();
  }

  public QuickSort(Pivot pivot) {
    PIVOT_ = pivot;
  }

  public void quickSortFirst(List<Integer> array) {
    if (array.size() == 1) return;

    //@BeforeRecursion
    Integer p = PIVOT_.choose(array);
    Integer boundary = partition(array, p);

    //@Recursion
    if (boundary != 0)
      quickSortFirst(array.subList(0, boundary));

    if (boundary+1 != array.size())
      quickSortFirst(array.subList(boundary+1, array.size()));
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

    Integer boundary = i;
    // В зависимости от того в какой часте находится
    if (idxPivot < i) boundary = i-1;
    swap(array, idxPivot, boundary);

    return boundary;
  }

  private void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
