#ifndef BANK_APP_H
#define BANK_APP_H

#include "include/BankAccount.h"
#include "include/BankUI.h"
#include <map>
#include <memory>
#include <string>

class BankApp {
public:
  BankApp();
  ~BankApp();

  // Initialize the application
  void initialize();

  // Run the application (blocking call)
  void run();

  // Cleanup
  void shutdown();

private:
  // UI instance
  std::unique_ptr<BankUI> m_ui;

  // Account storage
  std::map<std::string, std::shared_ptr<BankAccount>> m_accounts;

  // Setup UI bindings
  void setupBindings();

  // Load HTML content
  void loadUI();

  // Handler functions for UI callbacks
  std::string handleCreateAccount(const std::string &data);
  std::string handleDeposit(const std::string &data);
  std::string handleWithdraw(const std::string &data);
  std::string handleGetBalance(const std::string &data);
  std::string handleTransfer(const std::string &data);
};

#endif // BANK_APP_H
