#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

void decryptSubstCipherCommand(const QuadgramScorer& scorer);
void decryptSubstCipherFileCommand(const QuadgramScorer& scorer);

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

/**
 * Print instructions for using the program.
 */
void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "F - Decrypt Subst File" << endl;
  cout << "X - Exit Program" << endl;
}

int main() {
  ifstream dictFile("dictionary.txt");
  vector<string> dictionary;
  string word;
  while (dictFile >> word) {
    dictionary.push_back(word);  // Load words from dictionary into vector
  }

  // Load the quadgrams for the Substitution Cipher Decryptor
  ifstream quadgramFile("english_quadgrams.txt");
  vector<string> quadgrams;
  vector<int> counts;
  string line;

  while (getline(quadgramFile, line)) {
    stringstream ss(line);
    string quadgram;
    int count;
    getline(ss, quadgram, ',');
    ss >> count;
    quadgrams.push_back(quadgram);
    counts.push_back(count);
  }

  QuadgramScorer scorer(quadgrams, counts);  // Initialize the quadgram scorer

  // Seed the random number generator
  Random::seed(time(NULL));

  string command;

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  // Main command loop
  do {
    printMenu();  // Show the menu
    cout << endl << "Enter a command (case does not matter): ";
    getline(cin, command);
    cout << endl;

    if (command == "C" || command == "c") {
      runCaesarEncrypt();  // Call Caesar encrypt command
    } else if (command == "D" || command == "d") {
      runCaesarDecrypt(dictionary);  // Call Caesar decrypt command
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);  // Compute English-ness score
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();  // Apply random substitution cipher
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(
          scorer);  // Decrypt substitution cipher (console)
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFileCommand(scorer);
    } else if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));  // Set random seed for testing
    } else if (!(command == "X" || command == "x") && !cin.eof()) {
      cout << "Invalid command. Please try again." << endl;
    }
    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  cout << "Exiting program. Goodbye!" << endl;
  return 0;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  if (c < 'A' || c > 'Z') return c;

  // Calculate the new character with wrap-around
  int index = (c - 'A' + amount) % 26;
  if (index < 0) index += 26;  // Ensure positive result if amount is negative
  //this isnt needed but better to have in case.
  return 'A' + index;
}

string rot(const string& line, int amount) {
  string result;
  for (char c : line) {
    if (isalpha(c)) {
      // Convert to uppercase and rotate
      result += rot(toupper(c), amount);
    } else if (isspace(c)) {
      // Keep spaces in the result
      result += c;
    }
  }
  return result;
}

void runCaesarEncrypt() {
  // TODO: student fill this in
  string text;
  string amount_str;
  int amount;

  // Prompt user for text and rotation amount
  cout << "Enter the text to Caesar encrypt: ";
  getline(cin, text);
  cout << "Enter the number of characters to rotate by: ";
  getline(cin, amount_str);

  // Convert rotation amount from string to int
  amount = stoi(amount_str);

  // Perform Caesar encryption and output result
  string encrypted = rot(text, amount);
  cout << "Encrypted text: " << encrypted << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  // TODO: student fill this in
  for (string& word : strings) {
    word = rot(word, amount);  // Rotate each word in the vector
  }
}

string clean(const string& s) {
  // TODO: student fill this in
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      result += toupper(
          c);  // Keep only alphabetic characters and convert to uppercase
    }
  }
  return result;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student fill this in
  istringstream iss(s);
  vector<string> result;
  string word;
  while (iss >> word) {
    result.push_back(clean(word));  // Split by spaces
  }
  return result;
}

string joinWithSpaces(const vector<string>& words) {
  // TODO: student fill this in
  ostringstream oss;
  for (size_t i = 0; i < words.size(); ++i) {
    if (i > 0) oss << " ";
    oss << words[i];
  }
  return oss.str();
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  // TODO: student fill this in
  int count = 0;
  for (const string& word : words) {
    if (find(dict.begin(), dict.end(), word) != dict.end()) {
      count++;  // Count words found in the dictionary
    }
  }
  return count;
}

void runCaesarDecrypt(const vector<string>& dict) {
  // TODO: student fill this in
  string text;
  cout << "Enter the text to Caesar decrypt: ";
  getline(cin, text);

  vector<string> words = splitBySpaces(text);  // Split and clean the text

  vector<pair<int, string>> validDecryptions;  // To store all valid decryptions

  for (int shift = 0; shift < 26; ++shift) {
    vector<string> attempt = words;
    rot(attempt, shift);  // Apply rotation to each word in the vector

    // Count how many words match the dictionary
    int matches = numWordsIn(attempt, dict);

    // Check if more than half the words match
    if (matches > attempt.size() / 2) {
      validDecryptions.push_back({shift, joinWithSpaces(attempt)});
    }
  }

  // Print all valid decryptions
  if (!validDecryptions.empty()) {
    for (const auto& decryption : validDecryptions) {
      cout << decryption.second << endl;  // Print each valid decryption
    }
  } else {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student fill this in
  string result;
  for (char c : s) {
    if (isalpha(c)) {
      result += cipher[toupper(c) - 'A'];  // Substitute letter based on cipher
    } else {
      result += c;  // Leave non-alphabetic characters unchanged
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  // TODO: student fill this in
  string text;
  cout << "Enter the text to substitution-cipher encrypt: ";
  getline(cin, text);

  // Generate random substitution cipher
  vector<char> cipher = genRandomSubstCipher();

  // Encrypt the text
  string encrypted = applySubstCipher(cipher, text);
  cout << "Encrypted text: " << encrypted << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  // TODO: student fill this in
  double totalScore = 0.0;
  for (size_t i = 0; i + 3 < s.size(); ++i) {
    string quadgram = s.substr(i, 4);  // Get the quadgram
    totalScore += scorer.getScore(quadgram);
  }
  return totalScore;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  // TODO: student fill this in
  string text;
  cout << "Enter a string to score: ";
  getline(cin, text);

  // Clean input and compute score
  string cleanedText = clean(text);
  double score = scoreString(scorer, cleanedText);
  cout << "Englishness score: " << score << endl;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  // TODO: student fill this in
  vector<char> bestCipher = genRandomSubstCipher();
  string bestDecryption = applySubstCipher(bestCipher, ciphertext);
  double bestScore = scoreString(scorer, clean(bestDecryption));

  // hill climb 20 times
  for (int i = 0; i < 20; i++) {
    vector<char> currentCipher = bestCipher;
    double currentBestScore = bestScore;
    int noImprovementCount = 0;

    while (noImprovementCount < 1500) {
      int index1 = Random::randInt(25);
      int index2;
      do {
        index2 = Random::randInt(25);
      } while (index1 == index2);

      swap(currentCipher[index1], currentCipher[index2]);

      string currentDecryption = applySubstCipher(currentCipher, ciphertext);
      double currentScore = scoreString(scorer, clean(currentDecryption));

      // Better Score
      if (currentScore > currentBestScore) {
        currentBestScore = currentScore;
        bestCipher = currentCipher;
        noImprovementCount = 0;
      } else {
        // Revert the swap if it didn't improve the score
        swap(currentCipher[index1], currentCipher[index2]);
        noImprovementCount++;
      }
    }
  }
  return bestCipher;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  string ciphertext;
  cout << "Enter text to substitution-cipher decrypt: ";
  getline(cin, ciphertext);

  // Decrypt the text
  vector<char> bestCipher = decryptSubstCipher(scorer, ciphertext);
  string decrypted = applySubstCipher(bestCipher, ciphertext);
  cout << "Decrypted text: " << decrypted << endl;
}

void decryptSubstCipherFileCommand(const QuadgramScorer& scorer) {
  string text;
  string inputFilename, outputFilename;

  // Prompt for input and output filenames
  cout << "Enter input filename (ciphertext file): ";
  getline(cin, inputFilename);
  cout << "Enter output filename (plaintext file): ";
  getline(cin, outputFilename);

  // Open the input file for reading
  ifstream inputFile(inputFilename);
  if (!inputFile) {
    cout << "Error: Could not open input file." << endl;
    return;
  }

  // Read the ciphertext from the input file
  stringstream buffer;
  buffer << inputFile.rdbuf();  // Read entire file content into stringstream
  string ciphertext = buffer.str();

  // Decrypt the text using the substitution cipher decryption function
  vector<char> bestCipher = decryptSubstCipher(scorer, ciphertext);
  string decrypted = applySubstCipher(bestCipher, ciphertext);

  // Open the output file for writing
  ofstream outputFile(outputFilename);
  if (!outputFile) {
    cout << "Error: Could not open output file." << endl;
    return;
  }

  // Write the decrypted text to the output file
  outputFile << decrypted;

  cout << "Decrypted text has been written to " << outputFilename << endl;
}

#pragma endregion SubstDec
