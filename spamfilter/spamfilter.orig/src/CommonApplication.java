
//Title:      Common Word Counter
//Version:
//Copyright:  Copyright (c) 2000
//Author:     Bob Boyer
//Company:
//Description:Class Project CSE 218
package CmnWdCnt;

import java.io.* ;
import java.util.* ;
import gnu.regexp.* ; // BB Enhanced to accept Reader class

public class CommonApplication {

	boolean makeLower = true ;
  ICmnReader cmnReader = null ;
  int numOutputWords = 0 ;
  ICmnLexer lexer = null ;
  WordDataBase wordDB = null ;
  WordDataBase noiseDB = null ;

  String regexp = null ;
  CmnOutputOrganizer org = null ;

  public CommonApplication()
  {
  }

  public void SetRE( String regexp )
  {
    this.regexp = regexp ;
  }

// Input Related Functions
  public void SetupInputReqs( String[] args )
  {
    // Read only from stdin
    // cmnReader = new CmnReaderStdin() ;

    // Read from files whose names are specified on cmd line
    CmnReaderIterList tmpCmnReader = new CmnReaderIterList() ;
    tmpCmnReader.SetFileNames( args ) ;
    cmnReader = tmpCmnReader ;

    // Read from files whose names are in file whose name is given on cmd line
    /*
    CmdReaderIterFile tmpCmdReader = new CmdReaderIterFile() ;
    tmpCmnReader.SetFileNames( args[ 0 ] ) ;
    cmnReader = tmpCmnReader ;
    */

    // Create a count oriented data base
    wordDB = new CountDataBase() ;
    //wordDB = new WordDataBase() ;

    // Create a reg exp oriented lexer
    lexer = new CmnLexerRE( regexp ) ;
    //lexer = new CmnLexerRE( "[A-Z]+([ \t]*[A-Z])*" ) ;
    //lexer = new CmnLexerRE( "[;([!$#]" ) ;
    //lexer = new CmnLexerRE( "[A-Za-z0-9]+" ) ;
    //lexer = new CmnLexerRE( "[A-Za-z]+(-*[A-Za-z]+){0,1}" ) ;

    // Create the noise word database
    // noiseDB = new WordDataBase() ;
    // should load the noise words....
  }

// Processing Function
  public void CommonProcessing()
  {
    Reader reader = null ;

    // For stdin, only need to ask for reader once.
    /*
    reader = cmnReader.GetReader() ;
    ProcessReader( countDB, lexer, reader ) ;
    cmnReader.ReleaseReader( reader ) ;
    */

    // For multiple input files, must ask for next until get a null return
    while( null != ( reader = cmnReader.GetReader() ) )
    {
      ProcessReader( wordDB, lexer, reader ) ;
      cmnReader.ReleaseReader( reader ) ;
    }
  }

  public void ProcessReader(
    WordDataBase db,
    ICmnLexer lexer,
    Reader reader )
  {
    String str ;
    lexer.SetReader( reader ) ;
    while( lexer.hasMoreElements() )
    {
      str = lexer.nextString() ;
			if  ( makeLower )
				str = str.toLowerCase() ;
      if  ( null != str )
        if  ( null != noiseDB )
        {
          if  ( !noiseDB.HasWord( str ) )
            db.AddWord( str ) ;
        }
        else db.AddWord( str ) ;
    }
  }


// Output Related Functions
  public void SetupOutputReqs( String[] args )
  {
    numOutputWords = 100 ;
  }

  public int GetNumOutputWords()
  {
    return( numOutputWords ) ;
  }

  public Writer GetWriter()
  {
    try
    {
      return( new BufferedWriter( new OutputStreamWriter( System.out ) ) ) ;
    }
    catch( Exception e )
    {
      e.printStackTrace() ;
    }
    return( null ) ;
  }

  public void ReleaseWriter( Writer writer )
  {
    try
    {
      writer.flush() ;
      // Do nothing because the writer is stdout
    }
    catch( IOException e )
    {
      e.printStackTrace() ;
    }
  }

  public void SetOutputOrganizer( CmnOutputOrganizer org )
  {
    this.org = org ;
  }


  public void CommonResults()
  {
    //CmnOutputSpamCharOrganizer org = new CmnOutputSpamCharOrganizer() ;
    //CmnOutputSpamWordOrganizer org = new CmnOutputSpamWordOrganizer() ;
    //CmnOutputFreqOrganizer org = new CmnOutputFreqOrganizer() ;
    //CmnOutputCountOrganizer org = new CmnOutputCountOrganizer() ;
    // CmnOutputOrganizer org = new CmnOutputOrganizer() ;  // for just words

    Iterator iterator = org.OrganizeOutputs( wordDB ) ;

    int i = 0 ;

    Writer writer = GetWriter() ;
    if  ( null != writer )
    {
// to do all words for output, eliminate the limit check on i.
      while( ( i++ < GetNumOutputWords() )  &&  iterator.hasNext() )
      {
        try
        {
          writer.write( iterator.next() + "\n" ) ;
        }
        catch( IOException e )
        {
          e.printStackTrace() ;
        }
      }
      ReleaseWriter( writer ) ;
    }
  }

// and of course main()
  public static void main(String[] args) {
    CommonApplication common = new CommonApplication() ;

    String regexps[] = { "[A-Za-z0-9]+", ".", "[A-Z]+([.!$? \t]*[A-Z])*" } ;
    CmnOutputOrganizer orgs[] = {
      new CmnOutputSpamWordOrganizer(),
      new CmnOutputSpamCharOrganizer(),
      new CmnOutputCapsOrganizer()
    } ;

		boolean lowers[] = { true, false, false } ;

    for( int i = 0 ;   i < regexps.length ;   i++ )
    {
			common.makeLower = lowers[ i ] ;

      common.SetRE( regexps[ i ] ) ;
      common.SetupInputReqs( args ) ;

      common.SetOutputOrganizer( orgs[ i ] ) ;
      common.SetupOutputReqs( args ) ;

      common.CommonProcessing() ;
      common.CommonResults() ;
    }
  }
}
