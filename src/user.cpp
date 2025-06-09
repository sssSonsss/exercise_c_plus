#include "user.h"
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

User::User(const std::string& username, const std::string& password, 
           const std::string& email, bool is_admin)
    : username(username), email(email), is_admin(is_admin), 
      is_auto_generated_password(false), login_attempts(0),
      is_locked(false), is_email_verified(false) {
    // Hash password using OpenSSL EVP
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, password.c_str(), password.length());
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);
    
    std::stringstream ss;
    for(unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    password_hash = ss.str();
    
    // Generate wallet ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    const char* hex = "0123456789abcdef";
    std::string uuid;
    for(int i = 0; i < 32; i++) {
        uuid += hex[dis(gen)];
    }
    wallet_id = uuid;
}

void User::incrementLoginAttempts() {
    login_attempts++;
    if (login_attempts >= 5) {
        lockAccount();
    }
}

void User::resetLoginAttempts() {
    login_attempts = 0;
}

void User::lockAccount() {
    is_locked = true;
    lock_time = std::chrono::system_clock::now();
}

void User::unlockAccount() {
    is_locked = false;
    login_attempts = 0;
}

bool User::isAccountLocked() const {
    if (!is_locked) return false;
    
    // Auto unlock after 30 minutes
    auto now = std::chrono::system_clock::now();
    auto lock_duration = std::chrono::duration_cast<std::chrono::minutes>(now - lock_time);
    if (lock_duration.count() >= 30) {
        const_cast<User*>(this)->unlockAccount();
        return false;
    }
    return true;
}

bool User::verifyPassword(const std::string& password) const {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, password.c_str(), password.length());
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);
    
    std::stringstream ss;
    for(unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str() == password_hash;
}

void User::changePassword(const std::string& new_password) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, new_password.c_str(), new_password.length());
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);
    
    std::stringstream ss;
    for(unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    password_hash = ss.str();
    is_auto_generated_password = false;
}

std::string User::serialize() const {
    std::stringstream ss;
    ss << username << "|" << password_hash << "|" << email << "|" 
       << (is_admin ? "1" : "0") << "|" << (is_auto_generated_password ? "1" : "0") 
       << "|" << wallet_id << "|" << full_name << "|" << phone << "|" 
       << address << "|" << login_attempts << "|" << (is_locked ? "1" : "0") 
       << "|" << std::chrono::system_clock::to_time_t(lock_time) 
       << "|" << (is_email_verified ? "1" : "0");
    return ss.str();
}

std::shared_ptr<User> User::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string username, password_hash, email, is_admin_str, is_auto_str, 
                wallet_id, full_name, phone, address, login_attempts_str,
                is_locked_str, lock_time_str, is_email_verified_str;
    
    std::getline(ss, username, '|');
    std::getline(ss, password_hash, '|');
    std::getline(ss, email, '|');
    std::getline(ss, is_admin_str, '|');
    std::getline(ss, is_auto_str, '|');
    std::getline(ss, wallet_id, '|');
    std::getline(ss, full_name, '|');
    std::getline(ss, phone, '|');
    std::getline(ss, address, '|');
    std::getline(ss, login_attempts_str, '|');
    std::getline(ss, is_locked_str, '|');
    std::getline(ss, lock_time_str, '|');
    std::getline(ss, is_email_verified_str, '|');
    
    auto user = std::make_shared<User>(username, "", email, is_admin_str == "1");
    user->password_hash = password_hash;
    user->is_auto_generated_password = is_auto_str == "1";
    user->wallet_id = wallet_id;
    user->full_name = full_name;
    user->phone = phone;
    user->address = address;
    user->login_attempts = std::stoi(login_attempts_str);
    user->is_locked = is_locked_str == "1";
    user->lock_time = std::chrono::system_clock::from_time_t(std::stoll(lock_time_str));
    user->is_email_verified = is_email_verified_str == "1";
    
    return user;
} 