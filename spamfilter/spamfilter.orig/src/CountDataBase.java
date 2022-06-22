
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 2000
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CountDataBase extends WordDataBase {

  public CountDataBase()
  {
  }

  public void AddWord( String s )
  {
    if  ( null == s )
      return ;
    Integer n = (Integer) hashTbl.get( s ) ;
    if ( null != n )
      n = new Integer( n.intValue() + 1 ) ;
    else n = new Integer( 1 ) ;
    hashTbl.put( s, n ) ;
  }

  public int getWordCount( String s )
  {
    if  ( null == s )
      return( 0 ) ;
    Integer n = (Integer) hashTbl.get( s ) ;
    if ( null != n )
      return( n.intValue() ) ;
    else return( 0 ) ;
  }

}
