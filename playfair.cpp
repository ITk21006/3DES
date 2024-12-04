#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>

using namespace std;

//generate playfair matrix keyword
void createPlayfairMatrix(const string &keyword, vector<vector<char>> &matrix) {
    if (matrix.size() != 5 || matrix[0].size() != 5) {
        throw runtime_error("Matrix must be 5x5");
    }

    bool used[26] = {false};
    used['J' - 'A'] = true; 
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix[i][j] = ' ';
        }
    }

    int row = 0, col = 0;

    for (char ch : keyword) {
        if (isalpha(ch)) {
            ch = toupper(ch);
            if (ch == 'J') ch = 'I'; 
            if (!used[ch - 'A']) {
                if (row < 5 && col < 5) {  
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
    }

    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (!used[ch - 'A'] && ch != 'J') { 
            if (row < 5 && col < 5) { 
                matrix[row][col] = ch;
                col++;
                if (col == 5) {
                    col = 0;
                    row++;
                }
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
    // Convert J to I as per Playfair cipher rules
    if (ch == 'J') ch = 'I';
    
    ch = toupper(ch);
    
    if (matrix.size() != 5 || matrix[0].size() != 5) {
        throw runtime_error("Invalid matrix dimensions in findPosition");
    }
    
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == ch) {
                return {i, j};
            }
        }
    }
    
    throw runtime_error("Character '" + string(1, ch) + "' not found in Playfair matrix");
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
            plaintext += matrix[r1][(c1 + 4) % 5]; 
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

//read content from a file
string readFromFile(const string& filepath) {
    ifstream inFile(filepath);
    string content;
    string line;
    
    if (!inFile) {
        throw runtime_error("Unable to open input file: " + filepath + 
                          "\nPlease ensure you provide the complete file path (e.g., D:\\folder\\file.txt)");
    }
    
    while (getline(inFile, line)) {
        content += line + "\n";
    }
    
    inFile.close();
    return content;
}

//write content to a file
void writeToFile(const string& filepath, const string& content) {
    ofstream outFile(filepath);
    
    if (!outFile) {
        throw runtime_error("Unable to open output file: " + filepath + 
                          "\nPlease ensure you provide the complete file path (e.g., D:\\folder\\file.txt)");
    }
    
    outFile << content;
    outFile.close();
}

int main() {
    string keyword;
    int choice, inputMethod;
    
    vector<vector<char>> matrix(5, vector<char>(5));
    
    cout << "Enter keyword: ";
    getline(cin, keyword); 
    
    createPlayfairMatrix(keyword, matrix);
    
    cout << "Choose operation:\n";
    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "Enter choice (1 or 2): ";
    cin >> choice;
    
    cout << "\nChoose input method:\n";
    cout << "1. Console input\n";
    cout << "2. File input\n";
    cout << "Enter choice (1 or 2): ";
    cin >> inputMethod;
    cin.ignore(); 
    
    string text;
    string inputFilename, outputFilename;
    
    try {
        if (inputMethod == 1) {
            cout << "Enter text: ";
            getline(cin, text);
        } else if (inputMethod == 2) {
            cout << "Enter complete input file path (e.g., D:\\folder\\input.txt): ";
            getline(cin, inputFilename);
            cout << "Enter complete output file path (e.g., D:\\folder\\output.txt): ";
            getline(cin, outputFilename);
            
            //read the input file
            text = readFromFile(inputFilename);
        } else {
            throw runtime_error("Invalid input method selected");
        }
        
        //process text
        string result;
        if (choice == 1) {
            string preprocessedText = preprocessText(text);
            result = encrypt(preprocessedText, matrix);
            cout << "Text encrypted successfully!" << endl;
        } else if (choice == 2) {
            string preprocessedText = preprocessText(text);
            result = decrypt(preprocessedText, matrix);
            cout << "Text decrypted successfully!" << endl;
        } else {
            throw runtime_error("Invalid operation choice");
        }
        
        //output result
        if (inputMethod == 1) {
            cout << "Result: " << result << endl;
        } else {
            //write to output file
            writeToFile(outputFilename, result);
            cout << "Result written to " << outputFilename << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
