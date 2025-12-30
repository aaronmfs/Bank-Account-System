#include "Account.h"

Account::Account(int accNum, std::string n, double bal, std::string p)
    : accountNumber(accNum), name(n), balance(bal), pin(p) {}

int Account::getAccountNumber() const { return accountNumber; }
std::string Account::getName() const { return name; }
double Account::getBalance() const { return balance; }

bool Account::checkPin(const std::string& inputPin) const {
    return pin == inputPin;
}

bool Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        return true;
    }
    return false;
}

bool Account::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}

std::string Account::toFileString() const {
    return std::to_string(accountNumber) + "," + name + "," + 
           std::to_string(balance) + "," + pin;
}
