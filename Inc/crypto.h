#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <openssl/sha.h>

// Function to convert bytes to hex string
std::string toHexString(const unsigned char* hash, std::size_t length) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (std::size_t i = 0; i < length; ++i) {
        ss << std::setw(2) << static_cast<unsigned int>(hash[i]);
    }
    return ss.str();
}

// Function to convert some string in SHA256 hash
std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, input.c_str(), input.size());
    SHA256_Final(hash, &sha256_ctx);
    return toHexString(hash, SHA256_DIGEST_LENGTH);
}

