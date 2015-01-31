package stanford_algoritms_part1.sort.partioners;

import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 29.09.13
 * Time: 20:48
 * To change this template use File | Settings | File Templates.
 */
public interface Partioner {
  public Integer partition(List<Integer> array, Integer idxPivot);
}
