package stanford_algoritms_part1.sort.util_local;

import com.google.common.io.Closer;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created with IntelliJ IDEA.
 * User: кей
 * Date: 26.10.13
 * Time: 11:17
 * To change this template use File | Settings | File Templates.
 */
public class FileOperations {
  static public List<Integer> fileToList(String filename) throws IOException {
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
