#include "transaction.h"
#include "wallet.h"
#include <sstream>
#include <random>
#include <iomanip>

Transaction::Transaction(std::shared_ptr<Wallet> source, std::shared_ptr<Wallet> dest, 
                       double amount, TransactionType type)
    : source_wallet(source), destination_wallet(dest), amount(amount),
      type(type), status(TransactionStatus::PENDING),
      timestamp(std::chrono::system_clock::now()),
      is_otp_verified(false) {
    
    if (!source) {
        throw std::invalid_argument("Source wallet cannot be null");
    }
    
    if (!dest) {
        throw std::invalid_argument("Destination wallet cannot be null");
    }
    
    if (amount <= 0) {
        throw std::invalid_argument("Transaction amount must be positive");
    }
    
    if (source->getId() == dest->getId()) {
        throw std::invalid_argument("Source and destination wallets must be different");
    }
    
    // Generate unique transaction ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    const char* hex = "0123456789abcdef";
    std::string uuid;
    for(int i = 0; i < 32; i++) {
        uuid += hex[dis(gen)];
    }
    id = uuid;
}

bool Transaction::execute() {
    if (status != TransactionStatus::PENDING) {
        return false;
    }
    
    if (!is_otp_verified) {
        return false;
    }
    
    bool success = false;
    switch (type) {
        case TransactionType::TRANSFER:
            success = source_wallet->transfer(destination_wallet, amount);
            break;
        case TransactionType::DEPOSIT:
            success = destination_wallet->deposit(amount);
            break;
        case TransactionType::WITHDRAW:
            success = source_wallet->withdraw(amount);
            break;
    }
    
    if (success) {
        status = TransactionStatus::COMPLETED;
    } else {
        status = TransactionStatus::FAILED;
    }
    
    return success;
}

bool Transaction::cancel() {
    if (status != TransactionStatus::PENDING) {
        return false;
    }
    
    status = TransactionStatus::CANCELLED;
    return true;
}

bool Transaction::verifyOtp(const std::string& otp) {
    if (otp.empty() || otp_code.empty()) {
        return false;
    }
    
    is_otp_verified = (otp == otp_code);
    return is_otp_verified;
}

std::string Transaction::serialize() const {
    std::stringstream ss;
    ss << id << "|" << source_wallet->getId() << "|" 
       << (destination_wallet ? destination_wallet->getId() : "") << "|"
       << amount << "|" << static_cast<int>(type) << "|"
       << static_cast<int>(status) << "|"
       << std::chrono::system_clock::to_time_t(timestamp) << "|"
       << description << "|" << otp_code << "|"
       << (is_otp_verified ? "1" : "0");
    return ss.str();
}

std::shared_ptr<Transaction> Transaction::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string id, source_id, dest_id, amount_str, type_str, status_str,
                timestamp_str, description, otp_code, verified_str;
    
    std::getline(ss, id, '|');
    std::getline(ss, source_id, '|');
    std::getline(ss, dest_id, '|');
    std::getline(ss, amount_str, '|');
    std::getline(ss, type_str, '|');
    std::getline(ss, status_str, '|');
    std::getline(ss, timestamp_str, '|');
    std::getline(ss, description, '|');
    std::getline(ss, otp_code, '|');
    std::getline(ss, verified_str, '|');
    
    // Create source wallet
    auto source_wallet = std::make_shared<Wallet>(source_id);
    
    // Create destination wallet if exists
    std::shared_ptr<Wallet> dest_wallet;
    if (!dest_id.empty()) {
        dest_wallet = std::make_shared<Wallet>(dest_id);
    }
    
    auto transaction = std::make_shared<Transaction>(
        source_wallet, dest_wallet, std::stod(amount_str),
        static_cast<TransactionType>(std::stoi(type_str))
    );
    
    transaction->id = id;
    transaction->status = static_cast<TransactionStatus>(std::stoi(status_str));
    transaction->timestamp = std::chrono::system_clock::from_time_t(std::stoll(timestamp_str));
    transaction->description = description;
    transaction->otp_code = otp_code;
    transaction->is_otp_verified = verified_str == "1";
    
    return transaction;
} 