
//Title:        Common Word Counter
//Version:      
//Copyright:    Copyright (c) 2000
//Author:       Bob Boyer
//Company:      
//Description:  Class Project CSE 218
package CmnWdCnt;

import java.io.*;
import java.lang.* ;

public class Lexer {

  private class InternalResults
  {
    public InternalResults( String pStr, boolean pOk )
    {
      s = pStr ;
      ok = pOk ;
    }

    String s ;
    boolean ok ;
  }

  static final int MAX_CHARS_PER_STRING = 1 ;

  public Lexer() {
  }

  private boolean isSpecialChar( int ch )
  {
    return( ch == '-' ) ;
  }

  private boolean purgeBadLeadingChars( Reader r )
  {
    boolean ok = false ;  // Don't know whether next char will be good
    int ch = 0 ;
    char chArray[] = new char[MAX_CHARS_PER_STRING] ;
    while( !ok  &&  ( ch >= 0 ) )
    {
      try
      {
        r.mark( MAX_CHARS_PER_STRING ) ;
        if  ( (ch = r.read( chArray, 0, MAX_CHARS_PER_STRING )) >= 0 )
          ok = java.lang.Character.isLetter( chArray[0] ) ;
      }
      catch( IOException e )
      {
        e.printStackTrace() ;
      }
    }
    if  ( ok )
      try
      {
        r.reset() ;
      }
      catch( IOException e )
      {
        e.printStackTrace() ;
      }
    return( ok ) ;
  }

  public InternalResults acceptGoodCharSequence0( Reader r )
  {
    boolean ok = false ;
    String s = "" ;
    char chArray[] = new char[MAX_CHARS_PER_STRING] ;
    try
    {
      r.mark( MAX_CHARS_PER_STRING ) ;
      ok = r.read( chArray, 0, MAX_CHARS_PER_STRING ) >= 0 ;
      while( ok  &&  java.lang.Character.isLetter( chArray[0] ) )
      {
        s = s + chArray[0] ;
        r.mark( MAX_CHARS_PER_STRING ) ;
        ok = r.read( chArray, 0, MAX_CHARS_PER_STRING ) >= 0 ;
      }
      if  ( ok )
        r.reset() ;
    }
    catch( IOException e )
    {
      e.printStackTrace() ;
    }
    return( new InternalResults( s, ok ) ) ;
  }

  public InternalResults acceptGoodCharSequence1a( Reader r )
  {
    boolean ok = false ;
    String s = "" ;
    char chArray[] = new char[MAX_CHARS_PER_STRING] ;
    try
    {
      r.mark( MAX_CHARS_PER_STRING ) ;
      ok = r.read( chArray, 0, MAX_CHARS_PER_STRING ) >= 0 ;
      while( ok  &&  isSpecialChar( chArray[0] ) )
      {
        s = s + chArray[0] ;
        r.mark( MAX_CHARS_PER_STRING ) ;
        ok = r.read( chArray, 0, MAX_CHARS_PER_STRING ) >= 0 ;
      }
      if  ( ok )
        r.reset() ;
    }
    catch( IOException e )
    {
      e.printStackTrace() ;
    }
    return( new InternalResults( s, ok ) ) ;
  }

  public InternalResults acceptGoodCharSequence1( Reader r )
  {
    String s = "" ;
    InternalResults ir = acceptGoodCharSequence1a( r ) ;
    boolean ok = ir.ok ;
    // while( ok ) -- Note 1. If multiple hyphenations groups were allowed
    if  ( ok )
    {
      String sa = ir.s ;
      ir = acceptGoodCharSequence0( r ) ;
      //ok = ir.ok ; -- Note 1.
      if  ( ir.s.length() > 0 )
      {
        s = s + sa + ir.s ;
        if  ( ir.ok )
        {
          ir = acceptGoodCharSequence1a( r ) ;
          //ok = ir.ok ; -- Note 1.
        }
      }
      //else ok = false ; -- Note 1.
    }
    ir.s = s ;
    // ir.ok is currently set correctly
    return( ir ) ;
  }

  public String nextSymbol( Reader r )
  {
    try
    {
      if  ( !purgeBadLeadingChars( r ) )
        return( "" ) ;
      InternalResults ir = acceptGoodCharSequence0( r ) ;
      String s = ir.s ;
      ir = acceptGoodCharSequence1( r ) ;
      return( s + ir.s ) ;
    }
    catch( Exception e )
    {
      e.printStackTrace() ;
    }
    return( "" ) ;
  }

}