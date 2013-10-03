// http://stackoverflow.com/questions/2895342/java-how-can-i-split-an-arraylist-in-multiple-small-arraylists?lq=1
/**
 * На больших данных тест проваливается
 * - проверить инварианты
 * - проверить разбиение на подзадачи
 * - проверить процедуру деления со всеми позициями опорного элемента
 * */
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Random;

public class QuickSort {
  private final Pivot PIVOT_;
  private final Partioner PARTIONER_;
  public QuickSort() {
    PIVOT_ = new PivotDefault();
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
    Integer boundary = PARTIONER_.partition(array, p);

    //@Recursion
    if (boundary != 0) {
      // Проверить инвариант
      quickSortFirst(array.subList(0, boundary));
    }

    if (boundary+1 != array.size()) {
      // Проверить инвариант
      quickSortFirst(array.subList(boundary+1, array.size()));
    }
  }
}
