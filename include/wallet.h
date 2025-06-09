#pragma once

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>

class Transaction;

class Wallet {
private:
    std::string id;
    double balance;
    std::vector<std::shared_ptr<Transaction>> transactions;
    double daily_transfer_limit;
    double max_balance;
    std::chrono::system_clock::time_point last_transfer_time;
    int daily_transfer_count;
    mutable std::mutex mutex;

public:
    Wallet(const std::string& id);
    
    // Getters
    std::string getId() const { return id; }
    double getBalance() const { return balance; }
    const std::vector<std::shared_ptr<Transaction>>& getTransactionHistory() const { return transactions; }
    double getDailyTransferLimit() const { return daily_transfer_limit; }
    double getMaxBalance() const { return max_balance; }
    int getDailyTransferCount() const { return daily_transfer_count; }
    
    // Setters
    void setDailyTransferLimit(double limit) { daily_transfer_limit = limit; }
    void setMaxBalance(double max) { max_balance = max; }
    
    // Transaction methods
    bool transfer(std::shared_ptr<Wallet> dest_wallet, double amount);
    bool deposit(double amount);
    bool withdraw(double amount);
    void addTransaction(std::shared_ptr<Transaction> transaction);
    
    // Validation methods
    bool canTransfer(double amount) const;
    bool isDailyLimitExceeded() const;
    void resetDailyTransferCount();
    
    // Serialization
    std::string serialize() const;
    static std::shared_ptr<Wallet> deserialize(const std::string& data);
}; 