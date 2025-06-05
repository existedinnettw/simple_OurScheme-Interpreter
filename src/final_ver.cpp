
# include <iomanip>
# include <map>
# include <cstdio>
# include <string>
# include <cstring>
# include <cstdlib>
# include <iostream>
# include <vector>
# include <stack>
# include <ctype.h>
# include <sstream>
# include <string>
# include <cmath>

using namespace std;

char gTestNum = '\0' ;

enum TokenType {
  LEFT_PAREN, INT, STRING, FLOAT, NIL, T, SYMBOL, RIGHT_PAREN, DOT,
  DOTP, HASH, OPERATOR, QUOTE, CONS, LIST, DEFINE, CAR, CDR, ISATOM, ISPAIR, ISLIST, ISNULL,
  ISINT, ISREAL, ISNUM, ISSTR, ISBOOL, ISSYM, ADD, MINU, MUL, DIV,
  AND, OR, NOTT, BIGGER, BIG_EQ, SMAL, SMAL_EQ, EQUU, STRING_APP, STRING_BIG,
  STRING_SML, STRING_EQU, EQV, EQU, BEGIN, IF, COND, CLEAN, EXIT, LET, LAMBDA, VERBOS, ISVERBOS,
  PRINT, USDEFINE, EMPTY
};

string gmsg [14] = { "LEFT_PAREN", "INT", "STRING", "FLOAT",
  "NIL", "T", "SYMBOL", "RIGHT_PAREN", "DOT",
  "DOTP", "HASH", "OPERATOR", "EMPTY", "QUOTE" };

string gFunc_msg [43] = { "quote", "cons" , "list", "define", "car" , "cdr", "atom?", "pair?",
                     "list?", "null?", "integer?", "real?", "number?", "string?",
                     "boolean?", "symbol?", "+", "-" , "*" , "/", "and", "or" ,
                     "not" , ">", ">=" , "<", "<=", "=", "string-append", "string>?",
                     "string<?", "string=?", "eqv?", "equal?", "begin", "if", "cond","clean-environment",
                     "exit", "let", "lambda", "verbose", "verbose?" };


enum ErrorType  {
  EXP_RIGHTP, EXP_ATOMORLEFTP, EOLENC, EOFENC, NONFUNC, NON_LIST, WRONGTYPE, DEFERR,
  WRONGARGNUM, UNBOND, DEFLVERR, CLEANLVERR, EXITLVERR, DIVZRO, NORET, CNDERR, LETERR,
  LAMBERR, UNBONDPARA, UNBONDTEST, UNBONDCOND, NORET_B, SETERR
};

class DefineFuncton ;

class Token {
public:
  bool mfromquote;
  int mtype;
  string mtoken;
  Token* mleftnode;
  Token* mrightnode;
  DefineFuncton* muserfunc;

  Token() {
    mtype = EMPTY;
    mtoken = "";
    mleftnode = NULL;
    mrightnode = NULL;
    mfromquote = false;
    muserfunc = NULL;
  } // Token()

  Token( string content, int type ) {
    mtype = type;
    mtoken = content;
    mleftnode = NULL;
    mrightnode = NULL;
    mfromquote = false;
    muserfunc = NULL;

  } // Token()

  void SetttToken( string content, int type ) {
    mtype = type;
    mtoken = content;

  } // SetttToken()
};
typedef Token* TokenPtr ;
map<string,TokenPtr> gCmdMap;
map<int, map<string,TokenPtr> > gparaMap;

map<int, map<string,TokenPtr> >::iterator gitr;
map<string,TokenPtr>::iterator gptr;

int gmaplayer = -1;
void PrtExp( TokenPtr troot, int space, bool left_par, bool right )  ;
void PrtExp_Succ( TokenPtr troot, int space, bool left_par, bool right ) ;
Token * EvalExp( TokenPtr troot, int level, map<string,TokenPtr> * paraMap ) ;
bool IsProcFn( TokenPtr token ) ;

class DefineFuncton {
public:
  string mdefname;
  vector<string> marg ;
  vector<TokenPtr> mfunction ;
  DefineFuncton() {
    mdefname = "";
  } // DefineFuncton()
};

class Parser {
public:
  int mline ;
  int mcol ;
  bool mexit ;
  bool mexp_checker;
  bool mright_pare;
  bool mverb;
  bool mreq_type [13];
  TokenPtr mroot;

  Parser() {
    mline = 1;
    mcol = 0;
    mexit = false;
    mexp_checker = false ;
    mright_pare = false;
    for ( int i = 0 ; i < 13 ; ++i )
      mreq_type[i] = true;
    mroot = NULL;
    mverb = true;
  } // Parser()

  void Init() {
    mline = 1;
    mcol = 0;
    mverb = true;
    mroot = NULL;
    for ( int i = 0 ; i < 13 ; ++i )
      mreq_type[i] = true;
  } // Init()

  void First_Exp() {
    mreq_type[RIGHT_PAREN] = false;
    mreq_type[DOT] = false;
  } // First_Exp()

  void Left_Exp() {
    mreq_type[RIGHT_PAREN] = true;
    mreq_type[DOT] = false;
  } // Left_Exp()

  void Sec_Exp() {
    mreq_type[RIGHT_PAREN] = true;
    mreq_type[DOT] = true;
  } // Sec_Exp()

  char Getchar() {
    char temp = getchar();
    mcol++;
    if ( temp == EOF || temp == -1 ) {
      // cout << "+++++ EOF" << endl;
      return EOF ;
    } // if()

    return temp;
  } // Getchar()

  char Peek() {
    if ( cin.peek() != EOF ) {
      char temp = cin.peek();
      return temp;
    } // if()

    return EOF;
  } // Peek()


  char Skipcomment() {
    char temp = getchar();
    while ( temp != '\n' && temp != -1 )
      temp = getchar();

    return temp ;
  } // Skipcomment()

} ;

Parser gparser ;

class ErrorExp {
public:
  bool merror;
  TokenPtr merrorop;
  int merrortype ;
  int margutype ;

  ErrorExp() {
    merror = false;
    merrorop = NULL ;
    merrortype = -1;
    margutype = -1 ;
  } // ErrorExp()
  
  ErrorExp( bool err, TokenPtr errop, int errtype, int errmargutype ) {
    merror = err;
    merrorop = errop ;
    merrortype = errtype;
    margutype  = errmargutype ;
  } // ErrorExp()
  
  ErrorExp( bool err, TokenPtr errop, int errtype ) {
    merror = err;
    merrorop = errop ;
    merrortype = errtype;
  } // ErrorExp()

  void Init() {
    merror = false;
    merrorop = NULL ;
    merrortype = -1;
  } // Init()

  void SeterrExp( bool err, TokenPtr errop, int errtype ) {
    merror = err;
    merrorop = errop ;
    merrortype = errtype;
  } // SeterrExp()

  void PrtError( ErrorExp trooterr ) {
    /*
    if ( IsProcFn( trooterr.merrorop ) )
      trooterr.merrorop->mtoken = gFunc_msg[ trooterr.merrorop->mtype - 12 ] ;
    */
    
    if ( trooterr.merrortype == EXP_RIGHTP || trooterr.merrortype == EXP_ATOMORLEFTP ) {
      // cout <<" column"<< gparser.mcol - trooterr.merrorop->mtoken.size() + 1 ;
      if ( trooterr.merrortype == EXP_RIGHTP )
        cout << "ERROR (unexpected token) : ')' expected when token at Line " ;
      else
        cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " ;
      cout << gparser.mline
         << " Column "
         << gparser.mcol - trooterr.merrorop->mtoken.size() + 1
         << " is >>"
         << trooterr.merrorop->mtoken
         << "<<" ;
    } // if()
    else if ( trooterr.merrortype == EOLENC ) {
      cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line "
         << gparser.mline
         << " Column "
         << gparser.mcol;
    } // else if()
    else if ( trooterr.merrortype == EOFENC ) {
      gparser.mexit = true;
      cout << "ERROR (no more input) : END-OF-FILE encountered" ;
    } // else if()
    else if ( trooterr.merrortype == DEFLVERR ) {
      cout << "ERROR (level of DEFINE)" << endl ;
    } // else if
    else if ( trooterr.merrortype == CLEANLVERR ) {
      cout << "ERROR (level of CLEAN-ENVIRONMENT)" << endl ;
    } // else if
    else if ( trooterr.merrortype == EXITLVERR ) {
      cout << "ERROR (level of EXIT)" << endl ;
    } // else if
    else if ( trooterr.merrortype == NONFUNC ) {
      cout << "ERROR (attempt to apply non-function) : " ;
      PrtExp_Succ( trooterr.merrorop, 0, false, false ) ;
    } // if
    else if ( trooterr.merrortype == WRONGTYPE ) {
      // cout <<"****checktype "<<trooterr.merrortype<<" "<< gFunc_msg[ trooterr.merrortype - 12
      // ] <<" "<<trooterr.merrorop->mtoken<< endl;
      cout << "ERROR (" << gFunc_msg[ trooterr.margutype ] << " with incorrect argument type) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == NON_LIST ) {
      cout << "ERROR (non-list) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == WRONGARGNUM ) {
      cout << "ERROR (incorrect number of arguments) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == UNBOND ) {
      cout << "ERROR (unbound symbol) : "  ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == DIVZRO ) {
      cout << "ERROR (division by zero) : /" << endl ;
    } // else if
    else if ( trooterr.merrortype == NORET || trooterr.merrortype == NORET_B ) {

      cout << "ERROR (no return value) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == DEFERR ) {
      cout << "ERROR (DEFINE format) : " ;
      PrtExp( gparser.mroot, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == CNDERR ) {
      cout << "ERROR (COND format) : " ;
      PrtExp( gparser.mroot, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == LETERR ) {
      cout << "ERROR (LET format) : " ;
      PrtExp( gparser.mroot, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == SETERR ) {
      cout << "ERROR (SET! format) : " ;
      PrtExp( gparser.mroot, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == LAMBERR ) {
      cout << "ERROR (LAMBDA format) : " ;
      PrtExp( gparser.mroot, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == UNBONDPARA ) {
      cout << "ERROR (unbound parameter) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == UNBONDTEST ) {
      cout << "ERROR (unbound test-condition) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if
    else if ( trooterr.merrortype == UNBONDCOND ) {
      cout << "ERROR (unbound condition) : " ;
      PrtExp( trooterr.merrorop, 0, false, false ) ;
    } // else if

  } // PrtError()

};

bool Has_Digit( string s ) {
  for ( int j = 0 ; j < s.size() ; j++ ) {
    if ( isdigit( s.at( j ) ) )
      return true ;
  } // for

  return false;
} // Has_Digit()

Token * GetToken(  ErrorExp &trooterr ) {
  bool tdone = false ;
  bool iscomment = false;
  char thischar = '\0';
  char nextchar = '\0';

  Token * temptoken = new Token();

  while ( !tdone  ) {
    thischar = gparser.Peek() ;
    if ( thischar == EOF ) {
      trooterr.SeterrExp( true, temptoken, EOFENC ) ;
      tdone = true;
    } // if( -1 error occur )
    else {
      // cout << thischar << endl;
      if ( temptoken->mtype == EMPTY ) {
        // cout << "@@@@@@@empty " << thischar << " "  ;

        thischar = gparser.Getchar() ;
        nextchar = gparser.Peek() ;

        if ( thischar == '(' || thischar == ')' ) {
          if ( thischar == '(' ) {
            if ( nextchar == ')' ) {
              nextchar = gparser.Getchar() ;
              temptoken->mtype = NIL;
              temptoken->mtoken = "nil" ;
            } // if()
            else
              temptoken->mtype = LEFT_PAREN;
          } // if()
          else
            temptoken->mtype = RIGHT_PAREN;

          tdone = true;
        } // if()
        else if ( thischar == '\n' ) {

          if ( gparser.mexp_checker ) {
            gparser.mline = 1;
            gparser.mexp_checker = false;
          } // if()
          else
            gparser.mline++;

          gparser.mcol = 0;
        } // else if()
        else if ( thischar == ';' ) {

          while ( nextchar != '\n' && nextchar != -1 ) {
            nextchar = gparser.Getchar() ;
            nextchar = cin.peek();
          } // while ()

          if ( nextchar == -1 && !gparser.mexit ) {
            trooterr.SeterrExp( true, temptoken, EOFENC ) ;
            tdone = true;
          } // if( -1 error occur )

        } // else if
        else if ( thischar == '.' ) {
          temptoken->mtype = DOT;
        } // else if
        else if ( thischar == '#' ) {
          temptoken->mtype = HASH;
        } // else if
        else if ( thischar == '+' ||  thischar == '-' ) {
          temptoken->mtype = OPERATOR;
        } // else if
        else if ( thischar == '\'' ) {
          temptoken->mtype = QUOTE;
          tdone = true;
        } // else if
        else if ( thischar == '\"' ) {
          temptoken->mtype = STRING;
        } // else if
        else if ( isdigit( thischar ) ) {
          temptoken->mtype = INT;
        } // else if
        else if ( isspace( thischar ) ) {
        } // else if
        else if ( isalpha( thischar ) ) {
          if ( thischar == 't' )
            temptoken->mtype = T;
          else
            temptoken->mtype = SYMBOL;

        } // else if
        else
          temptoken->mtype = SYMBOL;

        if ( !isspace( thischar ) && thischar != ';' && temptoken->mtype != NIL )
          temptoken->mtoken += thischar;
      } // if( empty )
      else {
        // cout << "@@@@@@@notempty " << thischar << " "  ;

        if ( temptoken->mtype == STRING ) {

          if ( thischar == '\"' ) {
            thischar = gparser.Getchar();
            temptoken->mtoken += thischar;
            tdone = true;
          } // if ()
          else {
            if ( thischar == '\n' ) {
              ++ gparser.mcol;
              tdone = true;
              trooterr.SeterrExp( true, temptoken, EOLENC ) ;
            } // if()
            else {
              thischar = gparser.Getchar();
              nextchar = gparser.Peek();

              if ( nextchar == EOF || nextchar == '\n' ) {
                ++ gparser.mcol;
                temptoken->mtoken += thischar;
                if ( nextchar == EOF )
                  trooterr.SeterrExp( true, temptoken, EOFENC ) ;
                else if ( nextchar == '\n' )
                  trooterr.SeterrExp( true, temptoken, EOLENC ) ;

                tdone = true;
              } // if()
              else {
                int i = 0;
                if ( thischar == '\\' ) {
                  if ( nextchar != EOF ) {
                    nextchar = gparser.Getchar() ;
                    if ( nextchar == 'n' )
                      thischar = '\n';
                    else if ( nextchar == 't' )
                      thischar = '\t';
                    else if ( nextchar == '\"' )
                      thischar = '\"';
                    else if ( nextchar == '\\' )
                      ;
                    else
                      i++ ;
                  } // if()
                } // if()

                temptoken->mtoken += thischar;

                if ( i > 0 )
                  temptoken->mtoken += nextchar;
              } // else ()


            } // else()
          } // else()

        } // if()
        else if ( temptoken->mtype == DOT ) {
          // cout << " .... "<< thischar << endl;

          if ( thischar == '(' || thischar == ')' || isspace( thischar ) || thischar == ';'
               || thischar == '\'' || thischar == '\"' ) {
            if ( !gparser.mreq_type[DOT] ) {
              // cout << "********* not dot error" << endl;
              trooterr.SeterrExp( true, temptoken, EXP_ATOMORLEFTP ) ;
            } // if()

            tdone = true;
          } // if()
          else {
            thischar = gparser.Getchar();
            // cout << " .... "<< thischar << endl;

            if ( isdigit( thischar ) ) {
              temptoken->mtype = FLOAT;
              temptoken->mtoken += thischar;
            } // if()
            else {
              temptoken->mtype = SYMBOL;
              temptoken->mtoken += thischar;
            } // else()
          } // else()
        } // else if()
        else if ( temptoken->mtype == HASH ) { // ( -> symbol
          // cout << " **** "<< thischar << endl;
          if ( thischar == '(' || thischar == ')' || isspace( thischar ) || thischar == ';'
               || thischar == '\'' || thischar == '\"' ) {
            temptoken->mtype = SYMBOL;
            tdone = true;
          } // else if()
          else {
            thischar = gparser.Getchar();
            // cout << " **** "<< thischar << endl;

            if ( isalpha( thischar ) ) {
              if ( thischar == 'f' )
                temptoken->mtype = NIL;
              else if ( thischar == 't' )
                temptoken->mtype  = T;
              else
                temptoken->mtype  = SYMBOL;
            } // if()
            else
              temptoken->mtype = SYMBOL;

            temptoken->mtoken += thischar;
          } // else()
        } // else if()
        else if ( temptoken->mtype == OPERATOR ) { // ( -> symbol
          if ( thischar == '(' || thischar == ')' || thischar == '\''
               || thischar == '\"' || isspace( thischar ) || thischar == ';' ) {
            temptoken->mtype = SYMBOL;
            tdone = true;
          } // if()
          else {
            thischar = gparser.Getchar();

            if ( isdigit( thischar ) ) {
              temptoken->mtype = INT;
              temptoken->mtoken += thischar;
            } // else if
            else if ( thischar == '.' ) {
              temptoken->mtype  = FLOAT;
              temptoken->mtoken += thischar;
            } // else if
            else {
              temptoken->mtype = SYMBOL;
              temptoken->mtoken += thischar;
            } // else()
          } // else()
        } // else if()
        else {
          // cout << "@@@@@@@notempty " << thischar << " "  ;

          if ( thischar == '(' || thischar == ')' || thischar == '\''
               || thischar == '\"' || isspace( thischar ) || thischar == ';' ) {
            if ( temptoken->mtype == NIL )
              temptoken->mtoken = "nil" ;
            else if ( temptoken->mtype == T )
              temptoken->mtoken = "#t" ;
            else if ( temptoken->mtype == INT ) {
              stringstream str;
              str << atoi( temptoken->mtoken.c_str() );
              temptoken->mtoken = str.str();
            } // else if
            else if ( temptoken->mtype == FLOAT ) {
              if ( Has_Digit( temptoken->mtoken ) ) {
                stringstream str ;
                str << fixed << setprecision( 3 ) << atof( temptoken->mtoken.c_str() ) ;
                temptoken->mtoken = str.str();
              } // if()
              else
                temptoken->mtype = SYMBOL ;
            } // else if
            else if ( temptoken->mtype == SYMBOL ) {
              if ( strcmp( temptoken->mtoken.c_str(), "nil" ) == 0 )
                temptoken->mtype = NIL ;
            } // else if()

            tdone = true;
          } // if()
          else if ( temptoken->mtype == FLOAT ) { // ( xchange
            thischar = gparser.Getchar() ;

            if ( isdigit( thischar ) ) {
              temptoken->mtoken += thischar;
            } // if()
            else {
              temptoken->mtype = SYMBOL;
              temptoken->mtoken += thischar;
            } // else()
          } // else if ( float )
          else if ( temptoken->mtype == INT ) { // ( xchange
            thischar = gparser.Getchar();

            if ( isdigit( thischar ) ) {
              temptoken->mtype = INT;
              temptoken->mtoken += thischar;
            } // if()
            else if ( thischar == '.' ) {
              temptoken->mtype  = FLOAT;
              temptoken->mtoken += thischar;
            } // else if()
            else {
              temptoken->mtype = SYMBOL;
              temptoken->mtoken += thischar;
            } // else()
          } // else if()
          else if ( temptoken->mtype == NIL || temptoken->mtype == T
                    || temptoken->mtype == SYMBOL ) { // ( xchange
            thischar = gparser.Getchar();
            temptoken->mtype = SYMBOL;
            temptoken->mtoken += thischar;
          } // else if()
        } // else()
      } // else( is not first )
    } // else()
  } // while()

  // cout << "*********** check error" << endl;
  if ( !trooterr.merror ) {
    if ( !gparser.mreq_type[temptoken->mtype] ) {
      /*
      cout << "********* !gparser.mreq_type[temptoken->mtype] " << temptoken->mtoken <<
              " " << temptoken->mtype << " thischar: " << thischar << " nextchar: " <<
              nextchar << endl;
      */
      
      trooterr.SeterrExp( true, temptoken, EXP_ATOMORLEFTP ) ;
      tdone = true ;
    } // if()
    // cout << "*********** check error" << endl;

  } // if()

  return temptoken;

} // GetToken()

void SetToken( TokenPtr & troot, string content, int type ) {
  troot->mtype = type;
  troot->mtoken = content;
} // SetToken()


void ReturnToken( TokenPtr troot ) {
  for ( int j = 0 ; j < troot->mtoken.size() ; ++ j ) {
    cin.putback( troot->mtoken.at( troot->mtoken.size() - 1 - j ) ) ;
    --gparser.mcol ;
  } // for()
} // ReturnToken()

bool Synex_check( Token * temptoken, ErrorExp& trooterr ) {
  /*
  <S-exp> ::= <ATOM>
          | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
            | QUOTE <S-exp>
  */
  if ( !trooterr.merror ) {
    if ( !gparser.mreq_type[temptoken->mtype] ) {
      // cout << "********* Synex_check" << endl;

      trooterr.SeterrExp( true, temptoken, EXP_ATOMORLEFTP ) ;
      return false ;
    } // if()
    // cout << "*********** synex check error" << endl;
  } // if()

  return true ;
} // Synex_check()


void BuildExp( TokenPtr troot, ErrorExp &trooterr, bool left, bool right ) {
  // cout<<"++++ troot->mtype: "<< troot->mtype << " "<<troot->mtoken<<endl;
  if ( trooterr.merror )
    return;
  else if ( !Synex_check( troot, trooterr ) )
    return;
  else {
    gparser.mright_pare = false ;
    if ( troot->mtype == QUOTE ) { // node = ' -> (quote.(
      // cout << "++ quote"<< endl ;
      gparser.First_Exp() ;
      if ( right ) {  // is rightchild
        SetToken( troot, ".(", DOTP )  ;
        troot->mleftnode = new Token( "(", LEFT_PAREN ) ;
        troot->mleftnode->mleftnode = new Token( "quote", QUOTE ) ;
        troot->mleftnode->mrightnode = new Token( ".(", DOTP ) ;
        troot->mleftnode->mrightnode->mleftnode = GetToken( trooterr ) ;
        BuildExp( troot->mleftnode->mrightnode->mleftnode, trooterr, true, false ) ;
      } // if()
      else {
        SetToken( troot, "(", LEFT_PAREN )  ;
        troot->mleftnode = new Token( "quote", QUOTE ) ;
        troot->mrightnode = new Token( ".(", DOTP ) ;
        troot->mrightnode->mleftnode = GetToken( trooterr ) ;
        BuildExp( troot->mrightnode->mleftnode, trooterr, true, false ) ;
      } // else()
    } // if()
    else if ( troot->mtype == LEFT_PAREN ) { // node = ( || .( ( by input )
      gparser.Left_Exp();
      Token * tempNode = GetToken( trooterr ) ;
      // cout<<"++++ LEFT_PAREN next token: "<< tempNode->mtoken <<endl;

      if ( !Synex_check( tempNode, trooterr ) )
        return;
      else if ( tempNode->mtype == RIGHT_PAREN ) { // ()
        // cout<<"++++ (): "<< tempNode->mtype <<endl;
        SetToken( troot, "nil", NIL )  ;
        gparser.mright_pare = false;
        return;
      } // else if()
      else {
        // cout<<"++++ trootis( : "<< tempNode->mtoken <<endl;

        if ( right ) {  // is rightchild
          if ( troot->mtoken == ".(" ) {
            // cout <<" ++++++ .(" << troot->mtype <<  endl;
            troot->mleftnode = new Token( tempNode->mtoken, tempNode->mtype ) ;
            BuildExp( troot->mleftnode, trooterr, true, false ) ;
          } // if()
          else {
            // cout <<" ++++++ (" << troot->mtype <<  endl;

            SetToken( troot, ".(", DOTP )  ;
            troot->mleftnode = new Token( "(", LEFT_PAREN ) ;
            ReturnToken( tempNode ) ;
            // cout << "       ++" << troot->mleftnode->mleftnode->mtype << "/ "
            // << troot->mleftnode->mleftnode->mtoken ;
            BuildExp( troot->mleftnode, trooterr, true, false ) ;
          } // else()
        } // if()
        else { // isleftchid or root
          // cout<<"++++ ( is left: "<< tempNode->mtoken <<endl;

          troot->mleftnode = new Token( tempNode->mtoken, tempNode->mtype ) ;
          // cout<<"++++ setdone: "<< tempNode->mtoken <<endl;

          gparser.Left_Exp();
          BuildExp( troot->mleftnode, trooterr, true, false ) ;
        } // else()

        gparser.mright_pare = false;
      } // else()
    } // else if()
    else if ( troot->mtype == DOTP ) { // node =.( ( by me )
      BuildExp( troot->mleftnode, trooterr, true, false ) ;
    } // else if()
    else {
      gparser.mright_pare = false;
      return;
    } // else()

    if ( trooterr.merror )
      return;

    if ( troot->mrightnode == NULL ) {
      gparser.Sec_Exp() ;
      troot->mrightnode = GetToken( trooterr );

      if ( trooterr.merror )
        return;
      else if ( !Synex_check( troot->mrightnode, trooterr ) )
        return;
      else {
        if ( troot->mrightnode->mtype == DOT ) {

          gparser.First_Exp() ;
          troot->mrightnode = GetToken( trooterr );

          if ( !Synex_check( troot->mrightnode, trooterr ) )
            return;
          else if ( troot->mrightnode->mtype == LEFT_PAREN ) {
            SetToken( troot->mrightnode, ".(", LEFT_PAREN ) ;
            BuildExp( troot->mrightnode, trooterr, false, true ) ;
            gparser.mright_pare = false;

          } // else if()
          else if ( troot->mrightnode->mtype == QUOTE ) {
            SetToken( troot->mrightnode, ".(", DOTP ) ;
            troot->mrightnode->mleftnode = new Token( "quote", QUOTE ) ;
            BuildExp( troot->mrightnode, trooterr, false, true ) ;
          } // else if()
          else {
            gparser.mright_pare = false;
          } // else()
        } // if()
        else if (  troot->mrightnode->mtype == RIGHT_PAREN ) {
          gparser.mright_pare = true ;
          return ;
        } // else if()
        else if (  troot->mrightnode->mtype == QUOTE ) {
          BuildExp( troot->mrightnode, trooterr, false, true ) ;
        } // else if()
        else {
          troot->mrightnode->mleftnode = new Token( troot->mrightnode->mtoken, troot->mrightnode->mtype ) ;
          SetToken( troot->mrightnode, ".(", DOTP )  ;
          // cout <<" root: " << troot->mtoken << "root right: "
          BuildExp( troot->mrightnode, trooterr, false, true ) ;
        } // else()
      } // else()
    } // if()

    /*
    cout << "+ Getright "<<troot->mtoken<< "left: "<< troot->mleftnode->mtoken <<" right:"
         << troot->mrightnode->mtoken << " mright_pare: "<<gparser.mright_pare << endl ;
    */

    if ( troot->mtype == LEFT_PAREN && troot->mleftnode->mtype != QUOTE
         && troot->mrightnode->mtype != RIGHT_PAREN && !gparser.mright_pare && !trooterr.merror ) {
      // cout << "+ Getright "<< endl ;
      gparser.mreq_type[RIGHT_PAREN] = true;
      Token *tempNode = GetToken( trooterr );
      if ( tempNode->mtype == RIGHT_PAREN ) {
        gparser.mright_pare = false;
        return ;
      } // if()
      else {
        trooterr.SeterrExp( true, tempNode, EXP_RIGHTP ) ;
        return ;
      } // else()
    } // if()


    return;
  } // else ()

} // BuildExp()

// -------------- Project2 --------

void Retype( TokenPtr troot ) {
  
  if ( troot->mtype == RIGHT_PAREN ) {
    troot->SetttToken( "nil", NIL ) ;
  } // if()
  else if ( troot->mleftnode != NULL && troot->mrightnode == NULL ) {
    troot->mrightnode = new Token( "nil", NIL ) ;
  } // else if


  if ( troot->mleftnode != NULL ) Retype( troot->mleftnode ) ;
  if ( troot->mrightnode != NULL ) Retype( troot->mrightnode ) ;
} // Retype()

void Map_InternalCmd() {
  for ( int i = 0 ; i < 43 ; ++ i ) {
    stringstream str ;
    str << "#<procedure " << gFunc_msg[ i ] << ">" ;
    gCmdMap[ gFunc_msg[ i ] ] = new Token( str.str(), i + 12 ) ;
    // cout << gCmdMap[ gFunc_msg[ i ] ]->mContent << endl ;
  } // for
} // Map_InternalCmd()

bool FindDefine( TokenPtr token  ) {
  if ( gCmdMap.find( token->mtoken ) != gCmdMap.end() )
    return true ;
  else
    return false ;
} // FindDefine()

/*
bool FindparaDefine( TokenPtr token, int & layer ) {
  int count = -1;
  cout<< " FindparaDefine layer: " << gmaplayer <<endl;
  for ( count = gmaplayer ; count >= 0 ; --count ) {
    gitr = gparaMap.find( count ) ;

    if ( gitr != gparaMap.end() ) {
      gptr = gitr->second.find( token->mtoken );

      if ( gptr != gitr->second.end() ) {
        cout<< " ....return layer: "<<count <<" "<<endl;
        layer = count;
        
        return true ;
      } // if()
    } // if()
  } // for()

  return false ;
} // FindparaDefine()
*/

void Getargsize( TokenPtr token, vector<Token*> & argList ) {
  int num = 0 ;
  TokenPtr currentNode = token->mrightnode ;
  while (  currentNode != NULL  ) {
    if ( currentNode->mleftnode == NULL && currentNode->mtype != NIL
         && currentNode->mtype != RIGHT_PAREN ) {
      throw ErrorExp( true, token, NON_LIST ) ;
    } // if
    else if ( currentNode->mtype != NIL ) {
      argList.push_back( currentNode->mleftnode ) ;
    } // else if

    currentNode = currentNode->mrightnode ;
  } // while()

} // Getargsize()

bool IsProcFn( TokenPtr token ) {
  for ( int i = 0 ; i < 43 ; i++ ) {
    if ( strcmp( token->mtoken.c_str(), gFunc_msg[i].c_str() ) == 0 && !token->mfromquote  ) {
      // check ( exit )
      token->mtype = i + 12 ;
      return true ;
    } // if ()
  } // for()

  if ( token->mtype >= QUOTE && token->mtype <= USDEFINE )
    return true;

  return false ;
} // IsProcFn()

void CheckParaSize( TokenPtr token, int level, vector<Token*> & argList ) {
  if ( token->mtype == DEFINE ) {
    if ( level != 0 )
      throw ErrorExp( true, token, DEFLVERR ) ;
    if ( argList.size() != 2 ) {
      throw ErrorExp( true, token, DEFERR ) ;
    } // if()
  } // if()
  else if ( token->mtype == CLEAN || token->mtype == EXIT ) {
    if ( level != 0 ) {
      if ( token->mtype == CLEAN )
        throw ErrorExp( true, token, CLEANLVERR ) ;
      else if ( token->mtype == EXIT )
        throw ErrorExp( true, token, EXITLVERR ) ;

    } // if()

    if ( argList.size() != 0 ) {
      throw ErrorExp( true, token, WRONGARGNUM ) ;
    } // if()

  } // if() : if situation
  else if ( token->mtype == CONS || token->mtype == EQV || token->mtype == EQU  ) {
    if ( argList.size() != 2 ) {
      throw ErrorExp( true, token, WRONGARGNUM ) ;
    } // if()
  } // else if()
  else if ( token->mtype == IF ) {
    if ( argList.size() != 2 && argList.size() != 3 ) {
      throw ErrorExp( true, token, WRONGARGNUM ) ;
    } // if() : not 2 or 3
  } // else if()
  else if ( token->mtype == ADD || token->mtype == MINU || token->mtype == MUL || token->mtype == DIV ||
            token->mtype == AND || token->mtype == OR || token->mtype == BIGGER || token->mtype == BIG_EQ ||
            token->mtype == SMAL || token->mtype == SMAL_EQ || token->mtype == EQUU ||
            token->mtype == STRING_APP || token->mtype == STRING_BIG ||
            token->mtype == STRING_SML || token->mtype == STRING_EQU ) {
    if ( argList.size() < 2 ) {
      throw ErrorExp( true, token, WRONGARGNUM ) ;
    } // if()
  } // else if()
  else if ( token->mtype == BEGIN ) {
    if ( argList.size() < 1 ) {
      throw ErrorExp( true, token, WRONGARGNUM ) ;
    } // if() : not >= needNum
  } // if() : cond format
  else if ( token->mtype == COND ) {
    if ( argList.size() < 1 ) {
      throw ErrorExp( true, token, CNDERR ) ;
    } // if() : not >= needNum
  } // if() : cond format
  else if ( token->mtype == LET || token->mtype == LAMBDA ) {
    if ( argList.size() < 2 ) {
      if ( token->mtype == LET ) {
        // cout << " argusize error " << endl;
        throw ErrorExp( true, token, LETERR ) ;
      } // if()
      else if ( token->mtype == LAMBDA )
        throw ErrorExp( true, token, LAMBERR ) ;
    } // if() : not >= needNum
  } // if() : cond format
  else if ( token->mtype == USDEFINE ) {
    if ( argList.size() != token->muserfunc->marg.size() ) {
      TokenPtr returnt = new Token( token->muserfunc->mdefname, USDEFINE ) ;
      throw ErrorExp( true, returnt, WRONGARGNUM ) ;
    } // if() : not >= needNum
  } // else if() : cond format
  else {
    if ( argList.size() != 1 ) {
      throw ErrorExp( true, token, WRONGARGNUM ) ;
    } // if()
  } // else

} // CheckParaSize()

Token* IsCons( TokenPtr token, int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = new Token( "(", LEFT_PAREN ) ;

  result->mleftnode = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;
  result->mrightnode = EvalExp( argList.at( 1 ), level + 1, paraMap ) ;
  // cout<<"******* "<< result->mleftnode->mtoken <<endl;

  return result ;
} // IsCons()

Token* List( TokenPtr token, int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  Token* result = new Token( "nil", NIL ) ;
  if ( argList.size() > 0 ) {
    SetToken( result, "(", LEFT_PAREN ) ;
    Token* currrentNode = result ;
    result->mleftnode = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;
    for ( int i = 1 ; i < argList.size() ; i++, currrentNode = currrentNode->mrightnode ) {
      currrentNode->mrightnode = new Token( ".(", DOTP ) ;
      currrentNode->mrightnode->mleftnode = EvalExp( argList.at( i ),
                                                              level + 1, paraMap ) ;
      currrentNode->mrightnode->mrightnode = new Token( "nil", NIL ) ;
    } // for

  } // if()

  return result ;
} // List()

void SetQuote( TokenPtr token  ) {
  token->mfromquote = true ;
  if ( token->mleftnode != NULL ) SetQuote( token->mleftnode ) ;
  if ( token->mrightnode != NULL ) SetQuote( token->mrightnode ) ;
} // SetQuote()

Token * IsQuote( TokenPtr token, int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  SetQuote( token->mrightnode->mleftnode ) ;

  return token->mrightnode->mleftnode;

} // IsQuote()

Token* IsDefine( TokenPtr token, int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  Token* result = argList.at( 0 ) ;
  // cout << " define 0 " << token->mtoken <<endl;
  
  if ( ( result->mtype != SYMBOL && result->mtype != LEFT_PAREN ) ||
       IsProcFn( result ) ) {
    throw ErrorExp( true, token, DEFERR ) ;
  } // if() : first token is not symbol or is a function

  if ( result->mtype == LEFT_PAREN ) {

    TokenPtr usrdefine = new Token() ;
    usrdefine->muserfunc = new DefineFuncton() ;
    TokenPtr currentNode = result ;

    for ( int i = 0; currentNode != NULL ;
          currentNode = currentNode->mrightnode, ++i ) {
      if ( currentNode->mrightnode == NULL
           && currentNode->mtype != NIL && currentNode->mtype != RIGHT_PAREN ) {
        throw ErrorExp( true, result, NON_LIST ) ;
      } // if()
      else if ( currentNode->mtype != NIL ) {
        TokenPtr currentleft = currentNode->mleftnode ;
        /*
        if ( currentleft != NULL )
          cout << " ***currentleft->mtoken " << currentleft->mtoken <<" "<< i<<endl;
        cout<<"currentNode->mtype "<< currentNode->mtoken<<endl ;
        */
        
        if ( !IsProcFn( currentleft ) && currentleft->mtype == SYMBOL ) {
          if ( i > 0 ) {
            // cout <<"/define para"<< currentleft->mtoken <<endl;
            usrdefine->muserfunc->marg.push_back( currentleft->mtoken ) ;
          } // if()
          else
            usrdefine->muserfunc->mdefname = currentleft->mtoken ;
        } // if()
        else
          throw ErrorExp( true, token, DEFERR ) ;
      } // else if()
    } // for()

    for ( int i = 1 ; i < argList.size() ; ++i ) {
      // cout <<"/define muserfunc"<< argList.at( i )->mleftnode->mtoken <<endl;

      usrdefine->muserfunc->mfunction.push_back( argList.at( i ) ) ;
    } // for()

    stringstream str ;
    str << "#<procedure " << usrdefine->muserfunc->mdefname << ">" ;

    // cout <<"/define name"<< str.str() <<endl;

    usrdefine->SetttToken( str.str(), USDEFINE ) ;

    gCmdMap[ usrdefine->muserfunc->mdefname ] = usrdefine ;

    stringstream st ;
    st << usrdefine->muserfunc->mdefname << " defined" ;
    if ( gparser.mverb == true )
      cout << st.str() << endl ;

    return NULL ;
  } // if() : first token is (
  else {
    Token * resultsexp = NULL ;
    resultsexp = EvalExp( argList.at( 1 ), level + 1, paraMap ) ;

    if ( resultsexp->mtype == USDEFINE && resultsexp->mtoken == "#<procedure lambda>" )
      resultsexp->muserfunc->mdefname = "lambda" ;

    // cout << result->mtoken <<endl;
    gCmdMap[ result->mtoken ] = resultsexp ;

    string str = result->mtoken + " defined" ;
    if ( gparser.mverb == true )
      cout << str << endl ;

    return NULL ;
  } // else()


  return result ;
} // IsDefine()

Token * IsCar( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype != LEFT_PAREN ) {
    stringstream str ;
    // cout <<" throw WRONGTYPE car: "<< gFunc_msg[ 4 ] << endl;
    throw ErrorExp( true, result, WRONGTYPE, 4 ) ;
  } // if()

  // cout<< " ***fuck car " << result->mleftnode->mtoken << endl;
  return result->mleftnode ;
} // IsCar()

Token * IsCdr( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype != LEFT_PAREN ) {
    // cout <<" throw WRONGTYPE cdr: "<< gFunc_msg[ 5 ] << endl;
    throw ErrorExp( true, result, WRONGTYPE, 5 ) ;
  } // if()

  if ( result->mrightnode != NULL ) {
    if ( result->mrightnode->mtoken == ".(" )
      result->mrightnode->SetttToken( "(", LEFT_PAREN ) ;
  } // else()
  else
    return new Token( "nil", NIL );
    
  return result->mrightnode;
} // IsCdr()

Token * IsAtom( int level, vector<TokenPtr> & argList, map<string,TokenPtr> * paraMap ) {

  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype == LEFT_PAREN )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T ) ;
} // IsAtom()

Token * IsPair( int level, vector<TokenPtr> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype == LEFT_PAREN )
    return new Token( "#t", T ) ;
  else {
    return new Token( "nil", NIL ) ;
  } // else

} // IsPair()

Token * IsList( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype != LEFT_PAREN ) {
    if ( result->mtype == NIL )
      return new Token( "#t", T  ) ;
    else
      return new Token( "nil", NIL ) ;
  } // if()
  else {
    for ( Token* currentNode = result ; currentNode != NULL ; currentNode = currentNode->mrightnode ) {
      if ( currentNode->mrightnode == NULL ) {

        if ( currentNode->mtype != NIL  )
          return new Token( "nil", NIL ) ;
      } // if()
    } // for()

  } // else()

  return new Token( "#t", T  ) ;
} // IsList()

Token * IsNull( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level+ 1, paraMap ) ;

  if ( result->mtype != NIL )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T  ) ;

} // IsNull()

Token * IsInt( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype != INT )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T  ) ;


} // IsInt()

Token * IsReal( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;


  if ( result->mtype != FLOAT && result->mtype != INT )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T  ) ;


} // IsReal()

Token * IsNum( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;


  if ( result->mtype != FLOAT && result->mtype != INT )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T  ) ;

} // IsNum()

Token * IsStr( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;


  if ( result->mtype != STRING )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T  ) ;


} // IsStr()

Token * IsBool( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;


  if ( result->mtype != NIL && result->mtype != T )
    return new Token( "nil", NIL ) ;
  else
    return new Token( "#t", T ) ;


} // IsBool()

Token * IsSym( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;


  if ( IsProcFn( result ) )
    return new Token( "#t", T  ) ;
  else {
    if ( result->mtype != SYMBOL )
      return new Token( "nil", NIL );
    else
      return new Token( "#t", T  ) ;
  } // else ( IsCallable( pairRoot ) && !pairRoot->mIsQuoted )

  return result;
} // IsSym()


Token * IsAdd( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  bool isFloat = false ;
  float resultNum = 0.0;

  for ( int i = 0 ; i < argList.size() ; i++ ) {

    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype != INT && result->mtype != FLOAT ) {
      // cout <<" throw WRONGTYPE add: "<< gFunc_msg[ 16 ] << endl;

      throw ErrorExp( true, result, WRONGTYPE, 16 ) ;
    } // if
    else {
      if ( result->mtype == FLOAT && !isFloat )
        isFloat = true;

      resultNum += round( atof( result->mtoken.c_str() ) * 1000 ) / 1000;

      // cout <<" adding** "<< resultNum <<endl;
    } // else
  } // for

  stringstream str ;
  if ( isFloat ) {
    str << fixed << setprecision( 3 ) << resultNum ;
    return new Token( str.str(), FLOAT ) ;
  } // if
  else {
    str << round( resultNum ) ;
    return new Token( str.str(), INT ) ;
  } // else

} // IsAdd()

Token * IsSub( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  bool isFloat = false ;
  float resultNum = 0.0;

  for ( int i = 0 ; i < argList.size() ; i++ ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype != INT && result->mtype != FLOAT ) {
      // cout <<" throw WRONGTYPE sub: "<< gFunc_msg[ 17 ] << endl;

      throw ErrorExp( true, result, WRONGTYPE, 17 ) ;
    } // if()
    else {
      if ( result->mtype == FLOAT && !isFloat )
        isFloat = true;

      if ( i == 0 ) {
        resultNum = round( atof( result->mtoken.c_str() ) * 1000 ) / 1000;
      } // if()
      else {
        resultNum -= round( atof( result->mtoken.c_str() ) * 1000 ) / 1000;
      } // else()
    } // else
  } // for

  stringstream str ;
  if ( isFloat ) {
    str << fixed << setprecision( 3 ) << resultNum ;
    return new Token( str.str(), FLOAT ) ;
  } // if
  else {
    str << round( resultNum ) ;
    return new Token( str.str(), INT ) ;
  } // else

} // IsSub()

Token * IsMult( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  bool isFloat = false ;
  float resultNum = 1.0;

  for ( int i = 0 ; i < argList.size() ; i++ ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype != INT && result->mtype != FLOAT ) {
      // cout <<" throw WRONGTYPE mult: "<< gFunc_msg[ 18 ] << endl;

      throw ErrorExp( true, result, WRONGTYPE, 18 ) ;
    } // if()
    else {
      if ( result->mtype == FLOAT && !isFloat )
        isFloat = true;

      resultNum *= round( atof( result->mtoken.c_str() ) * 1000 ) / 1000;

    } // else
  } // for

  stringstream str ;
  if ( isFloat ) {
    str << fixed << setprecision( 3 ) << resultNum ;
    return new Token( str.str(), FLOAT ) ;
  } // if
  else {
    str << round( resultNum ) ;
    return new Token( str.str(), INT ) ;
  } // else

} // IsMult()

Token * IsDiv( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  bool isFloat = false ;
  float resultNum = 0.0;
  float num = 0.0;

  for ( int i = 0 ; i < argList.size() ; i++ ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype != INT && result->mtype != FLOAT ) {
      // cout <<" throw WRONGTYPE div: "<< gFunc_msg[ 19 ] << endl;

      throw ErrorExp( true, result, WRONGTYPE, 19 ) ;
    } // if()
    else {
      if ( result->mtype == FLOAT && !isFloat )
        isFloat = true;

      if ( i == 0 ) {
        resultNum = round( atof( result->mtoken.c_str() ) * 1000 ) / 1000;
      } // if()
      else {
        num = round( atof( result->mtoken.c_str() ) * 1000 ) / 1000;
        if ( num == 0.0 ) {
          throw ErrorExp( true, result, DIVZRO ) ;
        } // if()


        resultNum /= num ;
        // cout<<"diving*** "<< resultNum<<endl;
      } // else()
    } // else
  } // for

  stringstream str ;
  if ( isFloat ) {
    str << fixed << setprecision( 3 ) << resultNum ;
    return new Token( str.str(), FLOAT ) ;
  } // if
  else {
    str << ( int ) resultNum ;
    return new Token( str.str(), INT ) ;
  } // else
} // IsDiv()

Token* IsAnd( TokenPtr token, vector<Token*> & argList, int level, map<string,TokenPtr> * paraMap ) {
  TokenPtr result = NULL;
  level = level + 1 ;

  for ( int i = 0 ; i < argList.size() ; ++i ) {
    // cout << "  IsAnd: "<< i<<" is " << argList.at( i )->mleftnode->mtoken << endl;
    result = EvalExp( argList.at( i ), level, paraMap ) ;
    if ( result->mtype == NIL )
      return result;
  } // for()



  return result;
} // IsAnd()

Token* IsNot( TokenPtr token, vector<Token*> & argList, int level, map<string,TokenPtr> * paraMap ) {

  TokenPtr result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;

  if ( result->mtype == NIL )
    return new Token( "#t", T ) ;
  else
    return new Token( "nil", NIL ) ;


} // IsNot()

Token * IsOr( TokenPtr token, vector<Token*> & argList, int level, map<string,TokenPtr> * paraMap ) {

  TokenPtr result = NULL;
  level = level + 1 ;

  for ( int i = 0 ; i < argList.size() ; i++ ) {
    result = EvalExp( argList.at( i ), level, paraMap ) ;

    if ( result->mtype != NIL )
      return result ;
  } // for

  return result;

} // IsOr()


Token * Big( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  float cmp1 = 0.0, cmp2 = 0.0 ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {

    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;
    if ( result->mtype != INT && result->mtype != FLOAT ) {
      throw ErrorExp( true, result, WRONGTYPE, 23 ) ;
    } // if()
    else {

      if ( i == 0 ) {
        cmp1 = atof( result->mtoken.c_str() ) ;
      } // if()
      else {
        cmp2 = atof( result->mtoken.c_str() ) ;
        if ( cmp1 <= cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()
    } // else
  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );

} // Big()

Token * Bigeq( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  float cmp1 = 0.0, cmp2 = 0.0 ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {

    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;
    if ( result->mtype != INT && result->mtype != FLOAT ) {

      throw ErrorExp( true, result, WRONGTYPE, 24 ) ;

    } // if()
    else {

      if ( i == 0 ) {
        cmp1 = atof( result->mtoken.c_str() ) ;
      } // if()
      else {
        cmp2 = atof( result->mtoken.c_str() ) ;
        if ( cmp1 < cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()
    } // else
  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );

} // Bigeq()

Token * Sml( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  float cmp1 = 0.0, cmp2 = 0.0 ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {

    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;
    if ( result->mtype != INT && result->mtype != FLOAT ) {

      throw ErrorExp( true, result, WRONGTYPE, 25 ) ;

    } // if()
    else {

      if ( i == 0 ) {
        cmp1 = atof( result->mtoken.c_str() ) ;
      } // if()
      else {
        cmp2 = atof( result->mtoken.c_str() ) ;
        if ( cmp1 >= cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()
    } // else
  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );


} // Sml()

Token * Smleq( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  float cmp1 = 0.0, cmp2 = 0.0 ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {

    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;
    if ( result->mtype != INT && result->mtype != FLOAT ) {

      throw ErrorExp( true, result, WRONGTYPE, 26 ) ;

    } // if()
    else {

      if ( i == 0 ) {
        cmp1 = atof( result->mtoken.c_str() ) ;
      } // if()
      else {
        cmp2 = atof( result->mtoken.c_str() ) ;
        if ( cmp1 > cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()
    } // else
  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );


} // Smleq()

Token * Equu( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  float cmp1 = 0.0, cmp2 = 0.0 ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {

    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;
    if ( result->mtype != INT && result->mtype != FLOAT ) {

      throw ErrorExp( true, result, WRONGTYPE, 27 ) ;

    } // if()
    else {

      if ( i == 0 ) {
        cmp1 = atof( result->mtoken.c_str() ) ;
      } // if()
      else {
        cmp2 = atof( result->mtoken.c_str() ) ;
        if ( cmp1 != cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()
    } // else
  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );


} // Equu()

Token * Strapp( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  string restr = "";
  stringstream str ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype == STRING ) {
      result->mtoken = result->mtoken.substr( 1, result->mtoken.size() - 2 ) ;
      restr += result->mtoken ;

    } // if()
    else {
      // cout <<" throw WRONGTYPE strapp: "<< gFunc_msg[ 28 ] << endl;

      throw ErrorExp( true, result, WRONGTYPE, 28 ) ;

    } // else()
  } // for()

  str << "\"" << restr << "\"" ;

  return new Token( str.str(), STRING ) ;
} // Strapp()

Token * Strbig( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  string cmp1 = "\0", cmp2 = "\0" ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype == STRING ) {
      if ( i == 0 ) {
        cmp1 = result->mtoken.c_str() ;
      } // if()
      else {
        cmp2 = result->mtoken.c_str() ;
        if ( cmp1 <= cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()

    } // if
    else {

      throw ErrorExp( true, result, WRONGTYPE, 29 ) ;
    } // else

  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );



} // Strbig()

Token * Strsml( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  string cmp1 = "\0", cmp2 = "\0" ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype == STRING ) {
      if ( i == 0 ) {
        cmp1 = result->mtoken.c_str() ;
      } // if()
      else {
        cmp2 = result->mtoken.c_str() ;
        if ( cmp1 >= cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()

    } // if
    else {
      throw ErrorExp( true, result, WRONGTYPE, 30 ) ;
    } // else

  } // for

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );


} // Strsml()

Token * Streql( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  string cmp1 = "\0", cmp2 = "\0" ;
  bool cons = false ;
  for ( int i = 0 ; i < argList.size() ; ++ i ) {
    Token * result = EvalExp( argList.at( i ), level + 1, paraMap ) ;

    if ( result->mtype == STRING ) {
      if ( i == 0 ) {
        cmp1 = result->mtoken.c_str() ;
      } // if()
      else {
        cmp2 = result->mtoken.c_str() ;
        if ( cmp1 != cmp2 )
          cons  = true ;
        cmp1 = cmp2 ;
      } // else()

    } // if()
    else {

      throw ErrorExp( true, result, WRONGTYPE, 31 ) ;
    } // else()

  } // for()

  if ( cons )
    return new Token( "nil", NIL );
  else
    return new Token( "#t", T );


} // Streql()

bool AtomJudge( int type ) {
  if ( type == SYMBOL || type == INT ||
       type == FLOAT || type == STRING ||
       type == NIL || type == T )
    return true;

  else return false;
} // AtomJudge()

Token * IsEqv( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  TokenPtr cmp[2] ;

  for ( int i = 0 ; i < 2 ; i++ ) {
    cmp[i] = EvalExp( argList.at( i ), level + 1, paraMap ) ;
  } // for()

  if ( cmp[0] == cmp[1] ) {
    return new Token( "#t", T );
  } // if()
  else {
    if ( AtomJudge( cmp[0]->mtype ) && AtomJudge( cmp[1]->mtype ) &&
         cmp[0]->mtype != STRING && cmp[1]->mtype != STRING &&
         cmp[0]->mtoken == cmp[1]->mtoken ) {
      return new Token( "#t", T );
    } // if : same token ,not quote string

    else  {
      return new Token( "nil", NIL );
    } // else()
  } // else() : node compare

} // IsEqv()

int IdenticalTrees( TokenPtr a, TokenPtr b ) {

  if ( a == NULL && b == NULL )
    return 1;

  if ( a != NULL && b != NULL ) {
    return ( a->mtoken == b->mtoken
             && IdenticalTrees( a->mleftnode, b->mleftnode )
             && IdenticalTrees( a->mrightnode, b->mrightnode ) ) ;
  } // if()

  return 0;
} // IdenticalTrees()

Token * IsEql( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  TokenPtr cmp[2] ;

  for ( int i = 0 ; i < 2 ; i++ ) {
    cmp[i] = EvalExp( argList.at( i ), level + 1, paraMap ) ;
  } // for()

  if ( IdenticalTrees( cmp[0], cmp[1] ) )
    return new Token( "#t", T );
  else
    return new Token( "nil", NIL );

} // IsEql()

Token * IsBegin( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  Token * result = NULL;
  for ( int i = 0 ; i < argList.size() ; i++ ) {
    result = EvalExp( argList.at( i ), level + 1, paraMap ) ;
  } // for

  return result ;
} // IsBegin()


Token * IsIf( TokenPtr troot, int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  Token * result = EvalExp( argList.at( 0 ), level + 1, paraMap ) ;
  if ( result->mtype != NIL )
    return EvalExp( argList.at( 1 ), level + 1, paraMap ) ;
  else {
    if ( argList.size() < 3 ) {
      // cout << " //////////noret: " <<troot->mleftnode->mtoken <<endl;
      throw ErrorExp( true, troot, NORET ) ;
    } // if()
    else
      return EvalExp( argList.at( 2 ), level + 1, paraMap ) ;
  } // else()

} // IsIf()

Token * IsCond( TokenPtr troot, int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  for ( int i = 0 ; i < argList.size() ; ++i ) {

    Token * result = argList.at( i );
    if ( result->mtype == SYMBOL && FindDefine( result ) ) {
      result = EvalExp( result, level + 1, paraMap ) ;
    } // if()
    
    if ( result->mtype != LEFT_PAREN ) {
      throw ErrorExp( true, result, CNDERR ) ;
    } // if()
    else {
      try {
        vector<Token*> temparg ;
        Getargsize( result, temparg ) ;
        if ( temparg.size() == 0 ) {
          throw ErrorExp( true, result, CNDERR ) ;
        } // if()
      } catch ( ErrorExp e ) {
        if ( e.merrortype == NON_LIST ) {
          throw ErrorExp( true, result, CNDERR ) ;
        } // if()
        else
          throw;
      } // catch()
    } // else()
  } // for() check aralist type

  Token * result_result = NULL ;

  for ( int i = 0 ; i < argList.size() ; i++ ) {
    TokenPtr cons = argList.at( i ) ;
    TokenPtr cons_next = cons->mrightnode->mleftnode ;

    Token * result = NULL ;
    if ( i == argList.size() - 1 && cons->mleftnode->mtoken == "else" )
      result = new Token( "#t", T ) ;
    else {
      result = EvalExp( cons->mleftnode, level + 1, paraMap ) ;
      // cout << result->mtoken ;
    } // else()
    
    if ( cons_next != NULL ) {
      vector<Token*> temparg ;
      Getargsize( cons, temparg ) ;

      if ( result->mtype != NIL && result_result == NULL ) {
        for ( int k = 0 ; k < temparg.size() ; ++k ) {
          try {
            result_result = EvalExp( temparg.at( k ), level + 1, paraMap ) ;
          } catch ( ErrorExp e  ) {
            if ( k == ( temparg.size() - 1 ) && e.merrortype == NORET ) {
              throw ErrorExp( true, result_result, NORET ) ;
            } // if
            else {
              if ( e.merrortype != NORET )
                throw ;
            } // else
          } // catch()
        } // for

        return result_result ;
      } // if
    } // if
    else {
      throw ErrorExp( true, result, CNDERR ) ;
    } // else
  } // for

  if ( result_result == NULL ) {
    // cout << " //////////////noret: " <<troot->mleftnode->mtoken <<endl;
    throw ErrorExp( true, troot, NORET ) ;
  } // if

  return NULL ;

} // IsCond()

void GetPair( Token * token, vector<Token*> & argList ) {
  int num = 0 ;
  TokenPtr currentNode = token ;
  while (  currentNode != NULL  ) {
    if ( currentNode->mrightnode == NULL 
         && currentNode->mtype != NIL && currentNode->mtype != RIGHT_PAREN ) {
      throw ErrorExp( true, token, NON_LIST ) ;
    } // if
    else if ( currentNode->mtype != NIL && currentNode->mleftnode != NULL ) {
      argList.push_back( currentNode->mleftnode ) ;
    } // else if

    currentNode = currentNode->mrightnode ;
  } // while()
} // GetPair()


Token * IsLet( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  // ( let ( (x 3) (y '(1 2 3)))
  //      (cons 1 '(4 5))
  //      (cons x (cdr y))      ; the value of this one is the value of LET
  // )

  TokenPtr firstarg = argList.at( 0 ) ; // first arg ( (x 3) (y '(1 2 3)))
  map<string,TokenPtr> * tempLocal = new map<string,TokenPtr>() ;
  map<string,TokenPtr> & tempLocalRef = *tempLocal ;

  if ( paraMap != NULL ) {
    for ( map<string,TokenPtr>::iterator it = paraMap->begin() ;
          it != paraMap->end() ; ++ it ) {
      tempLocalRef[ it->first ] = it->second ;
    } // for
  } // if
    
  if ( firstarg->mtype != LEFT_PAREN && firstarg->mtype != NIL ) {
    throw ErrorExp( true, firstarg, LETERR ) ;
  } // if

  else {
    vector<TokenPtr> localdef ;
    try {
      GetPair( firstarg, localdef ) ; // (x 3) && (y '(1 2 3))
    } catch ( ErrorExp e ) {
      if ( e.merrortype == NON_LIST ) {

        throw ErrorExp( true, firstarg, LETERR ) ;
      } // if()
      else
        throw ;

    } // catch()

    // check local symbols format
    for ( int i = 0 ; i < localdef.size() ; i++ ) { // (x 3)
      if ( localdef.at( i )->mtype == LEFT_PAREN ) {

        vector<TokenPtr> pairlist ;
        try {
          GetPair( localdef.at( i ), pairlist ) ; // x && 3
        } catch ( ErrorExp e ) {
          if ( e.merrortype == NON_LIST ) {

            throw ErrorExp( true, firstarg, LETERR ) ;
          } // if()
          else
            throw;
        } // catch()

        if ( pairlist.size() != 2 ) {

          throw ErrorExp( true, firstarg, LETERR ) ;
        } // if() check format
        else {
          if ( pairlist.at( 0 )->mtype != SYMBOL || IsProcFn( pairlist.at( 0 ) ) ) {

            throw ErrorExp( true, firstarg, LETERR ) ;
          } // if() check format
          else {
            TokenPtr evalval = EvalExp( pairlist.at( 1 ), level + 1, paraMap ) ;
            tempLocalRef[ pairlist.at( 0 )->mtoken ] = evalval ;
          } // else()
        } // else
      } // if
      else {

        throw ErrorExp( true, firstarg, LETERR ) ;
      } // else
    } // for

    // eval
    Token * result = NULL ;
    for ( int i = 1 ; i < argList.size() ; i++ ) {
      if ( argList.at( i )->mtype == LEFT_PAREN )
        result = EvalExp( argList.at( i ), level + 1, tempLocal ) ;
      else
        result = EvalExp( argList.at( i ), level, tempLocal ) ;
    } // for

    gparaMap[gmaplayer].clear();
    --gmaplayer;

    return result ;
  } // else

} // IsLet()

Token * IsLambda( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {

  TokenPtr firstarg = argList.at( 0 ) ;
  if ( firstarg->mtype != LEFT_PAREN && firstarg->mtype != NIL ) {
    throw ErrorExp( true, firstarg, LAMBERR ) ;
  } // if

  TokenPtr lamFunc = new Token( "#<procedure lambda>", USDEFINE ) ;
  lamFunc->muserfunc = new DefineFuncton() ;
  lamFunc->muserfunc->mdefname = "lambda" ;
  TokenPtr currentNode = firstarg ;

  for ( int i = 0; currentNode != NULL ; currentNode = currentNode->mrightnode, ++i ) {
    if ( currentNode->mrightnode == NULL 
         && currentNode->mtype != NIL && currentNode->mtype != RIGHT_PAREN ) {
      throw ErrorExp( true, firstarg, LAMBERR ) ;
    } // if
    else if ( currentNode->mtype != NIL ) {
      Token* currentleft = currentNode->mleftnode ;
      if ( !IsProcFn( currentleft ) && currentleft->mtype == SYMBOL ) {
        lamFunc->muserfunc->marg.push_back( currentleft->mtoken ) ;
      } // if()
      else
        throw ErrorExp( true, firstarg, LAMBERR ) ;
    } // else if()
  } // for()

  for ( int i = 1 ; i < argList.size() ; ++ i ) {
    lamFunc->muserfunc->mfunction.push_back( argList.at( i ) ) ;
  } // for

  return lamFunc ;

} // IsLambda()

Token * IsExit( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  if ( level != 0 )
    throw ErrorExp( true, NULL, EXITLVERR ) ;
  else if ( !argList.empty() )
    throw ErrorExp( true, NULL, WRONGARGNUM ) ;

  gparser.mexit = true;

  return NULL ;
} // IsExit()

Token * IsClean( int level, vector<Token*> & argList, map<string,TokenPtr> * paraMap ) {
  if ( level != 0 )
    throw ErrorExp( true, NULL, CLEANLVERR ) ;
  else if ( !argList.empty() )
    throw ErrorExp( true, NULL, WRONGARGNUM ) ;

  gCmdMap.clear() ;
  Map_InternalCmd() ;
  if ( gparser.mverb == true )
    cout << "environment cleaned" << endl ;

  return NULL ;

} // IsClean()


Token * UserdefEva( Token* token, vector<TokenPtr> & argList, int level, map<string,TokenPtr> * paraMap ) {
  // (g 3 5)
  // origin: (define (g d y) (+ (* d y) d))

  DefineFuncton* thisfunct = token->muserfunc ;
  ++gmaplayer;
  // reset parameter d= 3, y =5 to gparaMap
  // 2 time to prevent para behind from getting previos para value
  

  for ( int i = 0 ; i < thisfunct->marg.size() ; ++i ) {
    try {
      argList.at( i ) = EvalExp( argList.at( i ), level + 1, paraMap ) ;
      // cout << "....para eval success : " << thisfunct->marg.at( i ) << endl ;

    } catch ( ErrorExp err ) {
      if ( err.merrortype == NORET ) {
        throw ErrorExp( true, argList.at( i ), UNBONDPARA ) ;
      } // if
      else
        throw ;
    } // catch()
  } // for() //firsttime eval
  
  map<string,TokenPtr> * currentMap = new map<string,TokenPtr>() ;
  map<string,TokenPtr> & currentMapitr = *currentMap ;
  
  for ( int i = 0 ; i < thisfunct->marg.size() ; i++ ) {
    currentMapitr[ thisfunct->marg.at( i ) ] = argList.at( i ) ;
    // cout << "....setting para : " << thisfunct->marg.at( i ) <<" "<<  argList.at( i )->mtoken ;
    // if( argList.at( i )->mleftnode!= NULL)
    // cout<<"  left: "<< argList.at( i )->mleftnode->mtoken <<endl;
  } // for() //sectime reset
  
  // eval function
  TokenPtr result = NULL ;
  // cout<<" thisfunct->mfunction.size(): "<< thisfunct->mfunction.size() <<endl;

  for ( int i = 0 ; i < thisfunct->mfunction.size() ; i ++ ) {
    result = EvalExp( thisfunct->mfunction.at( i ), level, currentMap ) ;
  } // for

  gparaMap[gmaplayer].clear();
  --gmaplayer;
  // cout<<" gparaMapsize(): "<< gparaMap.size() <<endl;

  return result ;
} // UserdefEva()

Token * Usrfunc( TokenPtr token, vector<TokenPtr> & argList, int level, map<string,TokenPtr> * paraMap ) {
  
  // cout<<"88888888888 caution ! eval userfunc !!!: " << token->mtoken <<endl;
  TokenPtr result = UserdefEva( token, argList, level, paraMap ) ;

  return result ;
} // Usrfunc()

Token * FindCorrespondFunction( TokenPtr troot, Token* currentNode, vector<Token*> & argList,
                                int level, map<string,TokenPtr> * paraMap ) {
  if ( currentNode->mtype != LIST )
    CheckParaSize( currentNode, level, argList ) ;
    
  if ( currentNode->mtype == CONS )
    return IsCons( currentNode, level, argList, paraMap ) ;
  else if ( currentNode->mtype == LIST )
    return List( currentNode, level, argList, paraMap ) ;
  else if ( currentNode->mtype == QUOTE ) {
    return IsQuote( troot, level, argList, paraMap ) ;
  } // else if
  else if ( currentNode->mtype == DEFINE )
    return IsDefine( currentNode, level, argList, paraMap ) ;
  else if ( currentNode->mtype == CAR )
    return IsCar( level, argList, paraMap ) ;
  else if ( currentNode->mtype == CDR )
    return IsCdr( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISATOM )
    return IsAtom( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISPAIR )
    return IsPair( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISLIST )
    return IsList( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISNULL )
    return IsNull( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISINT )
    return IsInt( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISREAL )
    return IsReal( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISNUM )
    return IsNum( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISSTR )
    return IsStr( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISBOOL )
    return IsBool( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ISSYM )
    return IsSym( level, argList, paraMap ) ;
  else if ( currentNode->mtype == ADD )
    return IsAdd( level, argList, paraMap ) ;
  else if ( currentNode->mtype == MINU )
    return IsSub( level, argList, paraMap ) ;
  else if ( currentNode->mtype == MUL )
    return IsMult( level, argList, paraMap ) ;
  else if ( currentNode->mtype == DIV )
    return IsDiv( level, argList, paraMap ) ;
  else if ( currentNode->mtype == AND )
    return IsAnd( currentNode, argList, level, paraMap ) ;
  else if ( currentNode->mtype == NOTT )
    return IsNot( currentNode, argList, level, paraMap ) ;
  else if ( currentNode->mtype == OR )
    return IsOr( currentNode, argList, level, paraMap ) ;
  else if ( currentNode->mtype == BIGGER )
    return Big( level, argList, paraMap ) ;
  else if ( currentNode->mtype == BIG_EQ )
    return Bigeq( level, argList, paraMap ) ;
  else if ( currentNode->mtype == SMAL )
    return Sml( level, argList, paraMap ) ;
  else if ( currentNode->mtype == SMAL_EQ )
    return Smleq( level, argList, paraMap ) ;
  else if ( currentNode->mtype == EQUU )
    return Equu( level, argList, paraMap ) ;
  else if ( currentNode->mtype == STRING_APP )
    return Strapp( level, argList, paraMap ) ;
  else if ( currentNode->mtype == STRING_BIG )
    return Strbig( level, argList, paraMap ) ;
  else if ( currentNode->mtype == STRING_SML )
    return Strsml( level, argList, paraMap ) ;
  else if ( currentNode->mtype == STRING_EQU )
    return Streql( level, argList, paraMap ) ;
  else if ( currentNode->mtype == EQV )
    return IsEqv( level, argList, paraMap ) ;
  else if ( currentNode->mtype == EQU )
    return IsEql( level, argList, paraMap ) ;
  else if ( currentNode->mtype == BEGIN )
    return IsBegin( level, argList, paraMap ) ;
  else if ( currentNode->mtype == IF )
    return IsIf( troot, level, argList, paraMap ) ;
  else if ( currentNode->mtype == COND )
    return IsCond( troot, level, argList, paraMap ) ;
  else if ( currentNode->mtype == LET )
    return IsLet( level, argList, paraMap ) ;
  else if ( currentNode->mtype == LAMBDA )
    return IsLambda( level, argList, paraMap ) ;
  else if ( currentNode->mtype == USDEFINE )
    return Usrfunc( currentNode, argList, level, paraMap ) ;
  else if ( currentNode->mtype == EXIT )
    IsExit( level, argList, paraMap ) ;
  else if ( currentNode->mtype == CLEAN )
    IsClean( level, argList, paraMap ) ;
  else if ( currentNode->mtype == PRINT ) {
    stringstream ss ;
    // PrintSExp( root, 1, true, true, ss ) ;
    for ( int i = 0 ; i < argList.size() ; ++ i )
      ss << argList.at( i )->mtoken << " is " <<
      EvalExp( currentNode, level, paraMap )->mtoken << endl ;

    cout << "{" << ss.str() << "}" << endl ;
    return NULL ;
  } // else if

  return NULL ;
} // FindCorrespondFunction()


Token * EvalExp( TokenPtr troot, int level, map<string,TokenPtr> * paraMap ) {

  if ( ( troot->mtype == SYMBOL || troot->mtype == QUOTE ) ) {
    if ( paraMap != NULL && paraMap->find( troot->mtoken ) != paraMap->end() ) {
      map<string,TokenPtr> & paraMapRef = *paraMap ;
      // cout << "localDefMapRef "<< troot->mtoken<<endl;
      // if ( paraMapRef[ troot->mtoken ] != NULL )
      //   cout<<" return value "<< paraMapRef[ troot->mtoken ]->mtoken <<endl;
      // if ( paraMapRef[ troot->mtoken ]->mleftnode != NULL )
      // cout<<" return value LEFT"<< paraMapRef[ troot->mtoken ]->mleftnode->mtoken <<endl;

      return paraMapRef[ troot->mtoken ] ;
    } // if
    else if ( FindDefine( troot ) ) { // userdefine
      // cout << "FindDefine : " <<  troot->mtoken <<endl;
      // cout<< "retuen value: "<<gCmdMap[ troot->mtoken ]->mtoken <<endl;
      // if ( gCmdMap[ troot->mtoken ]->mleftnode != NULL )
      //  cout<<" return value LEFT"<< gCmdMap[ troot->mtoken ]->mleftnode->mtoken <<endl;

      return gCmdMap[ troot->mtoken ] ;
    } // else if()
    else if ( IsProcFn( troot ) ) { // internal cmd
      // cout << "IsProcFn : " <<  troot->mtoken <<endl;

      return troot ;
    } // else if
    else
      throw ErrorExp( true, troot, UNBOND ) ;
  } // if
  else if ( troot->mtype == LEFT_PAREN ) {
    if ( troot->mtoken == "(" ) {

      vector<TokenPtr> curargList ;
      Getargsize( troot, curargList ) ;
      // cout << "////  argList.size: " << curargList.size() << endl;

      Token* currentNode = NULL ;
      Token* curtroot = troot->mleftnode ;
      // cout << "////  check left: "<< troot->mtoken<<" "<< curtroot->mtoken << endl;

      if ( curtroot->mtype == LEFT_PAREN  && curtroot->mtoken == "(" ) {
        // cout << "////  next level: "<< endl;
        currentNode = EvalExp( curtroot, level + 1, paraMap ) ;
      } // if()
      else
        currentNode = EvalExp( curtroot, level, paraMap ) ;

      // cout << " ****** currentnode " << currentNode->mtoken <<endl;

      if ( IsProcFn( currentNode ) ) {
        // cout << "////  IsProcFn( currentNode ) " << currentNode->mtoken << endl;

        return FindCorrespondFunction( troot, currentNode, curargList, level, paraMap ) ;
      } // if()
      else {
        // non_sense here
        if ( !currentNode->mfromquote && currentNode->mtype == SYMBOL ) {
          throw ErrorExp( true, currentNode, UNBOND ) ;
        } // if

        if ( currentNode != NULL )
          throw ErrorExp( true, currentNode, NONFUNC ) ;

      } // else

    } // if()
  } // else if()

  return troot ;
} // EvalExp()

void PrtExp( TokenPtr troot, int space, bool left_par, bool right ) {
  if ( troot != NULL && troot->mleftnode != NULL ) {
    if ( troot->mtype == LEFT_PAREN && troot->mtoken == "(" ) {
      if ( !left_par ) {
        for ( int i = 0 ; i < space ; i++ )
          cout << " " ;
      } // if()

      cout << "( " ;
      PrtExp( troot->mleftnode, space + 2, true, false ) ;
      PrtExp( troot->mrightnode, space + 2, false, true ) ;
      for ( int i = 0 ; i < space ; i++ )
        cout << " " ;
      cout << ")" << endl ;
    } // if()
    else if ( strcmp( troot->mtoken.c_str(), ".(" ) == 0 ) {
      PrtExp( troot->mleftnode, space, false, false ) ;
      PrtExp( troot->mrightnode, space, false, true ) ;
    } // else if()
    else if ( troot->mtype == LEFT_PAREN ) { // leftpara with error mesg
      cout << troot->mtoken << " ";
      PrtExp( troot->mleftnode, space + 2, true, false ) ;
      PrtExp( troot->mrightnode, space + 2, false, true ) ;
      for ( int i = 0 ; i < space ; i++ )
        cout << " " ;
      cout << ")" << endl ;
    } // else if()
  } // if()
  else {
    if ( troot != NULL ) {
      // cout<< "    **** "<< troot->mtoken << gmsg[troot->mtype]<< right << endl;
      if ( ! left_par ) {
        if ( troot->mtype != RIGHT_PAREN && troot->mtype != NIL ) {
          for ( int i = 0 ; i < space ; i++ )
            cout << " " ;
          if ( right )  {
            cout << "." << endl ;
            for ( int i = 0 ; i < space ; i++ )
              cout << " " ;
          } // if()
        } // if()
        else if ( troot->mtype == NIL && !right )
          for ( int i = 0 ; i < space ; i++ )
            cout << " " ;

      } // if()

      if ( troot->mtype == NIL && !right ) {
        cout << troot->mtoken << endl ;
      } // if()
      else if ( troot->mtype != RIGHT_PAREN && troot->mtype != NIL ) {
        if ( IsProcFn( troot ) )
          cout << gFunc_msg[ troot->mtype - 12 ] << endl;
        else
          cout << troot->mtoken << endl ;
      } // else if()

    } // if()
  } // else()
} // PrtExp()

void PrtExp_Succ( TokenPtr troot, int space, bool left_par, bool right ) {
  if ( troot != NULL && troot->mleftnode != NULL ) {
    if ( troot->mtype == LEFT_PAREN && troot->mtoken == "(" ) {
      if ( !right ) {
        if ( !left_par ) {
          for ( int i = 0 ; i < space ; i++ )
            cout << " " ;
        } // if()

        cout << "( " ;

        PrtExp_Succ( troot->mleftnode, space + 2, true, false ) ;
        PrtExp_Succ( troot->mrightnode, space + 2, false, true ) ;
        for ( int i = 0 ; i < space ; i++ )
          cout << " " ;
        cout << ")" << endl ;
      } // if()
      else {
        PrtExp_Succ( troot->mleftnode, space, false, false ) ;
        PrtExp_Succ( troot->mrightnode, space, false, true ) ;
      } // else()

    } // if()
    else if ( strcmp( troot->mtoken.c_str(), ".(" ) == 0 ) {
      PrtExp_Succ( troot->mleftnode, space, false, false ) ;
      PrtExp_Succ( troot->mrightnode, space, false, true ) ;
    } // else if()

  } // if()
  else {
    if ( troot != NULL ) {
      // cout<< "    **** "<< troot->mtoken << gmsg[troot->mtype]<< right << endl;
      if ( ! left_par ) {
        if ( troot->mtype != RIGHT_PAREN && troot->mtype != NIL ) {
          for ( int i = 0 ; i < space ; i++ )
            cout << " " ;
          if ( right )  {
            cout << "." << endl ;
            for ( int i = 0 ; i < space ; i++ )
              cout << " " ;
          } // if()
        } // if()
        else if ( troot->mtype == NIL && !right )
          for ( int i = 0 ; i < space ; i++ )
            cout << " " ;

      } // if()

      if ( troot->mtype == NIL && !right ) {
        cout << troot->mtoken << endl ;
      } // if()
      else if ( troot->mtype != RIGHT_PAREN && troot->mtype != NIL )
        cout << troot->mtoken << endl ;

    } // if()
  } // else()
} // PrtExp_Succ()


int main() {

  cout << "Welcome to OurScheme!" << endl ;
  // cout << endl << "> " ;
  Token * troot = NULL ;
  ErrorExp trooterr ;
  int first = 0;
  Map_InternalCmd() ;

  /*
    <S-exp> ::= <ATOM>
            | LEFT-PAREN <S-exp> { <S-exp> } [ DOT <S-exp> ] RIGHT-PAREN
            | QUOTE <S-exp>

     <ATOM>  ::= SYMBOL | INT | FLOAT | STRING
            | NIL | T | LEFT-PAREN RIGHT-PAREN
  */

  gTestNum = getchar();
  char temp = getchar();



  while ( !gparser.mexit ) {
    cout << endl << "> " ;
    gparser.Init() ;
    trooterr.Init() ;
    gparser.First_Exp() ;

    gparser.mexp_checker = false;
    troot = GetToken( trooterr ) ;
    gparser.mroot = troot ;

    // cout << "************* "<< troot->mtoken << " + " << gmsg[troot->mtype]<< endl ;
    if ( troot->mtype == LEFT_PAREN || troot->mtype == QUOTE ) {
      // ++gparser.mcol ;
      // cout << " @@@@@@@@ left quote"<< endl ;
      if ( !trooterr.merror ) {
        BuildExp( troot, trooterr, false, false ) ;
      } // if()
      // cout << "-- LEFT_PAREN || quote"<< endl ;
    } // if()
    else if ( ( troot->mtype == DOT || troot->mtype == RIGHT_PAREN ) && !trooterr.merror ) {
      // cout << "********* troot->mtype == DOT || troot->mtype == RIGHT_PAREN" << endl;

      trooterr.SeterrExp( true, troot, EXP_ATOMORLEFTP ) ;
    } // else if()

    if ( !trooterr.merror ) {
      // cout << "-- eval"<< endl ;
      try {
        
        Retype( troot ) ;
        troot = EvalExp( troot, 0, NULL ) ;

        /*
        if ( troot!= NULL)
          cout << " eval success" << troot->mtoken <<" "<<troot->mtype<<endl;
        */

        if ( !gparser.mexit ) {
          PrtExp_Succ( troot, 0, false, false ) ;
        } // if()

      } catch( ErrorExp trooterr ) {
        
        trooterr.PrtError( trooterr ) ;

      } // catch()

      gparser.mexp_checker = true ;
      char temp = getchar() ;
      string str;
      bool isalpha = false ;
      while ( temp != EOF && temp != '\n' && !isalpha ) {
        // cout<< "===== skip "<< endl;
        if ( temp == ';' ) {
          while ( temp != '\n' && temp != EOF ) {
            temp = getchar() ;
          } // while()
        } // if()
        else if ( temp == ' ' ) {
          str += temp ;
          temp = getchar() ;
        } // else if()
        else {
          str += temp ;
          isalpha = true ;
        } // else()

      } // while()

      if ( isalpha ) {
        for ( int j = 0 ; j < str.size() ; ++j ) {
          // cout << "--------"<< str.at( str.size() - 1 - j ) <<endl;
          cin.putback( str.at( str.size() - 1 - j ) ) ;
        } // for()
      } // if ()
      else if ( !gparser.mexit && temp == EOF ) {
        cin.putback( temp ) ;
      } // if()

    } // if()
    else {
      // cout << "-- error"<< endl ;
      trooterr.PrtError( trooterr ) ;
      if ( !gparser.mexit )
        cout << endl;
      char temp = getchar() ;
      while ( temp != EOF && temp != '\n' )
        temp = getchar() ;

      if ( !gparser.mexit && temp == EOF )
        cin.putback( temp ) ;


    } // else()

    gparaMap.clear();
    gmaplayer = -1;

  } // while()

  cout << "\nThanks for using OurScheme!" ;
  return 0;
} // main()
