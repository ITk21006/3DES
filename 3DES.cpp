#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <ctime>
#include <random>

// DES Tables
const int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
                  60, 52, 44, 36, 28, 20, 12, 4,
                  62, 54, 46, 38, 30, 22, 14, 6,
                  64, 56, 48, 40, 32, 24, 16, 8,
                  57, 49, 41, 33, 25, 17, 9, 1,
                  59, 51, 43, 35, 27, 19, 11, 3,
                  61, 53, 45, 37, 29, 21, 13, 5,
                  63, 55, 47, 39, 31, 23, 15, 7};

const int IP_INV[] = {40, 8, 48, 16, 56, 24, 64, 32,
                      39, 7, 47, 15, 55, 23, 63, 31,
                      38, 6, 46, 14, 54, 22, 62, 30,
                      37, 5, 45, 13, 53, 21, 61, 29,
                      36, 4, 44, 12, 52, 20, 60, 28,
                      35, 3, 43, 11, 51, 19, 59, 27,
                      34, 2, 42, 10, 50, 18, 58, 26,
                      33, 1, 41, 9, 49, 17, 57, 25};

// New DES Tables
const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9, 1,
    58, 50, 42, 34, 26, 18, 10, 2,
    59, 51, 43, 35, 27, 19, 11, 3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15, 7, 62, 54, 46, 38,
    30, 22, 14, 6, 61, 53, 45, 37,
    29, 21, 13, 5, 28, 20, 12, 4
};

const int PC2[] = {
    14, 17, 11, 24, 1, 5, 3, 28,
    15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32
};

const int SHIFTS[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

const int E[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

const int S[8][64] = {
    {
        14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
        0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
        4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
        15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
    },
    {
        15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
        3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
        0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
        13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
    },
    {
        10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
        13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
        13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
        1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
    },
    {
        7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
        13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
        10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
        3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
    },
    {
        2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
        14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
        4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
        11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
    },
    {
        12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
        10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
        9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
        4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
    },
    {
        4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
        13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
        1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
        6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
    },
    {
        13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
        1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
        7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
        2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
    }
};

const int P[] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};
class DES {
private:
    std::vector<uint64_t> subkeys;

    uint64_t initialPermutation(uint64_t block) {
        uint64_t result = 0;
        for (int i = 0; i < 64; i++) {
            result |= ((block >> (64 - IP[i])) & 0x01) << (63 - i);
        }
        return result;
    }

    uint64_t finalPermutation(uint64_t block) {
        uint64_t result = 0;
        for (int i = 0; i < 64; i++) {
            result |= ((block >> (64 - IP_INV[i])) & 0x01) << (63 - i);
        }
        return result;
    }

    uint32_t f(uint32_t R, uint64_t k) {
        // Expansion
        uint64_t expanded = 0;
        for (int i = 0; i < 48; i++) {
            expanded |= ((R >> (32 - E[i])) & 0x01) << (47 - i);
        }
        
        // XOR with key
        expanded ^= k;

        // S-box substitution
        uint32_t output = 0;
        for (int i = 0; i < 8; i++) {
            int row = ((expanded >> (47 - 6*i)) & 0x20) | ((expanded >> (47 - 6*i - 5)) & 0x01);
            int col = (expanded >> (47 - 6*i - 4)) & 0x0F;
            output |= (uint32_t)S[i][row * 16 + col] << (28 - 4*i);
        }

        // Permutation
        uint32_t result = 0;
        for (int i = 0; i < 32; i++) {
            result |= ((output >> (32 - P[i])) & 0x01) << (31 - i);
        }

        return result;
    }

    void generateSubkeys(uint64_t key) {
        // Initial key permutation
        uint64_t permuted_key = 0;
        for (int i = 0; i < 56; i++) {
            permuted_key |= ((key >> (64 - PC1[i])) & 0x01) << (55 - i);
        }

        uint32_t C = permuted_key >> 28;
        uint32_t D = permuted_key & 0x0FFFFFFF;

        subkeys.resize(16);
        
        for (int i = 0; i < 16; i++) {
            // Rotate left
            C = ((C << SHIFTS[i]) | (C >> (28 - SHIFTS[i]))) & 0x0FFFFFFF;
            D = ((D << SHIFTS[i]) | (D >> (28 - SHIFTS[i]))) & 0x0FFFFFFF;

            // Combine and permute
            uint64_t combined = ((uint64_t)C << 28) | D;
            uint64_t subkey = 0;

            for (int j = 0; j < 48; j++) {
                subkey |= ((combined >> (56 - PC2[j])) & 0x01) << (47 - j);
            }

            subkeys[i] = subkey;
        }
    }

public:
    DES(uint64_t key) {
        generateSubkeys(key);
    }

    uint64_t encrypt(uint64_t block) {
        block = initialPermutation(block);
        
        uint32_t left = block >> 32;
        uint32_t right = block & 0xFFFFFFFF;

        for (int i = 0; i < 16; i++) {
            uint32_t temp = right;
            right = left ^ f(right, subkeys[i]);
            left = temp;
        }

        uint64_t result = ((uint64_t)right << 32) | left;
        return finalPermutation(result);
    }

    uint64_t decrypt(uint64_t block) {
        block = initialPermutation(block);
        
        uint32_t left = block >> 32;
        uint32_t right = block & 0xFFFFFFFF;

        for (int i = 15; i >= 0; i--) {
            uint32_t temp = left;
            left = right ^ f(left, subkeys[i]);
            right = temp;
        }

        uint64_t result = ((uint64_t)left << 32) | right;
        return finalPermutation(result);
    }
};

class TripleDES {
private:
    DES des1, des2, des3;

public:
    TripleDES(uint64_t key1, uint64_t key2, uint64_t key3) 
        : des1(key1), des2(key2), des3(key3) {}

    uint64_t encrypt(uint64_t block) {
        return des3.encrypt(des2.decrypt(des1.encrypt(block)));
    }

    uint64_t decrypt(uint64_t block) {
        return des1.decrypt(des2.encrypt(des3.decrypt(block)));
    }
};

int main() {
    char mode, type;
    std::cout << "Encrypt or Decrypt? (e/d): ";
    std::cin >> mode;
    std::cout << "Text or File? (t/f): ";
    std::cin >> type;
    std::cin.ignore(); // Clear the newline

    if (type == 't') {
        // Hardcoded keys for testing
        uint64_t key1 = 0x133457799BBCDFF1;
        uint64_t key2 = 0x233457799BBCDFF2;
        uint64_t key3 = 0x333457799BBCDFF3;
        
        TripleDES tdes(key1, key2, key3);

        if (mode == 'e') {
            // Print concatenated secret key
            std::cout << "Secret Key (hex): ";
            for(int i = 60; i >= 0; i -= 4) {
                char hexDigit = ((key1 >> i) & 0xF);
                std::cout << char(hexDigit < 10 ? '0' + hexDigit : 'a' + (hexDigit - 10));
            }
            for(int i = 60; i >= 0; i -= 4) {
                char hexDigit = ((key2 >> i) & 0xF);
                std::cout << char(hexDigit < 10 ? '0' + hexDigit : 'a' + (hexDigit - 10));
            }
            for(int i = 60; i >= 0; i -= 4) {
                char hexDigit = ((key3 >> i) & 0xF);
                std::cout << char(hexDigit < 10 ? '0' + hexDigit : 'a' + (hexDigit - 10));
            }
            std::cout << std::endl;

            // Generate IV using time as seed
            srand(time(NULL));
            uint64_t iv = (uint64_t)rand() << 32 | rand();
            for(int i = 0; i < 8; i++) {
                iv = (iv << 8) | (rand() % 256);
            }

            std::string input;
            std::cout << "Enter text: ";
            std::getline(std::cin, input);

            // Pad input to multiple of 8 bytes
            if (input.length() % 8 != 0) {
                input.resize(input.length() + (8 - input.length() % 8), '\0');
            }

            // Output IV in hex
            std::cout << "IV (hex): ";
            for(int i = 60; i >= 0; i -= 4) {
                char hexDigit = ((iv >> i) & 0xF);
                std::cout << char(hexDigit < 10 ? '0' + hexDigit : 'a' + (hexDigit - 10));
            }
            std::cout << std::endl;

            std::cout << "Encrypted (hex): ";
            uint64_t previous_block = iv;

            // Process each 8-byte block
            for (size_t pos = 0; pos < input.length(); pos += 8) {
                uint64_t block = 0;
                for (int i = 0; i < 8; i++) {
                    block = (block << 8) | static_cast<unsigned char>(input[pos + i]);
                }

                block ^= previous_block;  // CBC mode
                uint64_t encrypted = tdes.encrypt(block);
                previous_block = encrypted;

                // Output hex
                for(int i = 60; i >= 0; i -= 4) {
                    char hexDigit = ((encrypted >> i) & 0xF);
                    std::cout << char(hexDigit < 10 ? '0' + hexDigit : 'a' + (hexDigit - 10));
                }
            }
            std::cout << std::endl;
        } else {
            // Get IV
            std::cout << "Enter IV (hex): ";
            std::string ivStr;
            std::getline(std::cin, ivStr);
            
            uint64_t iv = 0;
            for(int i = 0; i < 16; i++) {
                iv = (iv << 4) | (ivStr[i] >= 'a' ? ivStr[i] - 'a' + 10 : 
                                 ivStr[i] >= 'A' ? ivStr[i] - 'A' + 10 : 
                                 ivStr[i] - '0');
            }

            std::string input;
            std::cout << "Enter ciphertext (hex): ";
            std::getline(std::cin, input);

            std::string decryptedText;
            uint64_t previous_block = iv;

            // Process each 16-character hex block
            for (size_t pos = 0; pos < input.length(); pos += 16) {
                uint64_t block = 0;
                // Convert hex to uint64_t
                for(int i = 0; i < 16; i++) {
                    block = (block << 4) | (input[pos+i] >= 'a' ? input[pos+i] - 'a' + 10 : 
                                          input[pos+i] >= 'A' ? input[pos+i] - 'A' + 10 : 
                                          input[pos+i] - '0');
                }

                uint64_t decrypted = tdes.decrypt(block);
                decrypted ^= previous_block;  // CBC mode
                previous_block = block;
                
                // Convert to text
                for (int i = 0; i < 8; i++) {
                    char c = (decrypted >> (56 - 8 * i)) & 0xFF;
                    if (c != '\0') {
                        decryptedText += c;
                    }
                }
            }
            std::cout << "Decrypted text: " << decryptedText << std::endl;
        }
    } else if (type == 'f') {
        // Hardcoded keys for testing
        uint64_t key1 = 0x133457799BBCDFF1;
        uint64_t key2 = 0x233457799BBCDFF2;
        uint64_t key3 = 0x333457799BBCDFF3;
        
        TripleDES tdes(key1, key2, key3);

        std::string inputFile, outputFile;
        std::cout << "Enter input file path: ";
        std::cin >> inputFile;
        std::cout << "Enter output file path: ";
        std::cin >> outputFile;

        FILE* inFile = fopen(inputFile.c_str(), "rb");
        FILE* outFile = fopen(outputFile.c_str(), "wb");

        if (!inFile || !outFile) {
            std::cout << "Error opening files!" << std::endl;
            if (inFile) fclose(inFile);
            if (outFile) fclose(outFile);
            return 1;
        }

        unsigned char buffer[8];
        size_t bytesRead;
        
        while ((bytesRead = fread(buffer, 1, 8, inFile)) > 0) {
            if (bytesRead < 8) {
                for (size_t i = bytesRead; i < 8; i++) {
                    buffer[i] = 0;
                }
            }

            uint64_t block = 0;
            for (int i = 0; i < 8; i++) {
                block = (block << 8) | buffer[i];
            }

            uint64_t processed = (mode == 'e') ? tdes.encrypt(block) : tdes.decrypt(block);

            for (int i = 7; i >= 0; i--) {
                buffer[i] = processed & 0xFF;
                processed >>= 8;
            }

            fwrite(buffer, 1, 8, outFile);
        }

        fclose(inFile);
        fclose(outFile);
        std::cout << "File processed successfully!" << std::endl;
    }

    return 0;
}

