
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.* ;

public interface ICmnReader
{

  public Reader GetReader() ;
  public void ReleaseReader( Reader reader ) ;

}
