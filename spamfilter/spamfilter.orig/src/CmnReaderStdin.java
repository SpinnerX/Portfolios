
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.* ;

public class CmnReaderStdin implements ICmnReader
{
  boolean givenAwayResource ;
  Reader stdinReader ;

  public CmnReaderStdin() {}
  {
    givenAwayResource = false ;
    stdinReader = new InputStreamReader( System.in ) ;
  }

  public Reader GetReader()
  {
    if  ( !givenAwayResource )
      return( stdinReader ) ;
    else return( null ) ;
  }

  public void ReleaseReader( Reader reader )
  {
    if  ( reader.equals( stdinReader ) )
      givenAwayResource = false ;
  }
}