
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.* ;

public interface ICmnLexer
{

  public void SetReader( Reader reader ) ;
  public boolean hasMoreElements() ;
  public String nextString() ;

}
