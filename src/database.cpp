#include "database.h"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <stdexcept>
#include <iostream>

Database::Database(const std::string& dir) : data_dir(dir) {
    try {
        std::filesystem::create_directories(data_dir);
        loadData();
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to initialize database: " + std::string(e.what()));
    }
}

void Database::loadData() {
    try {
        // Load users
        std::ifstream user_file(data_dir + "/users.txt");
        if (!user_file.is_open()) {
            std::cout << "Warning: Could not open users file. Starting with empty database.\n";
            return;
        }
        std::string line;
        while (std::getline(user_file, line)) {
            if (!line.empty()) {
                try {
                    auto user = User::deserialize(line);
                    users[user->getUsername()] = user;
                } catch (const std::exception& e) {
                    std::cout << "Warning: Failed to load user: " << e.what() << "\n";
                }
            }
        }
        
        // Load wallets
        std::ifstream wallet_file(data_dir + "/wallets.txt");
        if (!wallet_file.is_open()) {
            std::cout << "Warning: Could not open wallets file.\n";
            return;
        }
        while (std::getline(wallet_file, line)) {
            if (!line.empty()) {
                try {
                    auto wallet = Wallet::deserialize(line);
                    wallets[wallet->getId()] = wallet;
                } catch (const std::exception& e) {
                    std::cout << "Warning: Failed to load wallet: " << e.what() << "\n";
                }
            }
        }
        
        // Load transactions
        std::ifstream transaction_file(data_dir + "/transactions.txt");
        if (!transaction_file.is_open()) {
            std::cout << "Warning: Could not open transactions file.\n";
            return;
        }
        while (std::getline(transaction_file, line)) {
            if (!line.empty()) {
                try {
                    auto transaction = Transaction::deserialize(line);
                    transactions[transaction->getId()] = transaction;
                } catch (const std::exception& e) {
                    std::cout << "Warning: Failed to load transaction: " << e.what() << "\n";
                }
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load data: " + std::string(e.what()));
    }
}

void Database::saveData() {
    try {
        // Save users
        std::ofstream user_file(data_dir + "/users.txt");
        if (!user_file.is_open()) {
            throw std::runtime_error("Could not open users file for writing");
        }
        for (const auto& [username, user] : users) {
            user_file << user->serialize() << std::endl;
        }
        
        // Save wallets
        std::ofstream wallet_file(data_dir + "/wallets.txt");
        if (!wallet_file.is_open()) {
            throw std::runtime_error("Could not open wallets file for writing");
        }
        for (const auto& [id, wallet] : wallets) {
            wallet_file << wallet->serialize() << std::endl;
        }
        
        // Save transactions
        std::ofstream transaction_file(data_dir + "/transactions.txt");
        if (!transaction_file.is_open()) {
            throw std::runtime_error("Could not open transactions file for writing");
        }
        for (const auto& [id, transaction] : transactions) {
            transaction_file << transaction->serialize() << std::endl;
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save data: " + std::string(e.what()));
    }
}

bool Database::addUser(std::shared_ptr<User> user) {
    if (users.find(user->getUsername()) != users.end()) {
        return false;
    }
    users[user->getUsername()] = user;
    saveData();
    return true;
}

std::shared_ptr<User> Database::getUser(const std::string& username) {
    auto it = users.find(username);
    return it != users.end() ? it->second : nullptr;
}

bool Database::updateUser(std::shared_ptr<User> user) {
    if (users.find(user->getUsername()) == users.end()) {
        return false;
    }
    users[user->getUsername()] = user;
    saveData();
    return true;
}

bool Database::addWallet(std::shared_ptr<Wallet> wallet) {
    if (wallets.find(wallet->getId()) != wallets.end()) {
        return false;
    }
    wallets[wallet->getId()] = wallet;
    saveData();
    return true;
}

std::shared_ptr<Wallet> Database::getWallet(const std::string& wallet_id) {
    auto it = wallets.find(wallet_id);
    return it != wallets.end() ? it->second : nullptr;
}

bool Database::updateWallet(std::shared_ptr<Wallet> wallet) {
    if (wallets.find(wallet->getId()) == wallets.end()) {
        return false;
    }
    wallets[wallet->getId()] = wallet;
    saveData();
    return true;
}

bool Database::addTransaction(std::shared_ptr<Transaction> transaction) {
    if (transactions.find(transaction->getId()) != transactions.end()) {
        return false;
    }
    transactions[transaction->getId()] = transaction;
    saveData();
    return true;
}

std::shared_ptr<Transaction> Database::getTransaction(const std::string& transaction_id) {
    auto it = transactions.find(transaction_id);
    return it != transactions.end() ? it->second : nullptr;
}

bool Database::backup() {
    try {
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << data_dir << "/backup_" << timestamp;
        std::string backup_dir = ss.str();
        
        if (!std::filesystem::create_directories(backup_dir)) {
            throw std::runtime_error("Failed to create backup directory");
        }
        
        // Copy all data files to backup directory
        for (const auto& file : {"users.txt", "wallets.txt", "transactions.txt"}) {
            std::string source = data_dir + "/" + file;
            std::string dest = backup_dir + "/" + file;
            
            if (!std::filesystem::exists(source)) {
                std::cout << "Warning: Source file " << file << " does not exist\n";
                continue;
            }
            
            std::filesystem::copy_file(source, dest, 
                std::filesystem::copy_options::overwrite_existing);
        }
        
        std::cout << "Backup created successfully at: " << backup_dir << "\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Backup failed: " << e.what() << "\n";
        return false;
    }
}

bool Database::restore(const std::string& backup_file) {
    try {
        if (!std::filesystem::exists(backup_file)) {
            std::cout << "Backup directory does not exist: " << backup_file << "\n";
            return false;
        }
        
        // Verify backup files exist
        for (const auto& file : {"users.txt", "wallets.txt", "transactions.txt"}) {
            std::string backup_path = backup_file + "/" + file;
            if (!std::filesystem::exists(backup_path)) {
                std::cout << "Backup file missing: " << file << "\n";
                return false;
            }
        }
        
        // Create temporary directory for verification
        std::string temp_dir = data_dir + "/temp_restore";
        std::filesystem::create_directories(temp_dir);
        
        // Copy backup files to temporary directory
        for (const auto& file : {"users.txt", "wallets.txt", "transactions.txt"}) {
            std::string source = backup_file + "/" + file;
            std::string dest = temp_dir + "/" + file;
            std::filesystem::copy_file(source, dest);
        }
        
        // Try to load data from temporary directory
        Database temp_db(temp_dir);
        
        // If successful, copy to main directory
        for (const auto& file : {"users.txt", "wallets.txt", "transactions.txt"}) {
            std::string source = temp_dir + "/" + file;
            std::string dest = data_dir + "/" + file;
            std::filesystem::copy_file(source, dest, 
                std::filesystem::copy_options::overwrite_existing);
        }
        
        // Clean up temporary directory
        std::filesystem::remove_all(temp_dir);
        
        // Reload data
        loadData();
        std::cout << "Restore completed successfully\n";
        return true;
    } catch (const std::exception& e) {
        std::cout << "Restore failed: " << e.what() << "\n";
        return false;
    }
} 