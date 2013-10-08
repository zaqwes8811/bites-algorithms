import com.google.common.collect.Ordering;
import com.google.common.io.Closer;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

import static org.junit.Assert.*;
import static org.junit.Assume.assumeTrue;

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
    Integer [] rawArray = {3,8,2,5, 1,4,7,6};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSort();
    sorter.quickSortFirst(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  @org.junit.Test
  public void testRandom() throws Exception {
    Integer [] rawArray = {3,8,2,5, 1,4,7,6, 10};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSort();
    for (int i = 0; i < 10; i++) {
      sorter.quickSortFirst(array);
      assumeTrue(Ordering.natural().isOrdered(array));
    }
  }

  @org.junit.Test
  public void testBoundary() throws Exception {
    Integer [] rawArray = {3,8,2,5, 6,4,7,1};

    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    QuickSort sorter = new QuickSort();
    sorter.quickSortFirst(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  @org.junit.Test
  public void testFromFile() throws Exception {
    String filename = "QuickSort.txt";
    List<Integer> array = fileToList(filename);

    int end = array.size();
    QuickSort sorter = new QuickSort(new PivotLast(), new PartionerBase());
    sorter.quickSortFirst(array.subList(0, end));
    assumeTrue(Ordering.natural().isOrdered(array.subList(0, end)));
  }

  @org.junit.Test
  public void testFromFileRandom() throws Exception {
    QuickSort sorter = new QuickSort(new PivotRandom(), new PartionerBase());
    for (int i = 0; i < 10; ++i) {
      String filename = "QuickSort.txt";
      List<Integer> array = fileToList(filename);
      int end = array.size();
      sorter.quickSortFirst(array.subList(0, end));
      assumeTrue(Ordering.natural().isOrdered(array.subList(0, end)));
    }
  }

  @org.junit.Test
  public void testProgramQuestion1() throws Exception {
    QuickSort sorter = new QuickSort(new PivotFirst(), new PartionerBase());
    String filename = "QuickSort.txt";
    List<Integer> array = fileToList(filename);
    sorter.quickSortFirst(array);
    assumeTrue(Ordering.natural().isOrdered(array));
  }

  static private List<Integer> fileToList(String filename) throws IOException {
    Closer closer = Closer.create();
    List<Integer> result = new ArrayList<Integer>();
    try {
      BufferedReader in = closer.register(new BufferedReader(new FileReader(filename)));
      String s;
      while ((s = in.readLine()) != null) {
        result.add(Integer.parseInt(s));
      }
    } catch (Throwable e) {
      closer.rethrow(e);
    } finally {
      closer.close();
    }
    return result;
  }
}

