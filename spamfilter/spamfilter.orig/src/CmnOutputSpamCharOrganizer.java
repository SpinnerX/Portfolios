//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnOutputSpamCharOrganizer extends CmnOutputOrganizer
{

  public CmnOutputSpamCharOrganizer()
  {
  }


  String words[] = {
    ";",
    "(",
    "[",
    "!",
    "$",
    "#"
  } ;

  public Iterator OrganizeOutputs( WordDataBase db )
  {
    if  ( db instanceof CountDataBase )
    {
      double totalCounts = 0 ;
      for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
        totalCounts += ((CountDataBase) db).getWordCount( (String) e.nextElement() ) ;

      double cnt = 0 ;
      Vector results = new Vector() ;
      for( int i = 0 ;   i < words.length ;   i++ )
      {
        cnt = ((CountDataBase) db).getWordCount( words[ i ] ) ;
				if (totalCounts > 0)
        	results.add( new OutputFreqItem( words[ i ], 100 * cnt / totalCounts ) ) ;
				else 
        	results.add( new OutputFreqItem( words[ i ], 0 ) ) ;
      }

      return( results.iterator() ) ;
    }
    else
      return( super.OrganizeOutputs( db ) ) ;
  }
} 
