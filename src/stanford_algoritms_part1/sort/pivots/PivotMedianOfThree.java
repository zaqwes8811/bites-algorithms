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
    Integer last = array.get(idx_last);
    Integer middle = array.get(idx_middle);

    Map<Integer, Integer> decoder = new HashMap<Integer, Integer>();
    decoder.put(first, idx_first);
    decoder.put(last, idx_last);
    decoder.put(middle, idx_middle);

    List<Integer> forSort = new ArrayList<Integer>(Arrays.asList(first, middle, last));
    Collections.sort(forSort);

    return decoder.get(forSort.get(1));
  }
}
