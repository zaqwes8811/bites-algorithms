package stanford_algoritms_part1.sort;// http://stackoverflow.com/questions/2895342/java-how-can-i-split-an-arraylist-in-multiple-small-arraylists?lq=1
/**
 * На больших данных тест проваливается
 * - проверить инварианты
 * - проверить разбиение на подзадачи
 * - проверить процедуру деления со всеми позициями опорного элемента
 * */

import stanford_algoritms_part1.sort.partioners.Partioner;
import stanford_algoritms_part1.sort.partioners.PartionerBase;
import stanford_algoritms_part1.sort.pivots.Pivot;
import stanford_algoritms_part1.sort.pivots.PivotFirst;

import java.util.Collections;
import java.util.List;

public class QuickSort {
  private final Pivot PIVOT_;
  private final Partioner PARTIONER_;
  public QuickSort() {
    PIVOT_ = new PivotFirst();
    PARTIONER_ = new PartionerBase();
  }

  public QuickSort(Pivot pivot, Partioner partioner) {
    PIVOT_ = pivot;
    PARTIONER_ = partioner;
  }

  public void quickSortFirst(List<Integer> array) {
    if (array.size() == 1) return;

    //@BeforeRecursion
    Integer p = PIVOT_.choose(array);
    Integer pivot = array.get(p);

    Integer i = PARTIONER_.partition(array, p);

    // Если левая часть не пуста
    if (i != 0) {
      // Проверить инвариант
      Integer minLeft = Collections.max(array.subList(0, i));
      if (!(minLeft < array.get(i)))
        throw new RuntimeException("Left postcondition failed");
      quickSortFirst(array.subList(0, i));
    }

    // Если правая не пуста
    if (i != array.size()-1) {
      Integer minRight = Collections.min(array.subList(i+1, array.size()));
      if (!(minRight > array.get(i)))
        throw new RuntimeException("Right postcondition failed.");
      quickSortFirst(array.subList(i+1, array.size()));
    }
  }
}
