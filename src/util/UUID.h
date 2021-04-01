//
// Created by quartzy on 3/29/21.
//

#ifndef MCCPP_UUID_H
#define MCCPP_UUID_H

#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>

class UUID{
private:
    uint64_t mostSigBits, leastSigBits;
public:
    UUID(const uint64_t &mostSigBits, const uint64_t &leastSigBits) : mostSigBits(mostSigBits),
                                                                      leastSigBits(leastSigBits) {}

    uint64_t getMostSigBits() const {
        return mostSigBits;
    }

    uint64_t getLeastSigBits() const {
        return leastSigBits;
    }

    bool operator== (const UUID& uuid) const{
        return uuid.mostSigBits == mostSigBits && uuid.leastSigBits == leastSigBits;
    }
};

#endif //MCCPP_UUID_H
