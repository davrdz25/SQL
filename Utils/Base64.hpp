#pragma once
#include <string>
#include <stdexcept>
#include <cctype>

class Base64 {
public:
    static void SetSecret(const std::string& secret);

    static std::string Encode(const std::string& input);

    static std::string Decode(const std::string& encoded);

    static std::string EncodeUrl(const std::string& input);

    static std::string DecodeUrl(const std::string& encoded);

private:
    static inline const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

    static inline const std::string base64url_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789-_";

    static inline std::string _secret{};

    static inline bool IsBase64(unsigned char c);
    static std::string EncodeBase64(const std::string& input);
    static std::string DecodeBase64(const std::string& encoded);

    static std::string EncodeBase64Url(const std::string& input);
    static std::string DecodeBase64Url(const std::string& encoded);
};
