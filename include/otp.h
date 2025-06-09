#ifndef OTP_H
#define OTP_H

#include <string>
#include <chrono>

class OTP {
private:
    std::string code;
    std::chrono::system_clock::time_point expiry_time;
    std::string user_email;

public:
    OTP(const std::string& email);
    
    // Getters
    std::string getCode() const { return code; }
    bool isValid() const;
    
    // OTP generation and verification
    static std::string generateOTP();
    bool verifyOTP(const std::string& input_code) const;
    
    // Email sending
    bool sendOTP() const;
};

#endif // OTP_H 