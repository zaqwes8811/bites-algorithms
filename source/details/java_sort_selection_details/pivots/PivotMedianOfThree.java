package stanford_algoritms_part1.sort.pivots;

import java.util.*;

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
    Integer idx_middle = array.size()/2;
    Integer idx_doubled = 2*idx_middle;
    if (idx_doubled.equals(array.size()))
      idx_middle--;

    Integer first = array.get(idx_first);
    Integer middle = array.get(idx_middle);
    Integer last = array.get(idx_last);

    List<Integer> forSort = new ArrayList<Integer>(Arrays.asList(first, middle, last));
    Collections.sort(forSort);
    Integer midValue = forSort.get(1);

    if (midValue.equals(first)) return idx_first;
    if (midValue.equals(middle)) return idx_middle;
    if (midValue.equals(last)) return idx_last;
    else return 0;
  }
}
