import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 29.09.13
 * Time: 16:42
 * To change this template use File | Settings | File Templates.
 */
public class QuickSortTest {
  @org.junit.Test
  public void testAsInVideo() throws Exception {
    QuickSort sorter = new QuickSort();
    Integer [] rawArray = {3,8,2,5, 1,4,7,6};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    sorter.quickSort(array);
    System.out.println(array);
  }

  public void testBoundary() throws Exception {
    QuickSort sorter = new QuickSort();
    Integer [] rawArray = {3,8,2,5, 6,4,7,1};

    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    sorter.quickSort(array);
    System.out.println(array);
  }
}
