
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class WordDataBase
{

  public WordDataBase()
  {
    hashTbl = new Hashtable() ;
  }

  public Enumeration keys()
  {
    return( hashTbl.keys() ) ;
  }

  public void AddWord( String str )
  {
    if  ( null == str )
      return ;
    Object n = (Integer) hashTbl.get( str ) ;
    if ( null == n )
      hashTbl.put( str, new Integer( 0 ) ) ;
  }

  public boolean HasWord( String str )
  {
    if  ( null == str )
      return( false ) ;
    Object n = (Integer) hashTbl.get( str ) ;
    return( null != n ) ;
  }

  Hashtable hashTbl ;
}