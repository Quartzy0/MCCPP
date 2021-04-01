//
// Created by quartzy on 3/13/21.
//

#include <string>
#include "PacketEncryptionResponse.h"

#include <openssl/pem.h>
#include <openssl/aes.h>

uint32_t network::PacketEncryptionResponse::getPacketId() {
    return 1;
}

uint32_t network::PacketEncryptionResponse::encode(uint8_t *out) {
    long l = (long) publicKey.size();
    const unsigned char* data1 = reinterpret_cast<const unsigned char *>(publicKey.data());
    RSA* rsa = d2i_RSA_PUBKEY(NULL, &data1, l);

    int rsaSize = RSA_size(rsa);

    std::string encryptedSS;
    std::string encryptedToken;

    encryptedSS.resize(rsaSize);
    encryptedToken.resize(rsaSize);

    // Encrypt the shared secret with public key
    RSA_public_encrypt(AES_BLOCK_SIZE, ss.data(), (unsigned char*)&encryptedSS[0], rsa, RSA_PKCS1_PADDING);

    // Encrypt the verify token with public key
    RSA_public_encrypt(verifyToken.size(), (const unsigned char*)verifyToken.data(), (unsigned char*)&encryptedToken[0], rsa, RSA_PKCS1_PADDING);

    RSA_free(rsa);


    int i = 0;

    i+=encodeVarInt(encryptedSS.length(), out + i);
    std::memcpy(out + i, encryptedSS.data(), encryptedSS.length());
    i+=encryptedSS.length();

    i+=encodeVarInt(encryptedToken.length(), out + i);
    std::memcpy(out + i, encryptedToken.data(), encryptedToken.length());
    i+=encryptedToken.length();

    return i;
}

uint32_t network::PacketEncryptionResponse::size() {
    return varIntLen(128) + 128 + varIntLen(128) + 128;
}

network::PacketEncryptionResponse::PacketEncryptionResponse(uint32_t ssLen, const std::vector<uint8_t> &ss,
                                                            uint32_t verifyTokenLen,
                                                            const std::vector<uint8_t> &verifyToken,
                                                            const std::vector<uint8_t> &publicKey) : ssLen(ssLen),
                                                                                                     ss(ss),
                                                                                                     verifyTokenLen(
                                                                                                             verifyTokenLen),
                                                                                                     verifyToken(
                                                                                                             verifyToken),
                                                                                                     publicKey(
                                                                                                             publicKey) {}

void network::PacketEncryptionResponse::decode(uint8_t *in, size_t size) {

}

void network::PacketEncryptionResponse::handlePacket() {

}
