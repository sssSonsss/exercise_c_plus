#include "wallet.h"
#include "transaction.h"
#include <sstream>
#include <random>
#include <chrono>
#include <stdexcept>
#include <ctime>

Wallet::Wallet(const std::string& id)
    : id(id), balance(0), daily_transfer_limit(1000000),
      max_balance(10000000), daily_transfer_count(0) {
    last_transfer_time = std::chrono::system_clock::now();
    if (id.empty()) {
        throw std::invalid_argument("Wallet ID cannot be empty");
    }
}

bool Wallet::canTransfer(double amount) const {
    if (amount <= 0) return false;
    if (amount > balance) return false;
    if (amount > daily_transfer_limit) return false;
    if (isDailyLimitExceeded()) return false;
    return true;
}

bool Wallet::isDailyLimitExceeded() const {
    auto now = std::chrono::system_clock::now();
    auto last_transfer = std::chrono::system_clock::to_time_t(last_transfer_time);
    auto current_time = std::chrono::system_clock::to_time_t(now);
    
    // Reset daily count if it's a new day
    if (std::difftime(current_time, last_transfer) >= 86400) { // 24 hours
        const_cast<Wallet*>(this)->resetDailyTransferCount();
        return false;
    }
    
    return daily_transfer_count >= 10; // Maximum 10 transfers per day
}

void Wallet::resetDailyTransferCount() {
    daily_transfer_count = 0;
    last_transfer_time = std::chrono::system_clock::now();
}

bool Wallet::transfer(std::shared_ptr<Wallet> dest_wallet, double amount) {
    if (!dest_wallet) return false;
    
    std::lock_guard<std::mutex> lock(mutex);
    
    if (!canTransfer(amount)) {
        return false;
    }
    
    if (dest_wallet->balance + amount > dest_wallet->max_balance) {
        return false;
    }
    
    balance -= amount;
    dest_wallet->balance += amount;
    daily_transfer_count++;
    last_transfer_time = std::chrono::system_clock::now();
    
    return true;
}

bool Wallet::deposit(double amount) {
    if (amount <= 0) return false;
    
    std::lock_guard<std::mutex> lock(mutex);
    
    if (balance + amount > max_balance) return false;
    
    balance += amount;
    return true;
}

bool Wallet::withdraw(double amount) {
    if (amount <= 0) return false;
    
    std::lock_guard<std::mutex> lock(mutex);
    
    if (amount > balance) return false;
    
    balance -= amount;
    return true;
}

void Wallet::addTransaction(std::shared_ptr<Transaction> transaction) {
    if (!transaction) return;
    
    std::lock_guard<std::mutex> lock(mutex);
    transactions.push_back(transaction);
}

std::string Wallet::serialize() const {
    std::stringstream ss;
    ss << id << "|" << balance << "|" << daily_transfer_limit << "|"
       << max_balance << "|" << daily_transfer_count << "|"
       << std::chrono::system_clock::to_time_t(last_transfer_time);
    return ss.str();
}

std::shared_ptr<Wallet> Wallet::deserialize(const std::string& data) {
    if (data.empty()) {
        throw std::invalid_argument("Cannot deserialize empty data");
    }
    
    std::stringstream ss(data);
    std::string id, balance_str, limit_str, max_str, count_str, time_str;
    
    std::getline(ss, id, '|');
    std::getline(ss, balance_str, '|');
    std::getline(ss, limit_str, '|');
    std::getline(ss, max_str, '|');
    std::getline(ss, count_str, '|');
    std::getline(ss, time_str, '|');
    
    auto wallet = std::make_shared<Wallet>(id);
    wallet->balance = std::stod(balance_str);
    wallet->daily_transfer_limit = std::stod(limit_str);
    wallet->max_balance = std::stod(max_str);
    wallet->daily_transfer_count = std::stoi(count_str);
    wallet->last_transfer_time = std::chrono::system_clock::from_time_t(std::stoll(time_str));
    
    return wallet;
} 