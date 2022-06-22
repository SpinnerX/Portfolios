//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnOutputSpamWordOrganizer extends CmnOutputOrganizer
{

  public CmnOutputSpamWordOrganizer()
  {
  }

  String words[] = {
    "make",
    "address",
    "all",
    "3d",
    "our",
    "over",
    "remove",
    "internet",
    "order",
    "mail",
    "receive",
    "will",
    "people",
    "report",
    "addresses",
    "free",
    "business",
    "email",
    "you",
    "credit",
    "your",
    "font",
    "000",
    "money",
    "hp",
    "hpl",
    "george",
    "650",
    "lab",
    "labs",
    "telnet",
    "857",
    "data",
    "415",
    "85",
    "technology",
    "1999",
    "parts",
    "pm",
    "direct",
    "cs",
    "meeting",
    "original",
    "project",
    "re",
    "edu",
    "table",
    "conference"
  } ;

  public Iterator OrganizeOutputs( WordDataBase db )
  {
    if  ( db instanceof CountDataBase )
    {
      double totalCounts = 0 ;
      for( Enumeration e = db.keys() ; e.hasMoreElements() ; )
			{
        totalCounts += ((CountDataBase) db).getWordCount( (String) e.nextElement() ) ;
			}

      double cnt ;
      Vector results = new Vector() ;
      for( int i = 0 ;   i < words.length ;   i++ )
      {
        cnt = ((CountDataBase) db).getWordCount( words[ i ] ) ;
				if (totalCounts > 0)
        	results.add( new OutputFreqItem( words[ i ], 100 * cnt / totalCounts ) ) ;
				else
        	results.add( new OutputFreqItem( words[ i ], 0));
      }

      return( results.iterator() ) ;
    }
    else
      return( super.OrganizeOutputs( db ) ) ;
  }

} 
