#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <memory>
#include <unordered_map>
#include "user.h"
#include "wallet.h"
#include "transaction.h"

class Database {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    std::unordered_map<std::string, std::shared_ptr<Wallet>> wallets;
    std::unordered_map<std::string, std::shared_ptr<Transaction>> transactions;
    
    std::string data_dir;
    void loadData();
    void saveData();

public:
    Database(const std::string& dir = "data");
    
    // User management
    bool addUser(std::shared_ptr<User> user);
    std::shared_ptr<User> getUser(const std::string& username);
    bool updateUser(std::shared_ptr<User> user);
    
    // Wallet management
    bool addWallet(std::shared_ptr<Wallet> wallet);
    std::shared_ptr<Wallet> getWallet(const std::string& wallet_id);
    bool updateWallet(std::shared_ptr<Wallet> wallet);
    
    // Transaction management
    bool addTransaction(std::shared_ptr<Transaction> transaction);
    std::shared_ptr<Transaction> getTransaction(const std::string& transaction_id);
    
    // Backup and restore
    bool backup();
    bool restore(const std::string& backup_file);
};

#endif // DATABASE_H 