#include "filemanager/filemanager.h"
#include <fstream>
#include <sstream>

FileManager::FileManager(const std::string &file) : filename(file) {}

// Get user by account ID
User *FileManager::getUser(const std::string &accountId) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, email, pass, name, bal;

        std::getline(ss, id, ',');
        std::getline(ss, email, ',');
        std::getline(ss, pass, ',');
        std::getline(ss, name, ',');
        std::getline(ss, bal, ',');

        if (id == accountId) {
            User *user = new User();
            user->accountId = id;
            user->email = email;
            user->password = pass;
            user->fullname = name;
            user->balance = bal;
            return user;
        }
    }
    return nullptr;
}

// Update balance in file
bool FileManager::updateBalance(const std::string &accountId, const std::string &newBalance) {
    std::ifstream in(filename);
    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string id, rest;
        std::getline(ss, id, ',');

        if (id == accountId) {
            std::getline(ss, rest);
            size_t lastComma = rest.rfind(',');
            lines.push_back(id + "," + rest.substr(0, lastComma + 1) + newBalance);
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    in.close();

    if (!found)
        return false;

    std::ofstream out(filename);
    for (const auto &l : lines)
        out << l << "\n";
    return true;
}

// Deposit money
bool FileManager::deposit(const std::string &accountId, double amount) {
    User *user = getUser(accountId);
    if (!user || amount <= 0)
        return false;

    double newBalance = std::stod(user->balance) + amount;
    delete user;
    return updateBalance(accountId, std::to_string(newBalance));
}

// Withdraw money
bool FileManager::withdraw(const std::string &accountId, double amount) {
    User *user = getUser(accountId);
    if (!user || amount <= 0)
        return false;

    double currentBalance = std::stod(user->balance);
    if (currentBalance < amount) {
        delete user;
        return false;
    }

    double newBalance = currentBalance - amount;
    delete user;
    return updateBalance(accountId, std::to_string(newBalance));
}

// Transfer money
bool FileManager::transfer(const std::string &from, const std::string &to, double amount) {
    User *fromUser = getUser(from);
    User *toUser = getUser(to);

    if (!fromUser || !toUser || amount <= 0)
        return false;

    double fromBalance = std::stod(fromUser->balance);
    if (fromBalance < amount) {
        delete fromUser;
        delete toUser;
        return false;
    }

    updateBalance(from, std::to_string(fromBalance - amount));
    updateBalance(to, std::to_string(std::stod(toUser->balance) + amount));

    delete fromUser;
    delete toUser;
    return true;
}
