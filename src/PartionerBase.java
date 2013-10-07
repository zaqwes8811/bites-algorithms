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

    // В зависимости от того в какой часте находится
    if (idxPivot < i)
      i--;

    // Меняем индексы
    swap(array, idxPivot, i);
    if (!checkPostcondition(array, i, pivot))
      throw new RuntimeException("checkPostcondition");
    return i;
  }

  // Самый большой левый элемент меньше pivot
  // Самый меньший правый больше pivot
  public Boolean checkPostcondition(
      List<Integer> array,
      Integer boundary,
      Integer pivot) {
    Integer maxLeft;
    Integer minRight;
    if (boundary == 0) maxLeft = pivot-1;
    else maxLeft = Collections.max(array.subList(0, boundary));
    if (pivot < maxLeft) return false;

    if (boundary == array.size()-1) return true;
    else {
      minRight = Collections.min(array.subList(boundary, array.size()));
      if (pivot < minRight) return false;
    }

    return true;
  }

  private void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
