#include <iostream> // for input/output
#include <string> // for strings
#include <vector> // for vectors
#include <sstream> // for string streams
#include <iomanip> // for input/output manipulation
#include <cstdlib> // for standard library functions
#include <ctime> // for time functions
#include <cstdint> // for fixed-width integer types
#include <fstream> // for files

//tables
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
const int S[8][4][16] = {
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

std::vector<uint8_t> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << "\n";
        exit(1);
    }
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return data;
}
void writeFile(const std::string& filename, const std::vector<uint8_t>& data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error writing to file: " << filename << "\n";
        exit(1);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

//for IV generation
uint64_t generateRandomBlock() {
    uint64_t block = 0;
    for (int i = 0; i < 8; ++i) {
        block = (block << 8) | (rand() % 256);
    }
    return block;
}

//permutation
uint64_t permute(uint64_t input, const int* table, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; ++i) {
        output = (output << 1) | ((input >> (64 - table[i])) & 1);
    }
    return output;
}

//left shift
uint32_t leftShift(uint32_t value, int shifts) {
    return ((value << shifts) | (value >> (28 - shifts))) & 0x0FFFFFFF;
}

// Generate 16 subkeys from a 64-bit key
void generateSubkeys(uint64_t key, uint64_t subkeys[16]) {
    uint64_t permutedKey = permute(key, PC1, 56);
    uint32_t C = (permutedKey >> 28) & 0x0FFFFFFF;
    uint32_t D = permutedKey & 0x0FFFFFFF;

    for (int i = 0; i < 16; ++i) {
        C = leftShift(C, SHIFTS[i]);
        D = leftShift(D, SHIFTS[i]);
        uint64_t combined = (static_cast<uint64_t>(C) << 28) | D;
        subkeys[i] = permute(combined, PC2, 48);
    }
}

// Apply a single round of DES
uint64_t desRound(uint64_t block, uint64_t subkey) {
    uint32_t left = block >> 32;
    uint32_t right = block & 0xFFFFFFFF;

    uint64_t expanded = permute(right, E, 48);
    uint64_t xored = expanded ^ subkey;

    //apply S-Boxes
    uint32_t sboxOutput = 0;
    for (int i = 0; i < 8; ++i) {
        int row = ((xored >> (42 - i * 6)) & 0x20) | ((xored >> (42 - i * 6)) & 0x01);
        int col = (xored >> (43 - i * 6)) & 0x0F;
        sboxOutput = (sboxOutput << 4) | S[i][row][col];
    }

    //apply P-Box
    uint32_t permuted = permute(sboxOutput, P, 32);
    return (static_cast<uint64_t>(right) << 32) | (left ^ permuted);
}

//DES encryption/decryption
uint64_t des(uint64_t block, uint64_t key, bool decrypt) {
    uint64_t subkeys[16];
    generateSubkeys(key, subkeys);

    block = permute(block, IP, 64);
    for (int i = 0; i < 16; ++i) {
        block = desRound(block, subkeys[decrypt ? 15 - i : i]);
    }
    block = (block << 32) | (block >> 32); // Swap halves
    return permute(block, IP_INV, 64);
}

//PKCS5 padding
std::vector<uint8_t> pad(const std::vector<uint8_t>& data, size_t blockSize) {
    size_t paddingSize = blockSize - (data.size() % blockSize);
    std::vector<uint8_t> padded = data;
    padded.insert(padded.end(), paddingSize, static_cast<uint8_t>(paddingSize));
    return padded;
}

//unpadding
std::vector<uint8_t> unpad(const std::vector<uint8_t>& data) {
    size_t paddingSize = data.back();
    return std::vector<uint8_t>(data.begin(), data.end() - paddingSize);
}

//convert text to bytes
std::vector<uint8_t> textToBytes(const std::string& text) {
    return std::vector<uint8_t>(text.begin(), text.end());
}

//convert bytes to text
std::string bytesToText(const std::vector<uint8_t>& bytes) {
    return std::string(bytes.begin(), bytes.end());
}

//encrypt using 3DES (CBC mode)
std::vector<uint8_t> encrypt3DES(const std::vector<uint8_t>& plaintext, uint64_t key1, uint64_t key2, uint64_t iv) {
    std::vector<uint8_t> ciphertext;
    uint64_t previousBlock = iv;

    for (size_t i = 0; i < plaintext.size(); i += 8) {
        //extract 8-byte block from plaintext
        uint64_t block = 0;
        for (size_t j = 0; j < 8 && i + j < plaintext.size(); ++j) {
            block = (block << 8) | plaintext[i + j];
        }

        //apply CBC mode (XOR with the previous ciphertext block)
        block ^= previousBlock;

        //encrypt using two key 3DES
        block = des(block, key1, false); //encrypt with key1
        block = des(block, key2, true);  //decrypt with key2
        block = des(block, key1, false); //encrypt with key1 again

        //append encrypted block to ciphertext
        for (int j = 7; j >= 0; --j) {
            ciphertext.push_back((block >> (j * 8)) & 0xFF);
        }

        //update previous block for next iteration
        previousBlock = block;
    }

    return ciphertext;
}

//decrypt using 3DES (CBC mode)
std::vector<uint8_t> decrypt3DES(const std::vector<uint8_t>& ciphertext, uint64_t key1, uint64_t key2, uint64_t iv) {
    std::vector<uint8_t> plaintext;
    uint64_t previousBlock = iv;

    for (size_t i = 0; i < ciphertext.size(); i += 8) {
        //extract 8-byte block from plaintext
        uint64_t block = 0;
        for (size_t j = 0; j < 8 && i + j < ciphertext.size(); ++j) {
            block = (block << 8) | ciphertext[i + j];
        }

        //decrypt using two key 3DES
        uint64_t decryptedBlock = des(block, key1, true);  //decrypt with key1
        decryptedBlock = des(decryptedBlock, key2, false); //encrypt with key2
        decryptedBlock = des(decryptedBlock, key1, true);  //decrypt with key1 again

        //apply CBC mode (XOR with the previous ciphertext block)
        decryptedBlock ^= previousBlock;

        //append decrypted block to plaintext
        for (int j = 7; j >= 0; --j) {
            plaintext.push_back((decryptedBlock >> (j * 8)) & 0xFF);
        }

        //update previous block for next iteration
        previousBlock = block;
    }

    return plaintext;
}


int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    char mode;
    while (true) {
        std::cout << "Select mode - (e) Encrypt, (d) Decrypt\n";
        std::cout << "Enter mode: ";
        std::cin >> mode;
        if (mode == 'e' || mode == 'd') break;
        std::cerr << "Invalid mode selected. Please enter 'e' for Encrypt or 'd' for Decrypt.\n";
    }
    std::cin.ignore(); //ignore newline after mode input

    char inputType;
    while (true) {
        std::cout << "Select input type - (t) Text, (f) File\n";
        std::cout << "Enter mode: ";
        std::cin >> inputType;
        if (inputType == 't' || inputType == 'f') break;
        std::cerr << "Invalid input type selected. Please enter 't' for Text or 'f' for File.\n";
    }
    std::cin.ignore(); //ignore newline after input type

    std::string key1Hex;
    while (true) {
        std::cout << "Enter Key 1 (16 hex characters): ";
        std::getline(std::cin, key1Hex);
        if (key1Hex.length() == 16) break;
        std::cerr << "Key 1 must be 16 hexadecimal characters!\n";
    }
    uint64_t key1 = std::stoull(key1Hex, nullptr, 16);

    std::string key2Hex;
    while (true) {
        std::cout << "Enter Key 2 (16 hex characters): ";
        std::getline(std::cin, key2Hex);
        if (key2Hex.length() == 16) break;
        std::cerr << "Key 2 must be 16 hexadecimal characters!\n";
    }
    uint64_t key2 = std::stoull(key2Hex, nullptr, 16);



    if (mode == 'e') {
        //encryption mode
        if (inputType == 't') {
            //text input
            std::cout << "Enter plaintext: ";
            std::string plaintext;
            std::getline(std::cin, plaintext);

            //generate random IV
            uint64_t iv = generateRandomBlock();

            //encrypt
            auto paddedData = pad(textToBytes(plaintext), 8);
            auto ciphertext = encrypt3DES(paddedData, key1, key2, iv);

            //output
            std::cout << "Ciphertext (Hex): ";
            for (auto byte : ciphertext) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
            }
            std::cout << "\nIV (Hex): " << std::hex << iv << "\n";
        
        } else if (inputType == 'f') {
            //file input
            std::cout << "Enter absolute path to file: ";
            std::string filename;
            std::getline(std::cin, filename);

            //read file data
            std::vector<uint8_t> data = readFile(filename);

            //generate random IV
            uint64_t iv = generateRandomBlock();

            //encrypt
            auto encryptedData = encrypt3DES(data, key1, key2, iv);

            //output encrypted data to a file
            std::string encryptedFilename = filename + ".enc";
            writeFile(encryptedFilename, encryptedData);

            //output IV
            std::cout << "Encrypted file: " << encryptedFilename << "\n";
            std::cout << "IV (Hex): " << std::hex << iv << "\n";
        }

    } else if (mode == 'd') {
        //decryption mode
        if (inputType == 't') {
            //text input
            std::cout << "Enter ciphertext (Hex): ";
            std::string ciphertextHex;
            std::getline(std::cin, ciphertextHex);

            //convert ciphertext from hex to binary
            std::vector<uint8_t> ciphertext;
            for (size_t i = 0; i < ciphertextHex.length(); i += 2) {
                ciphertext.push_back(static_cast<uint8_t>(std::stoi(ciphertextHex.substr(i, 2), nullptr, 16)));
            }

            //IV input
            std::string ivHex;
            while (true) {
                std::cout << "Enter IV (Hex): ";
                std::getline(std::cin, ivHex);
                if (ivHex.length() == 16) break;
                std::cerr << "IV must be 16 hexadecimal characters!\n";
            }
            uint64_t iv = std::stoull(ivHex, nullptr, 16);

            //decrypt
            auto decryptedData = decrypt3DES(ciphertext, key1, key2, iv);
            auto plaintext = bytesToText(unpad(decryptedData));

            //output decrypted text
            std::cout << "Decrypted text: " << plaintext << "\n";

        } else if (inputType == 'f') {
            //file input
            std::cout << "Enter absolute path to file: ";
            std::string encryptedFilename;
            std::getline(std::cin, encryptedFilename);

            //read encrypted file data
            std::vector<uint8_t> encryptedData = readFile(encryptedFilename);

            //IV input
            std::string ivHex;
            while (true) {
                std::cout << "Enter IV (Hex): ";
                std::getline(std::cin, ivHex);
                if (ivHex.length() == 16) break;
                std::cerr << "IV must be 16 hexadecimal characters!\n";
            }
            uint64_t iv = std::stoull(ivHex, nullptr, 16);

            //decrypt
            auto decryptedData = decrypt3DES(encryptedData, key1, key2, iv);

            //output decrypted data to a file (remove .dec.enc extensions)
            //outputted file should be in the same place where the encrypted file is
            std::string decryptedFilename = encryptedFilename + ".dec";
            writeFile(decryptedFilename, decryptedData);

            std::cout << "Decrypted file: " << decryptedFilename << "\n";
            std::cout << "Remove .enc.dec extensions before opening the file.\n";
        }
    }

    return 0;
}