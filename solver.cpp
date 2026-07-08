#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <utility>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;

const std::string RESET     = "\033[0m";
const std::string FIRST     = "\033[1;38;5;196;49m";
const std::string SECOND    = "\033[1;38;5;202;49m";
const std::string THIRD     = "\033[1;38;5;208;49m";
const std::string FOURTH    = "\033[1;38;5;214;49m";
const std::string FIFTH     = "\033[1;38;5;220;49m";
const std::string EXTRA     = "\033[1;38;5;7;49m";

int main() {
    // Get length of word
    cout << "Enter the length of the word: ";
    string line;
    getline(cin,line);
    int wordLength = stoi(line);

    // Read in the wordlist
    ifstream wordlist("wordlist.txt");
    vector<string> words;
    string word;
    while (wordlist >> word) {
        if (word.length() == wordLength) {
            words.push_back(word);
        }
    }
    wordlist.close();

    // Map of unguessed letters
    unordered_set<char> unguessed;
    for (char c = 'a'; c <= 'z'; ++c) {
        unguessed.insert(c);
    }

    // Guess letters and recieve best next letter to guess
    char done = 'y';
    int misses = 0;
    do {
        // Guess a letter
        cout << "Guess a letter: ";
        getline(cin,line);
        char ltr = tolower(line[0]);

        cout << "Hit or miss (h/m): ";
        getline(cin,line);
        char hitOrMiss = tolower(line[0]);

        unguessed.erase(ltr);

        // On a hit
        if (hitOrMiss == 'h') { 
            // Remove words without the letter the correct number of times
            cout << "How many are in the word: ";
            getline(cin,line);
            int ltrCount = stoi(line);
            
            auto it = words.begin();
            while (it != words.end()) {
                if (it->find(ltr) == string::npos || count(it->begin(), it->end(), ltr) != ltrCount)
                    it = words.erase(it);
                else ++it;
            }
        // On a miss
        } else {
            misses++;
            // Remove words with the letter
            auto it = words.begin();
            while (it != words.end()) {
                if (it->find(ltr) != string::npos)
                    it = words.erase(it);
                else ++it;
            }
        }

        // Make a map of how many words per remaining letter
        unordered_map<char, pair<int,vector<string>>> letterToPreval;
        for (const auto& l : unguessed) {
            for (const auto& w : words) {
                if (w.find(l) != string::npos) {
                    letterToPreval[l].first++;
                    letterToPreval[l].second.push_back(w);
                }
            }
        }

        // Sort letters by prevalance
        vector<pair<char, pair<int, vector<string>>>> ltrList(letterToPreval.begin(), letterToPreval.end());
        sort(ltrList.begin(), ltrList.end(), [](const auto& a, const auto& b) {
            return a.second.first > b.second.first;
        });

        // End if there is only one word left
        if (words.size() == 1) {
            cout << "\nThe word is: " << FIRST << words[0] << RESET << endl;
            break;
        } else if (!words.empty() && ltrList.empty()) {
            cout << "\n ---------- The word is one of the following ---------- " << endl;
            int font = 5;
            for (const auto& w : words) {
                cout << "- ";
                switch(font) {
                    case 5: cout << FIRST; break;
                    case 4: cout << SECOND; break;
                    case 3: cout << THIRD; break;
                    case 2: cout << FOURTH; break;
                    case 1: cout << FIFTH; break;
                    default: cout << EXTRA;
                }
                if(font > 0) font--;
                cout << w << RESET << endl;
            }
            break;
        } else if (words.empty()) {
            cout << FIRST << "\nNo words remaining." << RESET << endl;
            break;
        }

        // Print the sorted list
        cout << "\n---------- BEST NEXT GUESSES ----------" << endl;
        cout << "Total words remaining: " << words.size() << "\n" << endl;
        int s = min(static_cast<int>(ltrList.size()),10);
        for (int i = 0; i < s; ++i) {
            // Print the letter and number of words
            switch(i) {
                case 0: cout << "{ " << FIRST; break;
                case 1: cout << "{ " << SECOND; break;
                case 2: cout << "{ " << THIRD; break;
                case 3: cout << "{ " << FOURTH; break;
                case 4: cout << "{ " << FIFTH; break;
                default: cout << "{ " << EXTRA;
            }
            cout << static_cast<char>(toupper(ltrList[i].first)) << RESET << " } : " << ltrList[i].second.first << endl;
            
            // Print a few words as an example
            int n = min(static_cast<int>(ltrList[i].second.second.size()),5);
            if (n > 0) cout << ltrList[i].second.second[0];
            for (int j = 1; j < n; j++) {
                cout << ", " << ltrList[i].second.second[j];
            }
            if (ltrList[i].second.second.size() > 5) cout << ", ...";
            cout << "\n\n";
        }
    } while (true);

    // Print the number of misses
    cout << "\nTotal misses: " << EXTRA << misses << RESET << endl;
}
