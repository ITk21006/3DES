#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <random>

// Forward declaration of applyPermutation
std::string applyPermutation(const std::string &data, const int *table, int tableSize);

// Hardcoded keys for 3DES
const std::string key1 = "12345678"; // 8 bytes
const std::string key2 = "23456789"; // 8 bytes
const std::string key3 = "34567890"; // 8 bytes

// DES tables
const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int IP_INV[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

const int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

const int SHIFTS[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

const int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

const int S_BOX[8][4][16] = {
    // S1
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    // S2
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    // S3
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    // S4
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    // S5
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    // S6
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    // S7
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    // S8
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

const int P[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

// Function to convert string to hex
std::string toHex(const std::string &input) {
    std::ostringstream oss;
    for (unsigned char c : input) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}

// Function to convert hex to string
std::string fromHex(const std::string &input) {
    std::string output;
    for (size_t i = 0; i < input.length(); i += 2) {
        std::string byteString = input.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
        output += byte;
    }
    return output;
}

// Function to generate a random IV
std::string generateIV(size_t length) {
    std::string iv;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    for (size_t i = 0; i < length; ++i) {
        iv += static_cast<char>(dis(gen));
    }
    return iv;
}

// Placeholder for DES encryption function
std::string DES_encrypt(const std::string &data, const std::string &key) {
    // Apply initial permutation
    std::string permutedData = applyPermutation(data, IP, 64);

    // Split into left and right halves
    std::string left = permutedData.substr(0, 4);
    std::string right = permutedData.substr(4, 4);

    // Generate subkeys
    std::vector<std::string> subkeys = generateSubkeys(key);

    // 16 rounds of processing
    for (int i = 0; i < 16; ++i) {
        std::string expandedRight = applyExpansion(right, E);
        std::string xored = xorStrings(expandedRight, subkeys[i]);
        std::string substituted = applySBoxes(xored);
        std::string permuted = applyPermutation(substituted, P, 32);
        std::string newRight = xorStrings(left, permuted);
        left = right;
        right = newRight;
    }

    // Combine and apply final permutation
    std::string combined = right + left;
    return applyPermutation(combined, IP_INV, 64);
}

// Placeholder for DES decryption function
std::string DES_decrypt(const std::string &data, const std::string &key) {
    // Similar to DES_encrypt but with subkeys applied in reverse order
}

// XOR two strings
std::string xorStrings(const std::string &a, const std::string &b) {
    std::string result;
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] ^ b[i];
    }
    return result;
}

// Triple DES encryption with CBC mode
std::string tripleDES_encrypt(const std::string &data, const std::string &iv) {
    std::string previousBlock = iv;
    std::string encryptedData;
    for (size_t i = 0; i < data.size(); i += 8) {
        std::string block = data.substr(i, 8);
        block = xorStrings(block, previousBlock);
        block = DES_encrypt(block, key1);
        block = DES_decrypt(block, key2);
        block = DES_encrypt(block, key3);
        encryptedData += block;
        previousBlock = block;
    }
    return encryptedData;
}

// Triple DES decryption with CBC mode
std::string tripleDES_decrypt(const std::string &data, const std::string &iv) {
    std::string previousBlock = iv;
    std::string decryptedData;
    for (size_t i = 0; i < data.size(); i += 8) {
        std::string block = data.substr(i, 8);
        std::string decryptedBlock = DES_decrypt(block, key3);
        decryptedBlock = DES_encrypt(decryptedBlock, key2);
        decryptedBlock = DES_decrypt(decryptedBlock, key1);
        decryptedBlock = xorStrings(decryptedBlock, previousBlock);
        decryptedData += decryptedBlock;
        previousBlock = block;
    }
    return decryptedData;
}

// Function to handle text input
void processText(bool encrypt) {
    std::string input;
    std::cout << "Enter text: ";
    std::cin.ignore();
    std::getline(std::cin, input);

    if (encrypt) {
        std::string iv = generateIV(8); // 8 bytes for DES
        std::string encrypted = tripleDES_encrypt(input, iv);
        std::cout << "IV (hex): " << toHex(iv) << std::endl;
        std::cout << "Encrypted text (hex): " << toHex(encrypted) << std::endl;
    } else {
        std::string ivHex;
        std::cout << "Enter IV (hex): ";
        std::cin >> ivHex;
        std::string iv = fromHex(ivHex);
        std::string decrypted = tripleDES_decrypt(fromHex(input), iv);
        std::cout << "Decrypted text: " << decrypted << std::endl;
    }
}

// Function to handle file input
void processFile(bool encrypt) {
    std::string filename;
    std::cout << "Enter filename: ";
    std::cin >> filename;

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (encrypt) {
        std::string iv = generateIV(8); // 8 bytes for DES
        std::string encrypted = tripleDES_encrypt(data, iv);
        std::cout << "IV (hex): " << toHex(iv) << std::endl;
        std::cout << "Encrypted file content (hex): " << toHex(encrypted) << std::endl;

        // Save encrypted data to a new file
        std::ofstream outFile("encrypted_file.txt", std::ios::binary);
        if (outFile.is_open()) {
            std::string combinedHex = toHex(iv) + toHex(encrypted);
            outFile.write(combinedHex.c_str(), combinedHex.size()); // Write as binary
            outFile.close();
            std::cout << "Encrypted data saved to encrypted_file.txt" << std::endl;
        } else {
            std::cerr << "Error creating output file!" << std::endl;
        }
    } else {
        std::string ivHex;
        std::cout << "Enter IV (hex): ";
        std::cin >> ivHex;
        std::string iv = fromHex(ivHex);

        // Read the entire file content as a single string
        std::ifstream inFile("encrypted_file.txt", std::ios::binary);
        std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();

        // Extract the encrypted data from the file content
        std::string encryptedHex = fileContent.substr(ivHex.length());
        std::string decrypted = tripleDES_decrypt(fromHex(encryptedHex), iv);
        std::cout << "Decrypted file content: " << decrypted << std::endl;
    }
}

// Function to apply a permutation table to a string
std::string applyPermutation(const std::string &data, const int *table, int tableSize) {
    std::string permutedData;
    for (int i = 0; i < tableSize; ++i) {
        permutedData += data[table[i] - 1];
    }
    return permutedData;
}

// Function to generate 16 subkeys from the main key
std::vector<std::string> generateSubkeys(const std::string &key) {
    std::vector<std::string> subkeys;
    std::string permutedKey = applyPermutation(key, PC1, 56);

    std::string left = permutedKey.substr(0, 28);
    std::string right = permutedKey.substr(28, 28);

    for (int i = 0; i < 16; ++i) {
        // Perform left circular shifts
        left = leftShift(left, SHIFTS[i]);
        right = leftShift(right, SHIFTS[i]);

        // Combine halves and apply PC2 permutation
        std::string combined = left + right;
        subkeys.push_back(applyPermutation(combined, PC2, 48));
    }

    return subkeys;
}

// Function to perform left circular shift
std::string leftShift(const std::string &input, int shifts) {
    return input.substr(shifts) + input.substr(0, shifts);
}

// Function to apply the expansion table to a string
std::string applyExpansion(const std::string &data, const int *expansionTable) {
    std::string expandedData;
    for (int i = 0; i < 48; ++i) { // Assuming the expansion table size is 48
        expandedData += data[expansionTable[i] - 1];
    }
    return expandedData;
}

// Function to apply S-Boxes to a 48-bit input
std::string applySBoxes(const std::string &input) {
    std::string output;
    for (int i = 0; i < 8; ++i) {
        std::string sixBits = input.substr(i * 6, 6);
        int row = (sixBits[0] - '0') * 2 + (sixBits[5] - '0');
        int col = (sixBits[1] - '0') * 8 + (sixBits[2] - '0') * 4 + (sixBits[3] - '0') * 2 + (sixBits[4] - '0');
        int sBoxValue = S_BOX[i][row][col];
        output += static_cast<char>(sBoxValue);
    }
    return output;
}

int main() {
    char choice, type;
    std::cout << "Enter 'e' to encrypt or 'd' to decrypt: ";
    std::cin >> choice;
    std::cout << "Enter 't' for text or 'f' for file: ";
    std::cin >> type;

    bool encrypt = (choice == 'e');
    if (type == 't') {
        processText(encrypt);
    } else if (type == 'f') {
        processFile(encrypt);
    } else {
        std::cerr << "Invalid input type!" << std::endl;
    }

    return 0;
}