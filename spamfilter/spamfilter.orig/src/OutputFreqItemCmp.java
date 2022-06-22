
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class OutputFreqItemCmp implements Comparator
{

  public OutputFreqItemCmp()
  {
  }

  public int compare( Object k1, Object k2 )
  {
    double retval = ((OutputFreqItem) k2).val - ((OutputFreqItem) k1).val ;
    if  ( 0 == retval )
      return( ((OutputFreqItem) k1).key.compareTo( ((OutputFreqItem) k2).key ) ) ;
    else return( retval > 0 ? 1 : -1 ) ;
  }

}