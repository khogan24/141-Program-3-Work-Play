
/* ------------------------------------------------
 * main.cpp
 *
 * Class: Program #3 for CS 141, Fall 2019
 * System: Codio
 * Author: Kevin Hogan
 *
*/
#include <fstream>      // For file input
#include <cstdlib>      // For exit and abs
#include <iostream>
#include <string>
#include <fstream>      // For file input
#include <vector>		// For dictionary vector
#include <iomanip>		// For setw()
#include <cctype>       // For tolower()
using namespace std;

ifstream inputFileStream;    		// Vector of dictionary words read in from file
const string FILE_NAME = "dictionary.txt"; // not gonna change, probs could just call this in the 'read' function.
vector<string> dictionary; //dictionary with only words of <lengthOfWordsToUse> length. global because many functions use this
vector<int> parentVecorIndex;    
vector<string> expandedWordTree; 
vector<int> wordLenCount(30);    
const int SETW_VAL = 4; // global because many functions have a cout << setw(...) statment

//-----------------------------------------------------------------------------------------
void populateDictionary(int size) // fills the dict with words of set length.
{
	inputFileStream.open(FILE_NAME.c_str() );
	string line;
   // Verify that the open worked
    if( !inputFileStream.is_open()) {
        cout << "Could not find input file " << FILE_NAME << "  Exiting..." << endl;
        exit( -1);
    }
	while(getline(inputFileStream, line))
	{
		wordLenCount.at(line.size())++;
		line[0] = tolower(line[0]);
		if(line.size() == size)
		{
			dictionary.push_back(line);
		}
	}
	inputFileStream.close();
}


//-----------------------------------------------------------------------------------------
// Display ID info
void displayIdInfo()
{
    cout << " \n"
	<< "Program #3: Work Play  \n"
	<< "Author: Kevin Hogan      \n"
	<< "Lab: Tues 10am          \n"
	<< "System:  Codio         \n"
	<< "UIN: 673032668          \n"
	<< "NetId: KHOGAN24          \n"
	<< " \n";
}//end displayIDInfo()


//-----------------------------------------------------------------------------------------
// Use binary search to look up the search word in the dictionary vector, returning
// the word's index if found, -1 otherwise.
//
long binarySearch(
            string searchWord,                           // word to be looked up
			vector<string> vectorToSearch)               // vector to look through

{
    long low, mid, high;     // array indices for binary search
    long searchResult = -1;  // Stores index of word if search succeeded, else -1
    
    // Binary search for word
    low = 0;
    high = vectorToSearch.size() - 1;
    while ( low <= high)  {
        mid = (low + high) / 2;
        // SearchResult negative value means word is to the left, positive value means
        // word is to the right, value of 0 means word was found
        searchResult = searchWord.compare( vectorToSearch[ mid] );
        if ( searchResult == 0)  {
            // Word IS in dictionary, so return the index where the word was found
            return mid;
        }
        else if (searchResult < 0)  {
            high = mid - 1; // word should be located before the mid location
        }
        else  {
            low = mid + 1; // word should be located after the mid location
        }
    }
    
    // Word was not found
    return -1;
}//end binarySearch()


//-----------------------------------------------------------------------------------------
string randomWordGenerator(int lengthOfWordsToUse)
{
	return dictionary.at(rand() % dictionary.size());
}


//-----------------------------------------------------------------------------------------
bool checkIfWord(string userWord, vector<string> dictionary)
{
	int result = binarySearch(userWord , dictionary) + 1;
	if (result == 0)
		{
			cout <<"Sorry that is not a word, please try again"<< endl;
		}
	return result;
}


//-----------------------------------------------------------------------------------------
// iterates over each value of each word to check if they differ by only one letter
int offByOneLetter(string word1, string word2) // returns the amount the two words vary by. 
{ // assume strings are of the same size !!!
	int differentLetterCounter = 0;
	for(int i = 0; i < word1.size(); i++)
	{
		if(word1.at(i) != word2.at(i))
		{
			differentLetterCounter++;
		}
	}


	return differentLetterCounter; // returns amount words vary by.

}


//-----------------------------------------------------------------------------------------
// linear search, for when we are not sorted
int compareSearch(string wordToSearchFor, vector<string> vectorToSearch)// returns the index, if non is found, -1
{
	for(int i = 0; i < vectorToSearch.size(); i++)
	{
		if( wordToSearchFor == vectorToSearch.at(i)  )
		{
			return i; 
		}
	}return -1;
}


//-----------------------------------------------------------------------------------------
int expandWord(string startWord, string endWord, bool debugOn)// find all words reachable by changing one letter at a time.
{
	expandedWordTree.clear();// clears this vector, so we dont make it 2x as long
	parentVecorIndex.clear();// clears this vector, so we dont make it 2x as long
	string newWord; // will take on the value of expandedWordTree.at(expandWordTreeIndex)
	expandedWordTree.push_back(startWord);
	int counter = 0; // to track each time through the loop. if this exceeds the total number of words, leave the function.
	
	for(int expandedWordTreeIndex = 0; expandedWordTreeIndex < expandedWordTree.size(); expandedWordTreeIndex++)//for each word in expandedWordTree.
	{
		if(debugOn) {cout << expandedWordTreeIndex << ". " << expandedWordTree.at(expandedWordTreeIndex) << ":  ";} // print or be quiet?
		
		for(int letter = 0; letter < expandedWordTree.at( expandedWordTreeIndex).size(); letter++) // for each letter in each word.
		{
			newWord = expandedWordTree.at(expandedWordTreeIndex);
			for(int i = 97 /* ascii for 'a' */; i <= 122 /* ascii for 'z' */; i++) // from 'a' to 'z'
			{
				newWord[letter] = (char)i;// change the letter at index <letter> to next letter
				if((binarySearch(newWord, dictionary) != -1 ) // it it a word in the dictionary?
					&& (compareSearch(newWord, expandedWordTree) == -1)) // is this word already used? compareSearch(...) returns -1 if word is not found.
				{
					expandedWordTree.push_back(newWord);// if all checks pass, push it to expandWordTree.
					parentVecorIndex.push_back(expandedWordTreeIndex); // records current word's parent
					//cout<< " entered big if " << i++ << endl;
					
					if(debugOn) {cout  << expandedWordTree.size()-1 << ":"<< expandedWordTree.back() << " ";}
				}// end if(compareSearch(...){...} && ...)

				if(expandedWordTree.back() ==  endWord) // found the last word
				{
					cout << "\nWinning sequence was found!\n" << endl;
					return 1;
				}// end (expandedWordTree...)
			}// end if(...)
		}// end for(int letter...)
		if(debugOn) {cout << endl;}// print or be quiet?
		counter++;
		if(counter >=dictionary.size())
		{
		cout << "No Sequence Found" << endl;
		return 0;
		}
	}// end for(int expandWor
}// end expandWord(...){...}


int isInBounds(int & index)
{
	if(index < 0)
	{
		cout<<"Invalid start value. Resetting it to the minimun value, 0." << endl;
		index = 0;
		return 0;
	}
	else if(index > dictionary.size()-1)
	{
		cout<<"Invalid end value.  Resetting it to the maximum which is " << dictionary.size()-1<< endl;
		index = dictionary.size()-1;
		return 1;
	}
	return 2;
}
//-----------------------------------------------------------------------------------------
void displayWords(int startIndex, int endIndex)
{
	isInBounds(startIndex);
	isInBounds(endIndex);
	for(int i = startIndex; i <= endIndex; i++)
	{
		cout<< i <<" "<< dictionary.at(i)<<endl;
	}
}
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
int getNewWords(string & userInput, int lengthOfWordsToUse) // returns -1 if word does not pass, 1 if it does, and 2 if it was a random word and 0 if 'exit' is entered
{
	if(userInput == "r")
	{
		userInput = randomWordGenerator(lengthOfWordsToUse);
		return 2;
	}
	else if(userInput.compare("exit") == 0) // user types exit, program exits.
	{
		return 0;
	}
	else if(userInput.size() != lengthOfWordsToUse)  // wrong size
	{
		cout << "   *** '" << userInput <<"' is not of length " << lengthOfWordsToUse <<". Please retry." <<endl;
		return -1;
	}
	else if(binarySearch(userInput, dictionary) < 0) // not a word in dictionary
	{
		cout <<"   *** '" << userInput <<"' is not in the dictionary. Please retry." << endl;
		return -1;
	}
	else
	{
		return 1; // word passed all tests, return 1.
	}
	
}


//-----------------------------------------------------------------------------------------
bool playGame(string startWord, string endWord) // DOES NOT CHECK WORD DIFFERENCES. if(not longer && not off by more than 1 && not 'exit') <<<<<<<------------
{
	// pass user input to isValidGues() until it passes all checks
	// end when user guess is endWord
	string userInput = startWord;
	int counter = 1;
	int size = startWord.size();
	bool userHasWon = false;
	do
	{
		cout <<  " "<<counter << ". Previous word is '" << userInput << "'. Next word: ";
		cin >> userInput;
		
		if(userInput.compare("exit") == 0)  // 'exit'
		{
			return(false);
		}
		
		if(getNewWords(userInput, size) != 1) // check size. returns -1 if fails size and existance checks, 1 if it passes, and 0 if 'exit'   WORKS
		{
			userInput = startWord;
			continue;
		}
		else if(offByOneLetter(userInput, startWord) != 1) // returns 1 if off by one letter
		{
			cout << " *** '" << userInput << "'must be exactly 1 character different.  Please retry." <<endl;
			userInput = startWord;
			continue;
		}
		else if(userInput.compare(endWord) == 0)
		{
			cout << "Congratulations, you did it!" << endl;
			userHasWon = true;
		}
		startWord = userInput;
		counter++;
	}while(!userHasWon);
}

int displayWinningSoln(string startWord, string endWord, bool hasBedFound)
{
	cout << "\n";
	if(!hasBedFound)
	{
		cout << setw(SETW_VAL) << "*** You must find the solution before displaying it.\n" << endl;
		return 0;
	}

	cout <<"Winning sequence in reverse order is: " << endl;
	int index = expandedWordTree.size()-1;
	string wordToPrint = expandedWordTree.back();
	int x = index;
	//cout <<wordToPrint/* last word*/ << " " << compareSearch(wordToPrint, expandedWordTree)/* its index*/ << endl;
	index = parentVecorIndex.at(index - 1);
	do
	{
		cout << setw(SETW_VAL*2) <<compareSearch(wordToPrint, expandedWordTree) <<". " << wordToPrint <<   endl;
		wordToPrint = expandedWordTree.at(index);
		index = parentVecorIndex.at(compareSearch(wordToPrint, expandedWordTree));
		
	}while(wordToPrint != startWord);
	cout << setw(SETW_VAL * 2) <<compareSearch(wordToPrint, expandedWordTree) <<". " << wordToPrint  << endl;
	return 0;
	
	
	
}


//-----------------------------------------------------------------------------------------
int main()
{
    int lengthOfWordsToUse = 3;         // Default length of word to use in word transformation
	int startIndex, endIndex;
    int menuOption = -1;                // User menu option selection
	string userWord1, userWord2;
	string startWord = "dog";           // The start word for transformation
	string endWord = "cat";
	int nextWordCheck = 0;
	bool hasBeenFound = false; // has the winning solution been found?
	const int MIN_WORD_LEN_TO_DISPLAY = 1400; // sets how long each word must be to be shown in the count at the beggining of the program.

    // Display ID info
    displayIdInfo();
    
    // Seed the random number generator
    srand( 1);  // Change 1 to time(0) for non-predictable results, but leave as 1 for testing
	populateDictionary(lengthOfWordsToUse);
	cout << "Total number of words in dictionary file: 235886\n";
	cout << endl;
	cout << "Word lengths where there are more than "<<  MIN_WORD_LEN_TO_DISPLAY << " words:"<< endl;
	cout <<"Length  How Many"<<endl;
	cout <<"------  --------" << endl;
	for(int i = 0; i < wordLenCount.size(); i++)
	{
		if(wordLenCount[i] >= MIN_WORD_LEN_TO_DISPLAY)
		{
			cout << setw(SETW_VAL) <<i << "   " << setw(2*SETW_VAL)<< wordLenCount[i] << endl;
		}
	}
    // Menu display and handling infinite loop
    while( true)
	{
        cout << "\nCurrently we have " << dictionary.size() << " words of length "
             << lengthOfWordsToUse << " in the dictionary.  \n"
             << "Changing from '" << startWord << "' to '" << endWord << "'" << endl
             << endl;
        cout << "Choose from the following options:  \n"
             << "   1. Change the word length        \n"
             << "   2. Display some dictionary words \n"
             << "   3. Get start and end words       \n"
             << "   4. Play the word change game     \n"
             << "   5. Find the end word with debug  \n"
             << "   6. Find the end word             \n"
             << "   7. Display an answer sequence    \n"
             << "   8. Exit the program              \n"
             << "Your choice -> " << endl;
        cin >> menuOption;
		switch(menuOption)
		{
				
			case 1://1. Change the word length   
				
				cout<< endl<< "\nWhat length words do you want to use? ";
				cin >> lengthOfWordsToUse;
				dictionary.clear();
				startWord = endWord = "";
				populateDictionary(lengthOfWordsToUse);
				break;
				
			case 2://2. Display some dictionary words 
					cout << "Enter the start and end index values of words to display: ";
					cin >> startIndex;
					cin >> endIndex;
					cout << endl <<"About to display dictionary words from " << startIndex<<" to " << endIndex <<endl;
					if(startIndex > dictionary.size()-1)
					{
						cout<<"Invalid end value.  Resetting it to the maximum which is " << dictionary.size()-1<< endl;
						startIndex = dictionary.size()-1;
					}
					if(endIndex > dictionary.size()-1)
					{
						cout<<"Invalid end value.  Resetting it to the maximum which is " << dictionary.size()-1<< endl;
						endIndex = dictionary.size()-1;
					}
					displayWords(int(startIndex), int(endIndex));
				
				break;
				
			case 3: //3. Get start and end words  
				
				if(dictionary.size() == 0)
				{
					populateDictionary(lengthOfWordsToUse);
				}
				do{
					cout << "\nEnter starting word, or 'r' for a random word: " << endl;
					cin >> startWord;
					nextWordCheck = getNewWords(startWord, lengthOfWordsToUse);
					if(nextWordCheck == 0)
					{
						cout <<"\nExiting program." <<endl;
						exit(0);
					}
				}while(nextWordCheck < 0);
				
				do{
					cout << "\nEnter ending word, or 'r' for a random word: ";
					cin >> endWord;
					nextWordCheck = getNewWords(endWord, lengthOfWordsToUse);
					if(nextWordCheck == 0)
					{
						cout <<"\nExiting program." <<endl;
						exit(0);
					}
				}while(nextWordCheck < 0);

				break;
				
			case 4://4. Play the word change game 
				if(dictionary.size() == 0)
				{
					populateDictionary(lengthOfWordsToUse);
				}
				playGame(startWord, endWord);

				break;
				
			case 5://5. Find the end word with debug  
				expandWord(startWord, endWord, true);
				hasBeenFound = true;
				
				break;
				
			case 6://6. Find the end word    
				expandWord(startWord, endWord, false);
				hasBeenFound = true;

				break;
				
			case 7://7. Display an answer sequence
				displayWinningSoln(startWord, endWord, hasBeenFound);
				
				
				break;
				
			case 8://8. Exit the program  
				cout << "\nExiting the program" << endl;
				exit(0);
				break;
			default:
				cout <<"Invalid menu option.  Please retry." << endl;
				break;
		}
        
		// Menu option handling should go here
		// ...
        
    } 
	
    return 0;
}//end main()


