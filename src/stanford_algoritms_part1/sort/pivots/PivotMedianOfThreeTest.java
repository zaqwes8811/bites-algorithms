package stanford_algoritms_part1.sort.pivots;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.Assume.*;

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
    assumeTrue(pivot.choose(array).equals(4));
  }
}
