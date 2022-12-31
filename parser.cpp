#include<iostream>
#include<fstream>
#include<string>
#include "scanner.cpp"
using namespace std;

/*
Authors:
Brian Suan
Nate Flores
Lyle Gonzales
*/

ofstream error_file;        // File to store the errors
string filename;            // For the user filename user input
string saved_lexeme;        //is a sequence of characters in the source program that matches the pattern for a token 
tokentype saved_token;      //global buffer for token the scanner returned
bool token_available;       // global flag indicating whether
char trace;                 // To represent the user's trace choice



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
    if(trace_choice == 'y') {
        cout << "SYNTAX ERROR 2: unexpected \"" << saved_lexeme << "\" found in " << parserF << ". Halting..." << endl;

        error_file.open("errors.txt", std::ios_base::app);
        error_file << "Error from file: " << filename << "\n\tSYNTAX ERROR 2: unexpected \"" << saved_lexeme << "\" found in " << parserF << ". Halting...\n" << endl;
        error_file.close();

        exit(1); 
    }
    else {
        error_file.open("errors.txt", std::ios_base::app);
        error_file << "Error from file: " << filename << "\n\tSYNTAX ERROR 2: unexpected \"" << saved_lexeme << "\" found in " << parserF << ". Halting...\n" << endl;
        error_file.close();

        exit(1);
    } 
}


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
    cout << "\nProcessing <story>\n" << endl;
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

//<s> ::= [CONNECTOR] <noun> SUBJECT <after subject>
void s() {

    cout << "Processing <s>" << endl;
    next_token();

    if(next_token() == CONNECTOR) {
	    match(CONNECTOR);
	}

    noun();
    match(SUBJECT);
    after_subject();    
}

//<after subject> ::= <verb> <tense> PERIOD | <noun> <after noun>
void after_subject() {
    cout << "Processing <after subject>" << endl;
    switch(next_token()) {      //<verb> = word2
        case WORD2:
            verb();
            tense();
            match(PERIOD);
            break;
        case WORD1:             //<noun has two cases word 1 | pronoun
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

//<after noun> ::= <be> PERIOD | DESTINATION <verb><tense> PERIOD| OBJECT <after object>
void after_noun() {

    cout << "Processing <after noun>" << endl;

    switch(next_token()) {              //<be> has is or was
        case IS:
            be();
            match(PERIOD);
            break;
        case WAS:
            be();
            match(PERIOD);
            break;
        case DESTINATION:               //case destination
            match(DESTINATION);
            verb();
            tense();
            match(PERIOD);
            break;
        case OBJECT:                    //case object
            match(OBJECT);
            after_object();
            break;
        default:
            syntaxerror2("after_noun", saved_lexeme,trace);
    }
}

//<after object> ::= <verb> <tense> PERIOD | <noun> DESTINATION <verb> <tense> PERIOD
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
            verb();
            tense();
            match(PERIOD);
            break;
        case PRONOUN:
            noun();
            match(DESTINATION);
            verb();
            tense();
            match(PERIOD);
            break;
        default:
            syntaxerror2("after_object", saved_lexeme, trace);
    }
}


void noun() {
    cout << "Processing <noun>" << endl;

    switch(next_token()) {
        case WORD1:
            match(WORD1);
            break;
        case PRONOUN:
            match(PRONOUN);
            break;
        default:
            syntaxerror2("noun", saved_lexeme, trace);
    }
}


void verb() {
    
    cout << "Processing <verb>" << endl;
    match(WORD2);

}


void be() {

    cout << "Processing <be>" << endl;

    switch(next_token()) {
        case IS:
            match(IS);
            break;
        case WAS:
            match(WAS);
            break;
        default:
            syntaxerror2("be", saved_lexeme, trace);
    }
}


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
            syntaxerror2("tense", saved_lexeme, trace);
    }
}


// ----------- Driver ----------------- //
int main() {

    cout << "Enter the input file name: ";
    cin >> filename;
    fin.open(filename.c_str());

    cout << "Enter y/n to activate/deactivate tracing messages: ";
    cin >> trace;

    if(trace == 'y') {
        
        cout << "\nError tracing has been activated...\n" << endl;
    }
    else if(trace == 'n') {
        cout << "\nError tracing has been deactivated...\n" << endl;
    }
    
    story();  //** calls the <story> to start parsing
    //** closes the input file 
    fin.close();
}