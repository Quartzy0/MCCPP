#ifndef TESTING1_ENCRYPTION_H
#define TESTING1_ENCRYPTION_H

#include <openssl/x509.h>

#include <cstring>
#include <vector>

namespace encryption{
    EVP_CIPHER_CTX* ctx_encrypt;
    EVP_CIPHER_CTX* ctx_decrypt;

    uint8_t key[16];
    uint8_t iv[16];

    size_t blockSize;

    void init(uint8_t* keyIn, uint8_t* ivIn){
        std::memcpy(key, keyIn, 16);
        std::memcpy(iv, ivIn, 16);

        ctx_encrypt = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx_encrypt, EVP_aes_128_cfb8(), nullptr, key, iv);

        ctx_decrypt = EVP_CIPHER_CTX_new();
        EVP_DecryptInit_ex(ctx_decrypt, EVP_aes_128_cfb8(), nullptr, key, iv);

        blockSize = EVP_CIPHER_block_size(EVP_aes_128_cfb8());
    }

    std::vector<uint8_t> encrypt(uint8_t* data, size_t size){
        std::vector<uint8_t> out;
        out.resize(size + blockSize - 1llu);
        int outlen;
        if (!EVP_EncryptUpdate(ctx_encrypt, out.data(), &outlen, data, size)){
            fprintf(stderr, "OpenSSLError when encrypting");
        }
        out.resize(outlen);

        return out;
    }

    std::vector<uint8_t> decrypt(uint8_t* data, size_t size){
        std::vector<uint8_t> out;
        out.resize(size + blockSize - 1llu);
        int outlen;
        if (!EVP_DecryptUpdate(ctx_decrypt, out.data(), &outlen, data, size)){
            fprintf(stderr, "OpenSSLError when decrypting");
        }
        out.resize(outlen);

        return out;
    }
}

#endif