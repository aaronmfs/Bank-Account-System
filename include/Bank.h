#ifndef BANK_H
#define BANK_H

#include "Account.h"
#include <vector>
#include <string>

class Bank {
private:
    std::vector<Account> accounts;
    int nextAccountNumber;
    const std::string filename = "accounts.txt";
    
    void loadAccounts();
    void saveAccounts();
    Account* findAccount(int accNum);

public:
    Bank();
    
    // API methods that return JSON
    std::string createAccountAPI(const std::string& name, double initialDeposit, const std::string& pin);
    std::string depositAPI(int accNum, const std::string& pin, double amount);
    std::string withdrawAPI(int accNum, const std::string& pin, double amount);
    std::string checkBalanceAPI(int accNum, const std::string& pin);
    std::string transferAPI(int fromAccNum, const std::string& pin, int toAccNum, double amount);
};

#endif
