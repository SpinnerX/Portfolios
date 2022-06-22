
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnOutputOrganizer
{

  public CmnOutputOrganizer()
  {
  }

  public Iterator OrganizeOutputs( WordDataBase db )
  {
    String str ;
    ArrayList keys = new ArrayList() ;
    for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
    {
      keys.add( e.nextElement() ) ;
    }
    return( keys.iterator() ) ;
  }
}