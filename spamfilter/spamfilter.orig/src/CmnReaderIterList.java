
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.util.* ;

public class CmnReaderIterList extends CmnReaderIterator
{

  public CmnReaderIterList()
  {
  }

  public void SetFileNames( String[] fileNameArray )
  {
    iterator = Arrays.asList( fileNameArray ).iterator() ;
  }

}