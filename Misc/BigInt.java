import java.util.*;
import java.math.*;

class BigInt {
  public static void p  (Object o){ System.out.print  (o); }
  public static void pln(Object o){ System.out.println(o); }

	public static void main(String[] args) {
    Scanner in = new Scanner(System.in);
    Scanner testin = new Scanner("3 2 1");

    while (testin.hasNextInt()) {
      BigInteger a = new BigInteger(testin.next());
      BigInteger b = BigInteger.TEN;
      pln(a.add(b)); // Arithmetic
      pln(a.pow(3));
      pln(a.and(b)); // Bitwise ops
      pln(a.testBit(0)); // Bit fiddling
      pln(a.getLowestSetBit());
      pln(a.bitCount());
      pln(a.setBit(2)); // a is not modified
      pln(a.compareTo(b));
    }
  }
}









