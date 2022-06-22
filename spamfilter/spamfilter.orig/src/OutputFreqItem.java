
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

public class OutputFreqItem
{

  public OutputFreqItem( String s, double i )
  {
    key = s ;
    val = i ;
  }
  public String key ;
  public double val ;

  public String toString()
  {
    return( /* key + ": " + */ "" + val ) ;
  }

} 