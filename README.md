This project implements basic encryption and decryption algorithms using C++, focusing on the Caesar Cipher and Substitution Cipher.  It provides a solid introduction to working with strings, vectors, files, and algorithms in C++.
The project includes multiple tasks, each designed to demonstrate core features of C++ and the use of algorithmic problem-solving to encrypt and decrypt text. The goal is to apply function decomposition and write clear, well-structured code using minimal libraries.

1. Caesar Cipher Encoder
The Caesar cipher is a simple encryption technique where each letter in the plaintext is shifted forward by a certain number of positions in the alphabet. This part of the project implements two utility functions:

Rotate a character by a given amount: This function rotates a single character within the alphabet.
Rotate a string by a given amount: This function processes an entire string, removing non-alphabetic characters and rotating each letter.
The runCaesarEncrypt() helper function prompts the user for input text and a shift amount, applies the Caesar cipher, and outputs the encrypted text.

2. Caesar Cipher Decoder
This part of the project decrypts Caesar cipher text by using brute force to try all possible shifts. The approach is as follows:

Load a dictionary of words from a file.
For each possible shift, apply the Caesar cipher and check how many words in the result are valid English words using the dictionary.
Output the best decryption based on this comparison.
3. Substitution Cipher Encoder
A more complex cipher where each letter in the plaintext is replaced with a corresponding letter from a randomly shuffled alphabet. This task implements the following:

Random substitution cipher generation: A random mapping between letters is created.
Encrypt text using substitution: Each letter in the plaintext is replaced with the corresponding letter from the substitution cipher.
The program converts all letters to uppercase and leaves non-alphabetic characters unchanged.

4. Substitution Cipher Decoder (Hill-Climbing Algorithm)
Decrypting a substitution cipher is significantly more challenging because the mapping between letters is arbitrary. To solve this, the project implements a hill-climbing algorithm, which:

Starts with a random substitution cipher.
Performs random swaps between letters in the cipher and scores each resulting decryption based on its "English-ness".
Optimizes the cipher by choosing higher-scoring decryptions.
The scoring is based on the frequency of quadgrams (four-letter sequences) in the English language, as calculated from a precompiled list in english_quadgrams.txt. The algorithm runs multiple trials to escape local maxima and find the best possible decryption.

5. Substitution Cipher Decryption (File Input)
In addition to decrypting text from console input, the project also supports reading from files:

The user specifies an input file containing encrypted text and an output file.
The program decrypts the input file's contents using the hill-climbing algorithm and writes the decrypted text to the output file.
