#include "include/BankAccount.h"
#include <ctime>
#include <iomanip>
#include <sstream>

BankAccount::BankAccount(const std::string &account_number,
                         const std::string &owner_name, double initial_balance)
    : m_account_number(account_number), m_owner_name(owner_name),
      m_balance(initial_balance) {

  if (initial_balance > 0) {
    addTransaction("Initial Deposit", initial_balance);
  }
}

bool BankAccount::deposit(double amount) {
  if (amount <= 0) {
    return false;
  }

  m_balance += amount;
  addTransaction("Deposit", amount);
  return true;
}

bool BankAccount::withdraw(double amount) {
  if (amount <= 0 || amount > m_balance) {
    return false;
  }

  m_balance -= amount;
  addTransaction("Withdrawal", amount);
  return true;
}

bool BankAccount::transfer(BankAccount &target, double amount) {
  if (amount <= 0 || amount > m_balance) {
    return false;
  }

  m_balance -= amount;
  target.m_balance += amount;

  addTransaction("Transfer Out", amount);
  target.addTransaction("Transfer In", amount);

  return true;
}

std::string BankAccount::getAccountNumber() const { return m_account_number; }

std::string BankAccount::getOwnerName() const { return m_owner_name; }

double BankAccount::getBalance() const { return m_balance; }

std::vector<Transaction> BankAccount::getTransactionHistory() const {
  return m_transactions;
}

std::string BankAccount::toJSON() const {
  std::ostringstream json;
  json << std::fixed << std::setprecision(2);

  json << "{";
  json << "\"accountNumber\":\"" << m_account_number << "\",";
  json << "\"ownerName\":\"" << m_owner_name << "\",";
  json << "\"balance\":" << m_balance << ",";
  json << "\"transactionCount\":" << m_transactions.size();
  json << "}";

  return json.str();
}

void BankAccount::addTransaction(const std::string &type, double amount) {
  Transaction t;
  t.type = type;
  t.amount = amount;
  t.timestamp = getCurrentTimestamp();
  t.balance_after = m_balance;

  m_transactions.push_back(t);
}

std::string BankAccount::getCurrentTimestamp() const {
  time_t now = time(nullptr);
  char buf[80];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
  return std::string(buf);
}
