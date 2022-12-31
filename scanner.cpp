#include<iostream>
#include<fstream>
#include<string>
using namespace std;

/*
Authors:
Brian Suan
Nate Flores
Lyle Gonzales
*/


ofstream lexical_errors;

bool word(string s) {

    int state = 0;
    int charpos = 0;

    /* replace the following todo the word dfa  */
    while (s[charpos] != '\0')
    {
         //for q0
        if (state == 0 && s[charpos] == 'b' | s[charpos] == 'g' | s[charpos] == 'h' | s[charpos] == 'k' | s[charpos] == 'm' | s[charpos] == 'n' | s[charpos] == 'p' | s[charpos] == 'r')
            state = 1; //qy
        else if (state == 0 && s[charpos] == 'd' | s[charpos] == 'w' | s[charpos] == 'z' | s[charpos] == 'y' | s[charpos] == 'j')
            state = 2; //qsa
        else if (state == 0 && s[charpos] == 't')
            state = 3; //qt
        else if (state == 0 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E')
            state = 4; //q0q1
        else if (state == 0 && s[charpos] == 'c')
            state = 5; //qc
        else if (state == 0 && s[charpos] == 's')
            state = 7; //qs
        else {
            //for qy
            if (state == 1 && s[charpos] == 'y')
                state = 2;//qsa
            else if (state == 1 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E')
                state = 4;//q0qy
            else {
                //for qsa
                if (state == 2 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E')
                    state = 4; //q0q1    
                else {
                    //for qt
                    if (state == 3 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E')
                        state = 4; //qoq1
                    else if (state == 3 && s[charpos] == 's')
                        state = 2; //qsa //below
                    else {
                        //for q0q1
                        if (state == 4 && s[charpos] == 'c')
                            state = 5; //qc
                        else if (state == 4 && s[charpos] == 's')
                            state = 7; //qs
                        else if (state == 4 && s[charpos] == 't')
                            state = 3; //qt                        
                        else if (state == 4 && s[charpos] == 'b' | s[charpos] == 'g' | s[charpos] == 'h' | s[charpos] == 'k' | s[charpos] == 'm' | s[charpos] == 'p' | s[charpos] == 'r')
                            state = 1; //qy
                        else if (state == 4 && s[charpos] == 'n')
                            state = 6; //qoqy
                        else if (state == 4 && s[charpos] == 'd' | s[charpos] == 'w' | s[charpos] == 'z' | s[charpos] == 'y' | s[charpos] == 'j')
                            state = 2; //qsa
                        else if(state == 4 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E') {
                            state = 4; //q0q1
                        }

                        else {
                            //for qc
                            if (state == 5 && s[charpos] == 'h')
                                state = 2; //qsa
                            else {
                                //for q0qy
                                if (state == 6 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E')
                                    state = 4; //q0q1
                                else if (state == 6 && s[charpos] == 'c')
                                    state = 5; //qc //here
                                else if (state == 6 && s[charpos] == 's')
                                    state = 7; //qs
                                else if (state == 6 && s[charpos] == 't')
                                    state = 3;//qt
                                else if (state == 6 && s[charpos] == 'b' | s[charpos] == 'g' | s[charpos] == 'h' | s[charpos] == 'k' | s[charpos] == 'm' | s[charpos] == 'n' | s[charpos] == 'p' | s[charpos] == 'r')
                                    state = 1;//qy
                                else if (state == 6 && s[charpos] == 'd' | s[charpos] == 'w' | s[charpos] == 'z' | s[charpos] == 'y' | s[charpos] == 'j')
                                    state = 2; //qsa
                                else {
                                  
                                    //for qs
                                    if (state == 7 && s[charpos] == 'a' | s[charpos] == 'i' | s[charpos] == 'u' | s[charpos] == 'e' | s[charpos] == 'o' | s[charpos] == 'I' | s[charpos] == 'E')
                                        state = 4;//q0q1
                                    else if (state == 7 && s[charpos] == 'h')
                                        state = 2;//qsa
                                    else {                          
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } //here

        charpos++; //increment charpos
    }  //end of while

    if (state == 0 || state == 4 || state == 6)
        return true;
    else
        return false;
} 


bool period (string s) {  
    int state = 0;
    int charpos = 0;

    // Checks each character of the word to test if the expression 
    while (s[charpos] != '\0') {
        if (state == 0 && s[charpos] == '.') {
          state = 1;
        }
        else {
          return false;
        }
        charpos++;
    }


    if (state == 1) {
        return true;
    }
    else {
        return false;
    }

}

// -------------- Three  Tables ------------------ //

// ** Update the tokentype to be WORD1, WORD2, PERIOD, ERROR, EOFM, etc.
enum tokentype {
    WORD1, WORD2, PERIOD, ERROR,
    EOFM, VERB, VERBNEG, VERBPAST,
    VERBPASTNEG, IS, WAS, OBJECT,
    SUBJECT, DESTINATION, PRONOUN,
    CONNECTOR
};

// ** For the display names of tokens - must be in the same order as the tokentype.
string tokenName[30] = {
    "WORD1", "WORD2", "PERIOD", "ERROR",
    "EOFM", "VERB", "VERBNEG", "VERBPAST",
    "VERBPASTNEG", "IS", "WAS", "OBJECT",
    "SUBJECT", "DESTINATION", "PRONOUN",
    "CONNECTOR"
};

// ** Need the reservedwords table to be set up here. 
struct reserved {
    string word;
    tokentype token_type;
}

reserved[] = {
    {"masu", VERB},
    {"masen", VERBNEG},
    {"mashita", VERBPAST},
    {"masendeshita", VERBPASTNEG},
    {"desu", IS},
    {"deshita", WAS},
    {"o", OBJECT},
    {"wa", SUBJECT},
    {"ni", DESTINATION},
    {"watashi", PRONOUN},
    {"anata", PRONOUN},
    {"kare", PRONOUN},
    {"kanojo", PRONOUN},
    {"sore", PRONOUN},
    {"mata", CONNECTOR},
    {"soshite", CONNECTOR},
    {"shikashi", CONNECTOR},
    {"dakara", CONNECTOR},
    {"eofm", EOFM}
};


// ------------ Scanner and Driver ------------------ //

ifstream fin;  // global stream for reading from the input file

// Scanner processes only one word each time it is called
// Gives back the token type and the word itself
int scanner(tokentype& tt, string& w) {

    // ** Grab the next word from the file via fin
    fin >> w;
    // 1. If it is eofm, return right now.   
    if (w == "eofm") {
        //cout << "END OF FILE" << endl;
        return 0;
    }
    
    // Check if token is true
    if (word(w)) {   
        // Check if token is a reserved word
        for (int i = 0; i < 19; i++) {
            if (w == reserved[i].word) {
              tt = reserved[i].token_type;
              return 0;
            }
        }

        // Check the last letter of the token
        if (w[w.length()-1] == 'I' || w[w.length()-1] == 'E') {
          tt = WORD2;
        }
        else {
          tt = WORD1;
        }
    }
    else {
        if (period(w)) {           
            tt = PERIOD;
        }
        else {
            cout << "LEXICAL ERROR: \"" << w << "\" is not a valid token" << endl;

            lexical_errors.open("errors.txt", std::ios_base::app);
            lexical_errors << "\nLEXICAL ERROR: \"" << w << "\" is not a valid token" << endl;
            lexical_errors.close();

            tt = ERROR;
        }
    }


    return 0;

}