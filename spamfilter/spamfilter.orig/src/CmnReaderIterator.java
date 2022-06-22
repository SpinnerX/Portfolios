
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.* ;
import java.util.* ;

public class CmnReaderIterator implements ICmnReader
{
  Iterator iterator = null ;

  public CmnReaderIterator()
  {
  }

  public Reader GetReader()
  {
    Reader reader = null ;
    while( ( null == reader )  &&  iterator.hasNext() )
    {
      try
      {
        String fileName = (String) iterator.next() ;
        reader = new BufferedReader( new FileReader( fileName ) ) ;
      }
      catch( IOException e )
      {
        e.printStackTrace() ;
      }
    }
    return( reader ) ;
  }

  public void ReleaseReader( Reader reader )
  {
    try
    {
      reader.close() ;
    }
    catch( IOException e )
    {
      e.printStackTrace() ;
    }
  }

}