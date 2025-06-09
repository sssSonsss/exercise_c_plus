#include "otp.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <iostream>

OTP::OTP(const std::string& email) : user_email(email) {
    code = generateOTP();
    expiry_time = std::chrono::system_clock::now() + std::chrono::minutes(5);
}

std::string OTP::generateOTP() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);
    
    std::stringstream ss;
    ss << std::setw(6) << std::setfill('0') << dis(gen);
    return ss.str();
}

bool OTP::isValid() const {
    return std::chrono::system_clock::now() < expiry_time;
}

bool OTP::verifyOTP(const std::string& input_code) const {
    return isValid() && input_code == code;
}

bool OTP::sendOTP() const {
    // In a real implementation, this would send an email
    // For now, we'll just print the OTP to the console
    std::cout << "OTP for " << user_email << ": " << code << std::endl;
    return true;
} 