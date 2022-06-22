//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnOutputCapsOrganizer extends CmnOutputOrganizer
{

  public CmnOutputCapsOrganizer()
  {
  }

  public Iterator OrganizeOutputs( WordDataBase db )
  {
    if  ( db instanceof CountDataBase )
    {
      double avgCapLen = 0 ;
      double longestRun = 0 ;
      double totalCaps = 0 ;
      double totalWords = 0 ;

      String str ;
      int cnt ;

      for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
      {
        str = (String) e.nextElement() ;
        cnt = ((CountDataBase) db).getWordCount( str ) ;
        totalWords += cnt ;
        totalCaps += (cnt * str.length()) ;
        if  ( longestRun < str.length() )
          longestRun = str.length() ;
      }

      Vector results = new Vector() ;

      if (totalWords > 0)
      	results.add( new OutputFreqItem( "AvgCaps", totalCaps / totalWords ) ) ;
      else
      	results.add( new OutputFreqItem( "AvgCaps", 0) ) ;
      results.add( new OutputFreqItem( "LongRun", longestRun ) ) ;
      results.add( new OutputFreqItem( "TotalCaps", totalCaps ) ) ;

      return( results.iterator() ) ;
    }
    else
      return( super.OrganizeOutputs( db ) ) ;
  }

} 
