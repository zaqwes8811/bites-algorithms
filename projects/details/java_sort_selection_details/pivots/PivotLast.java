package stanford_algoritms_part1.sort.pivots;

import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 29.09.13
 * Time: 20:34
 * To change this template use File | Settings | File Templates.
 */

public class PivotLast implements Pivot {
  @Override
  public Integer choose(List<Integer> array) {
    return array.size()-1;
  }
}

