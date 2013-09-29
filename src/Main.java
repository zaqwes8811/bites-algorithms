public class Main {

    public static void main(String[] args) {
      Integer [] array = {3,8,2,5, 1,4,7,6};
      //Integer [] array = {3,8,2,5, 6,4,7,1};

     quickSort(array);
    }

  private static  void quickSort(Integer [] array) {
    if (array.length == 1) return;
    Integer p = choosePivot(array);

  }

    public static  Integer choosePivot(Integer [] array) {
      Integer size = array.length;
      Integer idxPivot = 0;
      Integer i = idxPivot+1;
      Integer pivot = array[idxPivot];
      for (Integer j = i; j < size; j++) {
        if (array[j] < pivot) {
          swap(array, j, i);
          i++;
        }
      }
      swap(array, idxPivot, i-1);
      return i;
    }

  private static void swap(Integer [] array, Integer a, Integer b) {
    Integer tmp = array[a];
    array[a] = array[b];
    array[b] = tmp;
  }
}
