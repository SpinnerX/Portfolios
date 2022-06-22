
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.* ;
import java.util.* ;

public class CmnReaderIterFile extends CmnReaderIterator
{

  public CmnReaderIterFile()
  {
  }

  public void SetFileNames( String fileOfNames )
  {
    ArrayList fileNames = new ArrayList() ;
    try
    {
      Reader reader = new BufferedReader( new FileReader( fileOfNames ) ) ;

      int ttype ;

      // This needs to be more sophisticated to allow complex filenames
      // drive:\name\name.name-name_+etc
      // Should probably allow "name with embeded blanks"
      StreamTokenizer st = new StreamTokenizer( reader ) ;
      while( ( ttype = st.nextToken() ) != st.TT_EOF )
      {
        if  ( ttype == st.TT_WORD )
          fileNames.add( st.sval ) ;
        // else ignore the non-word information
      }
    }
    catch( IOException e )
    {
      e.printStackTrace() ;
    }

    iterator = fileNames.iterator() ;
  }

}