// http://stackoverflow.com/questions/2895342/java-how-can-i-split-an-arraylist-in-multiple-small-arraylists?lq=1

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {

    public static void main(String[] args) {
      //Integer [] array = {3,8,2,5, 1,4,7,6};
      //Integer [] array = {3,8,2,5, 6,4,7,1};

      List<Integer> array = new ArrayList<Integer>(Arrays.asList(3,8,2,5, 1,4,7,6));

     quickSort(array);
      System.out.println(array);
    }

  private static  void quickSort(List<Integer> array) {
    if (array.size() == 1) return;

    //@BeforeRecursion
    Integer p = choosePivot(array);
    partition(array, p);

    //@Recursion
    quickSort(array.subList(0, p));
    //@AfterRecursion
    // Nothing
  }

    public static  Integer choosePivot(List<Integer> array) {
      return 0;
    }
    private static void partition(List<Integer> array, Integer idxPivot) {
      Integer size = array.size();
      Integer i = idxPivot+1;
      Integer pivot = array.get(idxPivot);
      for (Integer j = i; j < size; j++) {
        if (array.get(j) < pivot) {
          swap(array, j, i);
          i++;
        }
        System.out.println(array);
      }
      swap(array, idxPivot, i-1);
      System.out.println(array);
    }

  private static void swap(List<Integer> array, Integer a, Integer b) {
    Integer tmp = array.get(a);
    array.set(a, array.get(b));
    array.set(b, tmp);
  }
}
