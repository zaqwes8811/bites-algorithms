package stanford_algoritms_part1.sort.pivots;

import java.util.List;
import java.util.Random;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 29.09.13
 * Time: 20:33
 * To change this template use File | Settings | File Templates.
 */
public class PivotRandom implements Pivot {
  @Override
  public Integer choose(List<Integer> array) {
    int value = new Random().nextInt(array.size());
    return value;
  }
}