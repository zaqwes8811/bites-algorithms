import com.google.common.collect.ImmutableList;
import com.google.common.io.Closer;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
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

  @org.junit.Test
  public void testBoundary() throws Exception {
    QuickSort sorter = new QuickSort();
    Integer [] rawArray = {3,8,2,5, 6,4,7,1};

    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    sorter.quickSort(array);
    System.out.println(array);
  }

  @org.junit.Test
  public void testFromFile() throws Exception {

  }

  static private ImmutableList<String> fileToList(String filename) throws IOException {
    Closer closer = Closer.create();
    List<String> result = new ArrayList<String>();
    try {
      // TODO(zaqwes): Может лучше считать разом, а потом разбить на части?
      BufferedReader in = closer.register(new BufferedReader(new FileReader(filename)));
      String s;
      while ((s = in.readLine()) != null) result.add(s);
    } catch (Throwable e) {
      closer.rethrow(e);
    } finally {
      closer.close();
    }
    return ImmutableList.copyOf(result);
  }
}
