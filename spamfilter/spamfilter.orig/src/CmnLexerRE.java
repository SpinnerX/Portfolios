
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 1999
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.* ;
import gnu.regexp.* ;

public class CmnLexerRE implements ICmnLexer
{
  gnu.regexp.RE lexer = null ;
  REMatchEnumeration re = null ;

  public CmnLexerRE( String REPattern )
  {
    try {
      lexer = new gnu.regexp.RE( REPattern, 0, RESyntax.RE_SYNTAX_POSIX_EGREP ) ;
    }
    catch( Exception e )
    {
      e.printStackTrace() ;
    }
  }

  public void SetReader( Reader reader )
  {
      if  ( null != lexer )
        re = lexer.getMatchEnumeration( reader ) ;
  }

  public boolean hasMoreElements()
  {
    if  ( null == re )
      return( false ) ;
    else return( re.hasMoreElements() ) ;
  }

  public String nextString()
  {
    if  ( null == re )
      return( null ) ;
    else if  ( re.hasMoreElements() )
      return( re.nextMatch().toString() ) ;
    else return( null ) ;
  }

}
