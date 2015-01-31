package stanford_algoritms_part1.sort.pivots;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.Assume.*;
import static org.junit.Assert.*;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 26.10.13
 * Time: 12:15
 * To change this template use File | Settings | File Templates.
 */
public class PivotMedianOfThreeTest {
  @Test
  public void testChoose() throws Exception {
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(8, 2, 4, 5, 7, 1));
    Pivot pivot = new PivotMedianOfThree();
    Integer pivotIdx = pivot.choose(array);
    assumeTrue(pivotIdx.equals(2));
  }

  @Test
  public void testChooseOdd() throws Exception {
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(8, 2, 4, 7, 5, 6, 1));
    Pivot pivot = new PivotMedianOfThree();
    Integer pivotIdx = pivot.choose(array);
    assumeTrue(pivotIdx.equals(3));
  }

  @Test
  public void testSelectedLast() throws Exception {
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(8, 2, 4, 1, 5, 7, 6));
    Pivot pivot = new PivotMedianOfThree();
    Integer pivotIdx = pivot.choose(array);
    assumeTrue(pivotIdx.equals(6));
  }

  @Test
  public void testSelectedFirst() throws Exception {
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(6, 2, 4, 1, 5, 7, 8));
    Pivot pivot = new PivotMedianOfThree();
    Integer pivotIdx = pivot.choose(array);
    assumeTrue(pivotIdx.equals(0));
  }

  @Test
  public void testSmallMiddle() throws Exception {
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(1, 2, 3));
    Pivot pivot = new PivotMedianOfThree();
    Integer pivotIdx = pivot.choose(array);
    assumeTrue(pivotIdx.equals(1));
  }

  @Test
  public void testSmallMiddle0() throws Exception {
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(4, 5, 6, 7));
    Pivot pivot = new PivotMedianOfThree();
    Integer pivotIdx = pivot.choose(array);
    assumeTrue(array.get(pivotIdx).equals(5));
  }
}
