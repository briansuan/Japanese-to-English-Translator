#include<iostream>
#include<fstream>
#include"scanner.cpp"
#include<vector>
#include<string>
#include<map>
using namespace std;

/*
Authors:
Brian Suan
Nate Flores
Lyle Gonzales
*/

// *** GLOBAL VARIABLES FOR TRANSLATOR *** //
string japanese, english;          // This holds the "dictionary" (japanese = map key, english = map value)  
map<string, string> lexicon;      // This holds the "dictionary" (japanese = map key, english = map value)
ofstream output;
string savedEword;

// *** GLOBAL VARIABLES FOR PARSER *** //
ofstream error_file;        // File to store the errors
string filename;            // For the user filename user input
string saved_lexeme;        //is a sequence of characters in the source program that matches the pattern for a token 
tokentype saved_token;      //global buffer for token the scanner returned
bool token_available;       // global flag indicating whether
char trace;    

// ******************** BEGINING OF PARSER ******************** //
// Need to call these functions in advance so they can be used by other functions before them
tokentype next_token();
bool match(tokentype expected);
void s();
void after_subject();
void after_noun();
void after_object();
void noun();
void verb();
void be();
void tense();
void getEword();
void gen(string type);


// Type of error: ** Syntax Error 1
void syntaxerror1(string saved_lexeme, tokentype expected, char trace_choice ) {

  // To represent the user's skip or replace choice
  char sr;
  // If user chooses 'y' for the trace choice, print out syntax error messages and prompt user to choose if they want to skip or replace a token
  if(trace_choice == 'y') {
    cout << "SYNTAX ERROR 1: expected " << tokenName[expected] << " but found \"" << "\". Halting..." << endl;
    cout << "Skip or replace the token? (s or r): ";
    // Take in user's input
    cin >> sr;

    error_file.open("errors.txt", std::ios_base::app);
    error_file << "Error from file: " << filename << "\n\tSYNTAX ERROR 1: expected " << tokenName[expected] << " but found \"" << saved_lexeme << "\". Halting...\n" << endl;
    error_file.close();

    // If the user chooses to skip ('s') token, call scanner, print out scanner message
    // match the current saved_token, and call the next_token function
    if(sr == 's') {
      scanner(saved_token, saved_lexeme);    // call scanner to grab a new token
      cout << "Scanner called using word: " << saved_lexeme << endl;// saved_token is the token type and
      match(saved_token);
      next_token();
    }
    // Otherwise if the user chooses to replace ('r') the token, set token_available to false and print out Matched message
    else if(sr == 'r') {
      token_available = false;   // eat up the token
      cout << "Matched " << tokenName[expected] << endl;
    }
  }
  else {
    cout << "Skip or replace the token? (s or r): ";
    // Take in user's input
    cin >> sr;

    error_file.open("errors.txt", std::ios_base::app);
    error_file << "Error from file: " << filename << "\n\tSYNTAX ERROR 1: expected " << tokenName[expected] << " but found \"" << saved_lexeme << "\". Halting...\n" << endl;
    error_file.close();

    // If the user chooses to skip ('s') token, call scanner, print out scanner message
    // match the current saved_token, and call the next_token function
    if(sr == 's') {
      scanner(saved_token, saved_lexeme);    // call scanner to grab a new token
      cout << "Scanner called using word: " << saved_lexeme << endl;// saved_token is the token type and
      match(saved_token);
      next_token();
    }
    // Otherwise if the user chooses to replace ('r') the token, set token_available to false and print out Matched message
    else if(sr == 'r') {
      token_available = false;   // eat up the token
      cout << "Matched " << tokenName[expected] << endl;
    }
  }
}
// Type of error: ** Syntax Error 2
void syntaxerror2(string parserF, string saved_lexeme, char trace_choice ) {
  // if user enters y, print out the errors for the user to see 
  if(trace_choice == 'y') {

    cout << "SYNTAX ERROR 2: unexpected \"" << saved_lexeme << "\" found in " << parserF << ". Halting..." << endl; 
    // Print out the errors on the error.txt file 
    error_file.open("errors.txt", std::ios_base::app);
    error_file << "Error from file: " << filename << "\n\tSYNTAX ERROR 2: unexpected \"" << saved_lexeme << "\" found in " << parserF << ". Halting...\n" << endl;
    error_file.close();

    exit(1); 
  }
  // Otherwise, just print out the errors on the txt file without 
  else {
    error_file.open("errors.txt", std::ios_base::app);
    error_file << "Error from file: " << filename << "\n\tSYNTAX ERROR 2: unexpected \"" << saved_lexeme << "\" found in " << parserF << ". Halting...\n" << endl;
    error_file.close();

    exit(1);
  } 
}

// ** Need the updated match and next_token with 2 global vars
// saved_token and saved_lexeme

// Purpose: ** Gets next token 
tokentype next_token() {
  // if there is no saved token yet
  if(saved_lexeme == "eofm") {
    cout << "\nSuccessfully parsed <story>.\n" << endl;
    exit(1);
  }
  if (!token_available) { 
    scanner(saved_token, saved_lexeme);    // call scanner to grab a new token
    cout << "Scanner called using word: " << saved_lexeme << endl;  // saved_token is the token type and 
    // saved_lexeme is the word that is read in
    token_available = true; 
  } // mark that fact that you have saved it
  
  return saved_token;    // return the saved token
}

// Purpose: ** Match
bool match(tokentype expected) {  
  // mismatch has occurred with the next token
  if (next_token() != expected) {
    // calls a syntax error function here to generate a syntax error message here and do recovery
    syntaxerror1(saved_lexeme, expected, trace);
    return false;
  }
  // match has occurred
  else {
    token_available = false;   // eat up the token
    if(trace == 'y') {
      cout << "Matched " << tokenName[expected] << endl;
    }
    return true;                      // say there was a match
  }
}

//<story> ::= <s> { <s> } // stay in the loop as long as a possible start
//of <s> is the next_token (note it can be CONNECTOR or WORD1 or PRONOUN)
void story() {

  cout << "Processing <story>" << endl;
  s();

  while(true) {
    s();
    //if string is eofm
    if(saved_lexeme == "eofm") {
      cout << "Successfully parsed <story>" << endl;
      exit(1); //stops loop
	  }
  }
}
//<s> ::= [CONNECTOR #getEword#  #gen(CONNECTOR)#] <noun> #getEword#  SUBJECT #gen(ACTOR)# <after subject>
void s() {

  cout << "Processing <s>" << endl;
  next_token();

  if(saved_lexeme != "eofm") {
    //      cout << saved_lexeme << endl;
    if(next_token() == CONNECTOR) {
      match(CONNECTOR);
      getEword();
      gen ("CONNECTOR");
    }
    noun();
    match(SUBJECT);
    gen("ACTOR");
    after_subject();
  }    
}

//<after subject> ::= <verb> #getEword#  #gen(ACTION)# <tense> #gen(TENSE)# PERIOD |<noun> #getEword# <after noun>
void after_subject() {

  cout << "Processing <after subject>" << endl;

  switch(next_token()) {
    case WORD2:
      verb();
      tense();
      match(PERIOD);
      break;
    case WORD1: //<noun has two cases word 1 | pronoun
      noun();
      after_noun();
      break;
    case PRONOUN:
      noun();
      after_noun();
      break;
    default:
      syntaxerror2("after_subject", saved_lexeme, trace);
  }  
}

//<after noun> ::= <be> #gen(DESCRIPTION)# #gen(TENSE)# PERIOD |DESTINATION  #gen(TO)# <verb> #getEword#  #gen(ACTION)# <tense> #gen(TENSE)#  PERIOD |OBJECT #gen(OBJECT)# <after object>
void after_noun() {

  cout << "Processing <after noun>" << endl;

  switch(next_token()) {//<be> has is or was
    case IS:
      be();
      match(PERIOD);
      break;
    case WAS:
      be();
      match(PERIOD);
      break;
    case DESTINATION: //case destination
      match(DESTINATION);
      gen("TO");
      verb();
      tense();
      match(PERIOD);
      break;
    case OBJECT: //case object
      match(OBJECT);
      gen("OBJECT");
      after_object();
      break;
    default:
      syntaxerror2("after_noun", saved_lexeme, trace);
  }
}

//<after object> ::= <verb> #getEword#  #gen(ACTION)# <tense> #gen(TENSE)# PERIOD |<noun> #getEword# DESTINATION #gen(TO)# <verb> #getEword#  #gen(ACTION)# <tense> #gen(TENSE)# PERIOD
void after_object() {

  cout << "Processing <after object>" << endl;

  switch(next_token()) {
      //<verb> has WORD2
    case WORD2:
      verb();
      tense();
      match(PERIOD);
      break;
    case WORD1: //<noun> has WORD1 or PRONOUN
      noun();
      match(DESTINATION);
      gen("TO");
      verb();
      tense();
      match(PERIOD);
      break;
    case PRONOUN:
      noun();
      match(DESTINATION);
      gen("TO");
      verb();
      tense();
      match(PERIOD);
      break;
    default:
      syntaxerror2("after_object", saved_lexeme, trace);
  }
}

// <noun> ::= WORD1 | PRONOUN 
void noun() {

  cout << "Processing <noun>" << endl;

  switch(next_token()) {
    case WORD1:
      match(WORD1);
      getEword();
      break;
    case PRONOUN:
      match(PRONOUN);
      getEword();
      break;
    default:
      syntaxerror2("noun", saved_lexeme, trace);
  }
}

// <verb> ::= WORD2  
void verb() {

  cout << "Processing <verb>" << endl;
  
  match(WORD2);
  getEword();
  gen("ACTION");
}
// <be> ::=   IS | WAS  
void be(){
  cout << "Processing <be>" << endl;

  switch(next_token()) {
    case IS:
      match(IS);
      break;
    case WAS:
      match(WAS);
      break;
    default:
      syntaxerror2("be",saved_lexeme, trace);
  }
  gen("DESCRIPTION");
  gen("TENSE");
}
// <tense> ::= VERBPAST  | VERBPASTNEG | VERB | VERBNEG   
void tense() {
  cout << "Processing <tense>" << endl;

  switch(next_token()) {
    case VERBPAST:
      match(VERBPAST);
      break;
    case VERBPASTNEG:
      match(VERBPASTNEG);
      break;
    case VERB:
      match(VERB);
      break;
    case VERBNEG:
      match(VERBNEG);
      break;
    default:
      syntaxerror2("tense", saved_lexeme, saved_token);
  }
  gen("TENSE");
}
// ******************** END OF PARSER ******************** //



// Finds the english word for a given japanese word
void getEword() {
  bool found = false;
  // Loop through dictionary and find the english word
  for(auto const& x : lexicon) {
    // if the savedEword is the same as the value (english word) for a given lexicin[key]
    // set the savedEword as that value
    if(x.first == saved_lexeme) {
      savedEword = x.second;
      // set found to true
      found = true;
    }
  }
  // otherwise if we did not find an english word in the dict.
  // set savedEword to saved_lexeme
  if(found == false) {
    savedEword = saved_lexeme;
  }
}
// Generate the translated words and put the in the translated.txt file, 
void gen(string token_t) {
  // if we encounter a TENSE, print out a new line
  if(token_t == "TENSE") {
    output << token_t << ": " << tokenName[saved_token] << "\n" << endl;
  }
  // if there is no TENSE just print out w/o a new line
  else {
    output << token_t << ": " << savedEword << endl;
  }
}


int main() {

  ifstream input_lex;

  input_lex.open("lexicon.txt");
  while(input_lex >> japanese >> english) {
    lexicon[japanese] = english;
  }
  //** opens the lexicon.txt file and reads it into Lexicon
  //** closes lexicon.txt 
  input_lex.close();
  //** opens the output file translated.txt
  output.open("translated.txt");

  cout << "Enter the input file name: ";
  cin >> filename;
  fin.open(filename.c_str());

  cout << "Enter y/n to activate/deactivate tracing messages: ";
  cin >> trace;

  if(trace == 'y') {
      cout << "\nTracing messages has been activated...\n" << endl;
  }
  else if(trace == 'n') {
      cout << "\nTracing messages has been deactivated...\n" << endl;
  }
  
  // calls the <story> to start parsing
  story();
  // closes the input file 
  fin.close();
  // closes traslated.txt
  output.close();

}

