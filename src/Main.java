import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {

    public static void main(String[] args) {
      Integer [] array = {3,8,2,5, 1,4,7,6};
      Integer size = array.length;
      Integer idxPivot = 0;
      Integer i = idxPivot+1;
      Integer pivot = array[idxPivot];
      for (Integer j = i; j < size; j++) {
        if (array[j] < pivot) {
          swap(array, j, i);
          i++;
        }
        System.out.println(Arrays.toString(array));
      }
    }

  private static void swap(Integer [] array, Integer a, Integer b) {
    Integer tmp = array[a];
    array[a] = array[b];
    array[b] = tmp;
  }
}
