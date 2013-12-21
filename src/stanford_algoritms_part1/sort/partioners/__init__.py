# coding: utf-8


class PartionerBase(object):
    def partition(self, array):
        """
             Берем певый в качастве Pivot
        """
        IDX_PIVOT = 0

        """
    // Пока зависит от выбора опоры
    Integer size = array.size();
    Integer pivot = array.get(idxPivot);
    Integer i = 0;

    //@Pre
    if (!pivot.equals(array.get(idxPivot)))
      throw new RuntimeException("Befor cycle partioning.");

    // Проходим все, но проскакиваем указатель на опору
    for (Integer j = 0; j < size; j++) {
      if (j.equals(idxPivot)) continue;
      if (pivot > array.get(j)) {
        if (i.equals(idxPivot)) i++;
        if (i.equals(idxPivot) || j.equals(idxPivot))
          throw new RuntimeException("Active move pivot");

        // http://www.javaworld.com/javaworld/javaqa/2000-05/03-qa-0526-pass.html
        Collections.swap(array, j, i);
        i++;
      }
    }

    //@Post
    if (!pivot.equals(array.get(idxPivot)))
      throw new RuntimeException("Pivot moved!");

    // Граница будет указывать на первый правый
    if (inLeftPart(idxPivot, i)) i--;

    // Меняем индексы
    Collections.swap(array, idxPivot, i);

    // Похоже ошибка, когда выбранный стержень в левой часте
    // p in left and pivot in right
    if (pivot != array.get(i))
      throw new RuntimeException("Pivot not in right place");

    return i;
  }

  private boolean inLeftPart(Integer idxPivot, Integer i) {
    if (idxPivot < i) return true;
    return false;
  }


}"""