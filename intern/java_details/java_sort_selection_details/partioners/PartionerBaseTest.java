package stanford_algoritms_part1.sort.partioners;

import org.junit.Test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class PartionerBaseTest {
  @Test
  public void testPartition() throws Exception {
    Integer [] rawArray = {3,8,2,5, 1,4,7,6};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    for(Integer i = 0; i < array.size(); ++i) {
      partioner.partition(array, i);
    }
  }

  @Test
  public void testPartitionOdd() throws Exception {
    Integer [] rawArray = {3,8,2,5, 1,4,7,6, 9};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    for(Integer i = 0; i < array.size(); ++i) {
      partioner.partition(array, i);
    }
  }
  @Test
  public void testPartitionReal() throws Exception {
    Integer [] rawArray = {4, 5, 6};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    for(Integer i = 0; i < array.size(); ++i) {
      //System.out.println(i);
      partioner.partition(array, i);
    }
  }


  public void testPartitionSmall() throws Exception {
    Integer [] rawArray = {3};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    for(Integer i = 0; i < array.size(); ++i) {
      partioner.partition(array, i);
    }
  }

  public void testPartitionEmpty() throws Exception {
    Integer [] rawArray = {};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    for(Integer i = 0; i < array.size(); ++i) {
      partioner.partition(array, i);
    }
  }

  public void testPartitionDouble() throws Exception {
    Integer [] rawArray = {3, 2, 4};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    for(Integer i = 0; i < array.size(); ++i) {
      partioner.partition(array, i);
    }
  }

  @Test
  public void testCheckPostcondition() throws Exception {
    Integer [] rawArray = {3,8,2,5, 1,4,7,6};
    List<Integer> array = new ArrayList<Integer>(Arrays.asList(rawArray));

    Partioner partioner = new PartionerBase();
    partioner.partition(array, 0);
  }
}
