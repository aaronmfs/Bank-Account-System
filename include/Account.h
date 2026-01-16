#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
  private:
    int accountNumber;
    std::string name;
    double balance;
    std::string pin;

  public:
    Account(int accNum = 0, std::string n = "", double bal = 0.0, std::string p = "");

    int getAccountNumber() const;
    std::string getName() const;
    double getBalance() const;
    bool checkPin(const std::string &inputPin) const;
    bool deposit(double amount);
    bool withdraw(double amount);
    std::string toFileString() const;
};

#endif
