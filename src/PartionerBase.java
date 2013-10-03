import com.google.common.collect.ImmutableList;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

public class PartionerBase implements Partioner {
  @Override
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
    if (idxPivot < boundary)
      boundary--;

    // Меняем индексы
    swap(array, idxPivot, boundary);

    List<Integer> clone = new ArrayList<Integer>(array);

    if (!checkPostcondition(array, boundary, pivot))
      throw new RuntimeException(""
        //+" pivot = "+pivot
        +" boundary = "+boundary
        //+" now on idx_pivot = "+array.get(idxPivot)
        //+" idxPivot = "+idxPivot
        +" around = "+array.subList(boundary-2, boundary+3)
        //+" around = "+clone.subList(boundary-2, boundary+3)
        );
    return boundary;
  }

  public Boolean checkPostcondition(List<Integer> array, Integer boundary, Integer pivot) {
    Integer maxLeft;
    Integer minRight;
    if (boundary == 0) maxLeft = pivot-1;
    else maxLeft = Collections.max(array.subList(0, boundary));

    if (boundary == array.size()-1) minRight = pivot+1;
    else minRight = Collections.min(array.subList(boundary+1, array.size()));
    if (pivot > maxLeft && pivot < minRight) return true;
    return false;
  }

  private void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
