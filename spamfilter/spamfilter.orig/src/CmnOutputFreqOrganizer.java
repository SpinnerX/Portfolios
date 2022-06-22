//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnOutputFreqOrganizer extends CmnOutputOrganizer
{

  public CmnOutputFreqOrganizer()
  {
  }

  public Iterator OrganizeOutputs( WordDataBase db )
  {
    if  ( db instanceof CountDataBase )
    {
      double totalCounts = 0 ;
      for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
        totalCounts += ((CountDataBase) db).getWordCount( (String) e.nextElement() ) ;

      double cnt ;
      String str ;
      TreeSet ts = new TreeSet( new OutputFreqItemCmp() ) ;
      for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
      {
        str = (String) e.nextElement() ;
        cnt = ((CountDataBase) db).getWordCount( str ) ;
	if (totalCounts > 0)
        	ts.add( new OutputFreqItem( str, cnt / totalCounts ) ) ;
	else
        	ts.add( new OutputFreqItem( str, 0 ) ) ;
      }
      return( ts.iterator() ) ;
    }
    else
      return( super.OrganizeOutputs( db ) ) ;
  }


} 
