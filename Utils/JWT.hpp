#pragma once

#include <string>
#include <map>

class JWT
{
public:
    struct Payload
    {
        std::string uuid;
        std::string apikey;
        uint64_t validUntil;
    };

    static bool ValidateToken(const std::string &token, const std::string &secret, Payload &outPayload);
};
