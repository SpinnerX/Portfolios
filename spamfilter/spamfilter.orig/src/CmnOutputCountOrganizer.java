
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnOutputCountOrganizer extends CmnOutputOrganizer
{

  public CmnOutputCountOrganizer()
  {
  }

  public Iterator OrganizeOutputs( WordDataBase db )
  {
    if  ( db instanceof CountDataBase )
    {
      String str ;
      TreeSet ts = new TreeSet( new OutputCountItemCmp() ) ;
      for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
      {
        str = (String) e.nextElement() ;
        ts.add( new OutputCountItem( str, ((CountDataBase) db).getWordCount( str ) ) ) ;
      }
      return( ts.iterator() ) ;
    }
    else
      return( super.OrganizeOutputs( db ) ) ;
  }
}
