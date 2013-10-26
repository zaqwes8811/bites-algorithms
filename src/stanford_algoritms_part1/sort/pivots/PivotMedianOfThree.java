package stanford_algoritms_part1.sort.pivots;

import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 26.10.13
 * Time: 12:15
 * To change this template use File | Settings | File Templates.
 */
public class PivotMedianOfThree implements Pivot {
  public Integer choose(List<Integer> array) {
    Integer idx_first = 0;
    Integer idx_last = array.size()-1;
    Integer idx_middle = array.size()/2-1;

    Integer first = array.get(idx_first);
    Integer last = array.get(idx_last);
    Integer middle = array.get(idx_middle);
    return 0;
  }
}
