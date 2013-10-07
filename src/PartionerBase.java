import com.google.common.base.Joiner;
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

    // Граница будет указывать на первый правый
    if (inLeftPart(idxPivot, i)) i--;

    // Меняем индексы
    swap(array, idxPivot, i);

    // Похоже ошибка, когда выбранный стержень в левой часте
    // p in left and pivot in right
    if (pivot != array.get(i))
      throw new RuntimeException(Joiner.on('\t').join(
        array.subList(0, i).contains(pivot),
        array.subList(i-4, i+4),
        array.subList(i+1, array.size()).contains(pivot)));

    return i;
  }

  private boolean inLeftPart(Integer idxPivot, Integer i) {
    if (idxPivot < i) return true;
    return false;
  }

  private void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
