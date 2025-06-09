#pragma once
#include <string>
#include <memory>
#include <chrono>

class Wallet;

enum class TransactionType {
    TRANSFER,
    DEPOSIT,
    WITHDRAW
};

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED,
    CANCELLED
};

class Transaction {
private:
    std::string id;
    std::shared_ptr<Wallet> source_wallet;
    std::shared_ptr<Wallet> destination_wallet;
    double amount;
    TransactionType type;
    TransactionStatus status;
    std::chrono::system_clock::time_point timestamp;
    std::string description;
    std::string otp_code;
    bool is_otp_verified;

public:
    Transaction(std::shared_ptr<Wallet> source, std::shared_ptr<Wallet> dest, 
                double amount, TransactionType type = TransactionType::TRANSFER);
    
    // Getters
    std::string getId() const { return id; }
    std::shared_ptr<Wallet> getSourceWallet() const { return source_wallet; }
    std::shared_ptr<Wallet> getDestinationWallet() const { return destination_wallet; }
    double getAmount() const { return amount; }
    TransactionType getType() const { return type; }
    TransactionStatus getStatus() const { return status; }
    std::chrono::system_clock::time_point getTimestamp() const { return timestamp; }
    std::string getDescription() const { return description; }
    bool isOtpVerified() const { return is_otp_verified; }
    
    // Setters
    void setStatus(TransactionStatus new_status) { status = new_status; }
    void setDescription(const std::string& desc) { description = desc; }
    void setOtpCode(const std::string& otp) { otp_code = otp; }
    void setOtpVerified(bool verified) { is_otp_verified = verified; }
    
    // Transaction methods
    bool execute();
    bool cancel();
    bool verifyOtp(const std::string& otp);
    
    // Serialization
    std::string serialize() const;
    static std::shared_ptr<Transaction> deserialize(const std::string& data);
}; 