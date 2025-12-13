#include "include/BankApp.h"
#include <iostream>

BankApp::BankApp() : m_ui(nullptr) {}

BankApp::~BankApp() { shutdown(); }

void BankApp::initialize() {
  // Create UI instance
  m_ui = std::make_unique<BankUI>(900, 700);
  m_ui->initialize();

  // Setup all UI bindings
  setupBindings();

  // Load the HTML interface
  loadUI();

  std::cout << "Bank application initialized successfully" << std::endl;
}

void BankApp::run() {
  if (!m_ui) {
    throw std::runtime_error("Application not initialized");
  }

  std::cout << "Starting bank application..." << std::endl;
  m_ui->run();
}

void BankApp::shutdown() {
  m_accounts.clear();
  std::cout << "Bank application shutdown" << std::endl;
}

void BankApp::setupBindings() {
  // Bind C++ functions to JavaScript
  m_ui->bindFunction("createAccount", [this](const std::string &data) {
    return handleCreateAccount(data);
  });

  m_ui->bindFunction("deposit", [this](const std::string &data) {
    return handleDeposit(data);
  });

  m_ui->bindFunction("withdraw", [this](const std::string &data) {
    return handleWithdraw(data);
  });

  m_ui->bindFunction("getBalance", [this](const std::string &data) {
    return handleGetBalance(data);
  });

  m_ui->bindFunction("transfer", [this](const std::string &data) {
    return handleTransfer(data);
  });
}

void BankApp::loadUI() {
  std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Bank Account System</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            padding: 20px;
        }
        
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: white;
            border-radius: 12px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            overflow: hidden;
        }
        
        .header {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            padding: 30px;
            text-align: center;
        }
        
        .header h1 {
            font-size: 2em;
            margin-bottom: 10px;
        }
        
        .content {
            padding: 30px;
        }
        
        .account-info {
            background: #f7fafc;
            padding: 20px;
            border-radius: 8px;
            margin-bottom: 30px;
        }
        
        .account-info h2 {
            color: #667eea;
            margin-bottom: 15px;
        }
        
        .balance {
            font-size: 2.5em;
            color: #2d3748;
            font-weight: bold;
        }
        
        .actions {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
            gap: 15px;
            margin-bottom: 30px;
        }
        
        .btn {
            padding: 15px 25px;
            border: none;
            border-radius: 8px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s;
            color: white;
        }
        
        .btn-deposit {
            background: linear-gradient(135deg, #10b981 0%, #059669 100%);
        }
        
        .btn-withdraw {
            background: linear-gradient(135deg, #ef4444 0%, #dc2626 100%);
        }
        
        .btn-refresh {
            background: linear-gradient(135deg, #3b82f6 0%, #2563eb 100%);
        }
        
        .btn:hover {
            transform: translateY(-2px);
            box-shadow: 0 5px 15px rgba(0,0,0,0.2);
        }
        
        .btn:active {
            transform: translateY(0);
        }
        
        .status {
            padding: 15px;
            border-radius: 8px;
            margin-top: 20px;
            display: none;
        }
        
        .status.success {
            background: #d1fae5;
            color: #065f46;
            display: block;
        }
        
        .status.error {
            background: #fee2e2;
            color: #991b1b;
            display: block;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>🏦 Bank Account System</h1>
            <p>Manage your finances with ease</p>
        </div>
        
        <div class="content">
            <div class="account-info">
                <h2>Account Balance</h2>
                <div class="balance" id="balance">$0.00</div>
                <p style="color: #718096; margin-top: 10px;">
                    Account: <span id="accountNumber">Loading...</span>
                </p>
                <p style="color: #718096;">
                    Owner: <span id="ownerName">Loading...</span>
                </p>
            </div>
            
            <div class="actions">
                <button class="btn btn-deposit" onclick="deposit()" >
                    💰 Deposit $500</ button>
                     <button class = "btn btn-withdraw" onclick =
                          "withdraw()">
                    💸 Withdraw $200</ button>
                     <button class = "btn btn-refresh" onclick =
                          "refreshBalance()">
                    🔄 Refresh</ button></ div>

                     <div id = "status" class = "status"></ div></ div></ div>

                     <script> window.addEventListener(
                         'load', async() = > {
                           await createAccount();
                           await refreshBalance();
                         });

  async function createAccount() {
    try {
      const result = await window.createAccount('{}');
      console.log('Account created:', result);
    } catch (error) {
      showStatus('Error creating account: ' + error, 'error');
    }
  }

  async function deposit() {
    try {
      const result = await window.deposit('{}');
      const data = JSON.parse(result);

      if (data.error) {
        showStatus(data.error, 'error');
      } else {
        updateUI(data);
        showStatus('Successfully deposited $500!', 'success');
      }
    } catch (error) {
      showStatus('Error: ' + error, 'error');
    }
  }

  async function withdraw() {
    try {
      const result = await window.withdraw('{}');
      const data = JSON.parse(result);

      if (data.error) {
        showStatus(data.error, 'error');
      } else {
        updateUI(data);
        showStatus('Successfully withdrew $200!', 'success');
      }
    } catch (error) {
      showStatus('Error: ' + error, 'error');
    }
  }

  async function refreshBalance() {
    try {
      const result = await window.getBalance('{}');
      const data = JSON.parse(result);

      if (data.error) {
        showStatus(data.error, 'error');
      } else {
        updateUI(data);
        showStatus('Balance refreshed!', 'success');
      }
    } catch (error) {
      showStatus('Error: ' + error, 'error');
    }
  }

  function updateUI(data) {
    document.getElementById('balance').textContent =
        '$' + data.balance.toFixed(2);
    document.getElementById('accountNumber').textContent = data.accountNumber;
    document.getElementById('ownerName').textContent = data.ownerName;
  }

  function showStatus(message, type) {
    const status = document.getElementById('status');
    status.textContent = message;
    status.className = 'status ' + type;

    setTimeout(() = > { status.className = 'status'; }, 3000);
  }
    </script>
</body>
</html>
    )";
    
    m_ui->setHTML(html);
}

// Handler implementations
std::string BankApp::handleCreateAccount(const std::string &data) {
  // For demo: create a sample account
  auto account = std::make_shared<BankAccount>("ACC001", "John Doe", 1000.0);
  m_accounts["ACC001"] = account;

  return account->toJSON();
}

std::string BankApp::handleDeposit(const std::string &data) {
  if (m_accounts.find("ACC001") != m_accounts.end()) {
    m_accounts["ACC001"]->deposit(500.0);
    return m_accounts["ACC001"]->toJSON();
  }

  return R"({"error":"Account not found"})";
}

std::string BankApp::handleWithdraw(const std::string &data) {
  if (m_accounts.find("ACC001") != m_accounts.end()) {
    if (m_accounts["ACC001"]->withdraw(200.0)) {
      return m_accounts["ACC001"]->toJSON();
    }
    return R"({"error":"Insufficient funds"})";
  }

  return R"({"error":"Account not found"})";
}

std::string BankApp::handleGetBalance(const std::string &data) {
  if (m_accounts.find("ACC001") != m_accounts.end()) {
    return m_accounts["ACC001"]->toJSON();
  }

  return R"({"error":"Account not found"})";
}

std::string BankApp::handleTransfer(const std::string &data) {
  // TODO: Implement transfer logic
  return R"({"error":"Transfer not implemented yet"})";
}
