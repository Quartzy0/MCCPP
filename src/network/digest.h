#ifndef TESTING1_DIGEST_H
#define TESTING1_DIGEST_H

/*
 *
 *
 *         Code from: https://gist.github.com/madmongo1/53c303c6fe8de64f93adc014c7671d51
 *
 *
*/


/*
 * HEADER FILE
 */

//
// Copyright (c) 2020 Richard Hodges (hodges.r@gmail.com)
//
// Official repository: https://github.com/AlexAndDad/gateway
//

#include <openssl/sha.h>
#include <string>

struct daft_hash_impl
{
    daft_hash_impl()
            : ctx_ {}
    {
        SHA1_Init(&ctx_);
    }

    daft_hash_impl(daft_hash_impl const &) = delete;
    daft_hash_impl(daft_hash_impl &&)      = delete;
    daft_hash_impl &operator=(daft_hash_impl const &) = delete;
    daft_hash_impl &operator=(daft_hash_impl &&) = delete;
    ~daft_hash_impl() {}

    void update(std::string in) { SHA1_Update(&ctx_, in.data(), in.size()); }
    void update(unsigned char* data, size_t size) { SHA1_Update(&ctx_, data, size); }
    void update(char* data, size_t size) { SHA1_Update(&ctx_, data, size); }
    void update(const unsigned char* data, size_t size) { SHA1_Update(&ctx_, data, size); }

    std::string finalise();

private:
    SHA_CTX ctx_;
};

/*
 * CPP FILE
 */

//
// Copyright (c) 2020 Richard Hodges (hodges.r@gmail.com)
//
// Official repository: https://github.com/AlexAndDad/gateway
//

#include <algorithm>
#include <array>
#include <string_view>
#include <openssl/bn.h>

std::string daft_hash_impl::finalise()
{
    auto result = std::string();

    auto buf = std::array< std::uint8_t, 20 >();
    SHA1_Final(buf.data(), &ctx_);

    // convert has to bignum
    BIGNUM *bn = BN_bin2bn(buf.data(), buf.size(), nullptr);

    // reset the hasher for next use
    SHA1_Init(&ctx_);

    // check for "negative" value
    if (BN_is_bit_set(bn, 159))
    {
        result += '-';

        // perform 1's compliment on the bignum's bits
        auto tmp = std::vector< unsigned char >(BN_num_bytes(bn));
        BN_bn2bin(bn, tmp.data());
        std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char b) { return ~b; });
        BN_bin2bn(tmp.data(), tmp.size(), bn);

        // add 1 "as-if" 2's compliment

        BN_add_word(bn, 1);
    }

    // convert to hex
    auto hex = BN_bn2hex(bn);

    // remove any leading zeroes except the last
    auto view = std::string_view(hex);
    while (view.size() && view[0] == '0')
        view = view.substr(1);

    // append the hex to the result
    result.append(view.begin(), view.end());
    OPENSSL_free(hex);
    BN_free(bn);

    // convert the hex to lower case
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}


#endif //TESTING1_DIGEST_H
