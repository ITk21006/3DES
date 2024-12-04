#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

//generate playfair matrix keyword
void createPlayfairMatrix(const string &keyword, vector<vector<char>> &matrix) {
    bool used[26] = {false}; //track characters already used in matrix
    used['J' - 'A'] = true; 
    int row = 0, col = 0;

    //create matrix using keyword
    for (char ch : keyword) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            if (!used[ch - 'A']) {
                matrix[row][col] = ch;
                used[ch - 'A'] = true;
                col++;
                if (col == 5) {
                    col = 0;
                    row++;
                }
            }
        }
    }

    //fill remaining spots
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (!used[ch - 'A']) {
            matrix[row][col] = ch;
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }
}

//preprocess input text (remove non-alphabetic characters, split into pairs)
string preprocessText(const string &text) {
    string result;
    for (char ch : text) {
        if (isalpha(ch)) {
            result += toupper(ch);
        }
    }

    //if number of characters = odd, append an 'X' to last one
    if (result.length() % 2 != 0) {
        result += 'X';
    }

    return result;
}

//find position of a character in matrix
pair<int, int> findPosition(char ch, const vector<vector<char>> &matrix) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == ch) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

//encrypt plaintext
string encrypt(const string &plaintext, const vector<vector<char>> &matrix) {
    string ciphertext;

    for (size_t i = 0; i < plaintext.length(); i += 2) {
        char first = plaintext[i];
        char second = plaintext[i + 1];

        auto [r1, c1] = findPosition(first, matrix);
        auto [r2, c2] = findPosition(second, matrix);

        //rule 1: if letters are in the same row
        if (r1 == r2) {
            ciphertext += matrix[r1][(c1 + 1) % 5];
            ciphertext += matrix[r2][(c2 + 1) % 5];
        }
        //rule 2: if letters are in the same column
        else if (c1 == c2) {
            ciphertext += matrix[(r1 + 1) % 5][c1];
            ciphertext += matrix[(r2 + 1) % 5][c2];
        }
        //rule 3: if letters are neither
        else {
            ciphertext += matrix[r1][c2];
            ciphertext += matrix[r2][c1];
        }
    }

    return ciphertext;
}

//decrypt the ciphertext
string decrypt(const string &ciphertext, const vector<vector<char>> &matrix) {
    string plaintext;

    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        char first = ciphertext[i];
        char second = ciphertext[i + 1];

        auto [r1, c1] = findPosition(first, matrix);
        auto [r2, c2] = findPosition(second, matrix);

        //rule 1: if letters are in the same row
        if (r1 == r2) {
            plaintext += matrix[r1][(c1 + 4) % 5]; // Subtract 1 by adding 4 (mod 5)
            plaintext += matrix[r2][(c2 + 4) % 5];
        }
        //rule 2: if letters are in the same column
        else if (c1 == c2) {
            plaintext += matrix[(r1 + 4) % 5][c1];
            plaintext += matrix[(r2 + 4) % 5][c2];
        }
        //rule 3: if letters are neither
        else {
            plaintext += matrix[r1][c2];
            plaintext += matrix[r2][c1];
        }
    }

    return plaintext;
}

int main() {
    string keyword, text;
    int choice;

    cout << "Enter keyword: ";
    cin >> keyword;

    cout << "Choose operation:\n";
    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "Enter choice (1 or 2): ";
    cin >> choice;
    cin.ignore();

    cout << "Enter text: ";
    getline(cin, text);

    //create the cipher matrix
    vector<vector<char>> matrix(5, vector<char>(5));
    createPlayfairMatrix(keyword, matrix);

    if (choice == 1) {
        //encryption
        string preprocessedText = preprocessText(text);
        string encryptedText = encrypt(preprocessedText, matrix);
        cout << "Encrypted text: " << encryptedText << endl;
    } else if (choice == 2) {
        //decryption
        string preprocessedText = preprocessText(text);
        string decryptedText = decrypt(preprocessedText, matrix);
        cout << "Decrypted text: " << decryptedText << endl;
    } else {
        cout << "Invalid choice!" << endl;
    }

    return 0;
}
