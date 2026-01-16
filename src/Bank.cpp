#include "Bank.h"
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

std::string getTimestamp() {
    time_t now = time(0);
    char buf[80];
#ifdef _MSC_VER
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
#else
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
#endif
    return std::string(buf);
}

Bank::Bank() : nextAccountNumber(1001) {
    std::cout << "[" << getTimestamp() << "] Bank system initialized" << std::endl;
    loadAccounts();
}

void Bank::loadAccounts() {
    std::cout << "[" << getTimestamp() << "] Loading accounts from file..." << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "[" << getTimestamp() << "] No existing accounts file found" << std::endl;
        return;
    }

    std::string line;
    int count = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> tokens;

        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() == 4) {
            int accNum = std::stoi(tokens[0]);
            std::string name = tokens[1];
            double balance = std::stod(tokens[2]);
            std::string pin = tokens[3];

            accounts.push_back(Account(accNum, name, balance, pin));
            count++;

            if (accNum >= nextAccountNumber) {
                nextAccountNumber = accNum + 1;
            }
        }
    }
    file.close();
    std::cout << "[" << getTimestamp() << "] Loaded " << count << " accounts successfully" << std::endl;
}

void Bank::saveAccounts() {
    std::cout << "[" << getTimestamp() << "] Saving accounts to file..." << std::endl;
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "[" << getTimestamp() << "] ERROR: Failed to open file for saving" << std::endl;
        return;
    }

    for (const auto &acc : accounts) {
        file << acc.toFileString() << std::endl;
    }
    file.close();
    std::cout << "[" << getTimestamp() << "] Accounts saved successfully" << std::endl;
}

Account *Bank::findAccount(int accNum) {
    for (auto &acc : accounts) {
        if (acc.getAccountNumber() == accNum) {
            return &acc;
        }
    }
    return nullptr;
}

std::string Bank::createAccountAPI(const std::string &name, double initialDeposit, const std::string &pin) {
    std::cout << "[" << getTimestamp() << "] CREATE ACCOUNT - Attempt by: " << name << ", Initial Deposit: $" << std::fixed << std::setprecision(2)
              << initialDeposit << std::endl;

    if (initialDeposit < 0) {
        std::cout << "[" << getTimestamp() << "] CREATE ACCOUNT - FAILED: Invalid deposit amount" << std::endl;
        return R"({"success": false, "message": "Invalid initial deposit amount"})";
    }

    if (pin.length() != 4) {
        std::cout << "[" << getTimestamp() << "] CREATE ACCOUNT - FAILED: Invalid PIN length" << std::endl;
        return R"({"success": false, "message": "PIN must be 4 digits"})";
    }

    accounts.push_back(Account(nextAccountNumber, name, initialDeposit, pin));
    int newAccNum = nextAccountNumber;
    nextAccountNumber++;
    saveAccounts();

    std::cout << "[" << getTimestamp() << "] CREATE ACCOUNT - SUCCESS: Account #" << newAccNum << " created for " << name << std::endl;

    std::ostringstream json;
    json << R"({"success": true, "message": "Account created successfully!", "accountNumber": )" << newAccNum << "}";
    return json.str();
}

std::string Bank::depositAPI(int accNum, const std::string &pin, double amount) {
    std::cout << "[" << getTimestamp() << "] DEPOSIT - Account #" << accNum << ", Amount: $" << std::fixed << std::setprecision(2) << amount << std::endl;

    Account *acc = findAccount(accNum);
    if (!acc) {
        std::cout << "[" << getTimestamp() << "] DEPOSIT - FAILED: Account not found" << std::endl;
        return R"({"success": false, "message": "Account not found"})";
    }

    if (!acc->checkPin(pin)) {
        std::cout << "[" << getTimestamp() << "] DEPOSIT - FAILED: Incorrect PIN for Account #" << accNum << std::endl;
        return R"({"success": false, "message": "Incorrect PIN"})";
    }

    if (acc->deposit(amount)) {
        saveAccounts();
        std::cout << "[" << getTimestamp() << "] DEPOSIT - SUCCESS: Account #" << accNum << ", New Balance: $" << std::fixed << std::setprecision(2)
                  << acc->getBalance() << std::endl;
        std::ostringstream json;
        json << R"({"success": true, "message": "Deposit successful!", "balance": )" << std::fixed << std::setprecision(2) << acc->getBalance() << "}";
        return json.str();
    }

    std::cout << "[" << getTimestamp() << "] DEPOSIT - FAILED: Invalid amount" << std::endl;
    return R"({"success": false, "message": "Invalid amount"})";
}

std::string Bank::withdrawAPI(int accNum, const std::string &pin, double amount) {
    std::cout << "[" << getTimestamp() << "] WITHDRAW - Account #" << accNum << ", Amount: $" << std::fixed << std::setprecision(2) << amount << std::endl;

    Account *acc = findAccount(accNum);
    if (!acc) {
        std::cout << "[" << getTimestamp() << "] WITHDRAW - FAILED: Account not found" << std::endl;
        return R"({"success": false, "message": "Account not found"})";
    }

    if (!acc->checkPin(pin)) {
        std::cout << "[" << getTimestamp() << "] WITHDRAW - FAILED: Incorrect PIN for Account #" << accNum << std::endl;
        return R"({"success": false, "message": "Incorrect PIN"})";
    }

    if (acc->withdraw(amount)) {
        saveAccounts();
        std::cout << "[" << getTimestamp() << "] WITHDRAW - SUCCESS: Account #" << accNum << ", New Balance: $" << std::fixed << std::setprecision(2)
                  << acc->getBalance() << std::endl;
        std::ostringstream json;
        json << R"({"success": true, "message": "Withdrawal successful!", "balance": )" << std::fixed << std::setprecision(2) << acc->getBalance() << "}";
        return json.str();
    }

    std::cout << "[" << getTimestamp() << "] WITHDRAW - FAILED: Insufficient funds or invalid amount" << std::endl;
    return R"({"success": false, "message": "Insufficient funds or invalid amount"})";
}

std::string Bank::checkBalanceAPI(int accNum, const std::string &pin) {
    std::cout << "[" << getTimestamp() << "] CHECK BALANCE - Account #" << accNum << std::endl;

    Account *acc = findAccount(accNum);
    if (!acc) {
        std::cout << "[" << getTimestamp() << "] CHECK BALANCE - FAILED: Account not found" << std::endl;
        return R"({"success": false, "message": "Account not found"})";
    }

    if (!acc->checkPin(pin)) {
        std::cout << "[" << getTimestamp() << "] CHECK BALANCE - FAILED: Incorrect PIN for Account #" << accNum << std::endl;
        return R"({"success": false, "message": "Incorrect PIN"})";
    }

    std::cout << "[" << getTimestamp() << "] CHECK BALANCE - SUCCESS: Account #" << accNum << ", Balance: $" << std::fixed << std::setprecision(2)
              << acc->getBalance() << std::endl;

    std::ostringstream json;
    json << R"({"success": true, "accountNumber": )" << acc->getAccountNumber() << R"(, "name": ")" << acc->getName() << R"(")"
         << R"(, "balance": )" << std::fixed << std::setprecision(2) << acc->getBalance() << "}";
    return json.str();
}

std::string Bank::transferAPI(int fromAccNum, const std::string &pin, int toAccNum, double amount) {
    std::cout << "[" << getTimestamp() << "] TRANSFER - From Account #" << fromAccNum << " to Account #" << toAccNum << ", Amount: $" << std::fixed
              << std::setprecision(2) << amount << std::endl;

    Account *fromAcc = findAccount(fromAccNum);
    if (!fromAcc) {
        std::cout << "[" << getTimestamp() << "] TRANSFER - FAILED: Source account not found" << std::endl;
        return R"({"success": false, "message": "Your account not found"})";
    }

    if (!fromAcc->checkPin(pin)) {
        std::cout << "[" << getTimestamp() << "] TRANSFER - FAILED: Incorrect PIN for Account #" << fromAccNum << std::endl;
        return R"({"success": false, "message": "Incorrect PIN"})";
    }

    Account *toAcc = findAccount(toAccNum);
    if (!toAcc) {
        std::cout << "[" << getTimestamp() << "] TRANSFER - FAILED: Recipient account not found" << std::endl;
        return R"({"success": false, "message": "Recipient account not found"})";
    }

    if (fromAcc->withdraw(amount)) {
        toAcc->deposit(amount);
        saveAccounts();
        std::cout << "[" << getTimestamp() << "] TRANSFER - SUCCESS: $" << std::fixed << std::setprecision(2) << amount << " transferred from #" << fromAccNum
                  << " to #" << toAccNum << std::endl;
        std::cout << "[" << getTimestamp() << "] TRANSFER - Account #" << fromAccNum << " New Balance: $" << std::fixed << std::setprecision(2)
                  << fromAcc->getBalance() << std::endl;
        std::ostringstream json;
        json << R"({"success": true, "message": "Transfer successful!", "balance": )" << std::fixed << std::setprecision(2) << fromAcc->getBalance() << "}";
        return json.str();
    }

    std::cout << "[" << getTimestamp() << "] TRANSFER - FAILED: Insufficient funds or invalid amount" << std::endl;
    return R"({"success": false, "message": "Transfer failed - insufficient funds or invalid amount"})";
}
