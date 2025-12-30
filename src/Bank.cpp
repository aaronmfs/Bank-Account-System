#include "Bank.h"
#include <fstream>
#include <sstream>
#include <iomanip>

Bank::Bank() : nextAccountNumber(1001) {
    loadAccounts();
}

void Bank::loadAccounts() {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
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
            
            if (accNum >= nextAccountNumber) {
                nextAccountNumber = accNum + 1;
            }
        }
    }
    file.close();
}

void Bank::saveAccounts() {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (const auto& acc : accounts) {
        file << acc.toFileString() << std::endl;
    }
    file.close();
}

Account* Bank::findAccount(int accNum) {
    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == accNum) {
            return &acc;
        }
    }
    return nullptr;
}

// Returns JSON response
std::string Bank::createAccountAPI(const std::string& name, double initialDeposit, const std::string& pin) {
    if (initialDeposit < 0) {
        return R"({"success": false, "message": "Invalid initial deposit amount"})";
    }
    
    if (pin.length() != 4) {
        return R"({"success": false, "message": "PIN must be 4 digits"})";
    }

    accounts.push_back(Account(nextAccountNumber, name, initialDeposit, pin));
    int newAccNum = nextAccountNumber;
    nextAccountNumber++;
    saveAccounts();
    
    std::ostringstream json;
    json << R"({"success": true, "message": "Account created successfully!", "accountNumber": )" 
         << newAccNum << "}";
    return json.str();
}

std::string Bank::depositAPI(int accNum, const std::string& pin, double amount) {
    Account* acc = findAccount(accNum);
    if (!acc) {
        return R"({"success": false, "message": "Account not found"})";
    }
    
    if (!acc->checkPin(pin)) {
        return R"({"success": false, "message": "Incorrect PIN"})";
    }
    
    if (acc->deposit(amount)) {
        saveAccounts();
        std::ostringstream json;
        json << R"({"success": true, "message": "Deposit successful!", "balance": )" 
             << std::fixed << std::setprecision(2) << acc->getBalance() << "}";
        return json.str();
    }
    
    return R"({"success": false, "message": "Invalid amount"})";
}

std::string Bank::withdrawAPI(int accNum, const std::string& pin, double amount) {
    Account* acc = findAccount(accNum);
    if (!acc) {
        return R"({"success": false, "message": "Account not found"})";
    }
    
    if (!acc->checkPin(pin)) {
        return R"({"success": false, "message": "Incorrect PIN"})";
    }
    
    if (acc->withdraw(amount)) {
        saveAccounts();
        std::ostringstream json;
        json << R"({"success": true, "message": "Withdrawal successful!", "balance": )" 
             << std::fixed << std::setprecision(2) << acc->getBalance() << "}";
        return json.str();
    }
    
    return R"({"success": false, "message": "Insufficient funds or invalid amount"})";
}

std::string Bank::checkBalanceAPI(int accNum, const std::string& pin) {
    Account* acc = findAccount(accNum);
    if (!acc) {
        return R"({"success": false, "message": "Account not found"})";
    }
    
    if (!acc->checkPin(pin)) {
        return R"({"success": false, "message": "Incorrect PIN"})";
    }
    
    std::ostringstream json;
    json << R"({"success": true, "accountNumber": )" << acc->getAccountNumber()
         << R"(, "name": ")" << acc->getName() << R"(")"
         << R"(, "balance": )" << std::fixed << std::setprecision(2) << acc->getBalance() << "}";
    return json.str();
}

std::string Bank::transferAPI(int fromAccNum, const std::string& pin, int toAccNum, double amount) {
    Account* fromAcc = findAccount(fromAccNum);
    if (!fromAcc) {
        return R"({"success": false, "message": "Your account not found"})";
    }
    
    if (!fromAcc->checkPin(pin)) {
        return R"({"success": false, "message": "Incorrect PIN"})";
    }
    
    Account* toAcc = findAccount(toAccNum);
    if (!toAcc) {
        return R"({"success": false, "message": "Recipient account not found"})";
    }
    
    if (fromAcc->withdraw(amount)) {
        toAcc->deposit(amount);
        saveAccounts();
        std::ostringstream json;
        json << R"({"success": true, "message": "Transfer successful!", "balance": )" 
             << std::fixed << std::setprecision(2) << fromAcc->getBalance() << "}";
        return json.str();
    }
    
    return R"({"success": false, "message": "Transfer failed - insufficient funds or invalid amount"})";
}
