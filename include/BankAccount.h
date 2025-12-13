#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>
#include <vector>

struct Transaction {
  std::string type;
  double amount;
  std::string timestamp;
  double balance_after;
};

class BankAccount {
public:
  BankAccount(const std::string &account_number, const std::string &owner_name,
              double initial_balance = 0.0);

  // Account operations
  bool deposit(double amount);
  bool withdraw(double amount);
  bool transfer(BankAccount &target, double amount);

  // Getters
  std::string getAccountNumber() const;
  std::string getOwnerName() const;
  double getBalance() const;
  std::vector<Transaction> getTransactionHistory() const;

  // Convert to JSON for UI
  std::string toJSON() const;

private:
  std::string m_account_number;
  std::string m_owner_name;
  double m_balance;
  std::vector<Transaction> m_transactions;

  void addTransaction(const std::string &type, double amount);
  std::string getCurrentTimestamp() const;
};

#endif // BANK_ACCOUNT_H
