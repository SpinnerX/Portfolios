
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

public class OutputCountItem
{
  public OutputCountItem( String s, int i )
  {
    key = s ;
    val = i ;
  }
  public String key ;
  public int val ;

  public String toString()
  {
    return( key ) ; // + ": " + val ) ;
  }
}
