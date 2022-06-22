
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class OutputCountItemCmp implements Comparator
{
  public OutputCountItemCmp() {}

  public int compare( Object k1, Object k2 )
  {
    int retval = ((OutputCountItem) k2).val - ((OutputCountItem) k1).val ;
    if  ( 0 == retval )
      return( ((OutputCountItem) k1).key.compareTo( ((OutputCountItem) k2).key ) ) ;
    else return( retval ) ;
  }
}
