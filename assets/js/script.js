// Store accounts in memory
let accounts = [];
let currentAccount = null;

// Initialize
function init() {
    updateDateTime();
    setInterval(updateDateTime, 60000);
    updateDashboard();
}

function updateDateTime() {
    const now = new Date();
    const options = { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' };
    document.getElementById('current-date').textContent = now.toLocaleDateString('en-US', options);
}

function updateDashboard() {
    if (currentAccount) {
        document.getElementById('dashboard-balance').textContent = `$${currentAccount.balance.toFixed(2)}`;
        document.getElementById('dashboard-account').textContent = `Account #${currentAccount.accountNumber}`;
        document.getElementById('current-account-display').textContent = currentAccount.name;

        // Pre-fill account numbers
        document.getElementById('deposit-account').value = currentAccount.accountNumber;
        document.getElementById('withdraw-account').value = currentAccount.accountNumber;
        document.getElementById('balance-account').value = currentAccount.accountNumber;
        document.getElementById('transfer-account').value = currentAccount.accountNumber;
    } else {
        document.getElementById('dashboard-balance').textContent = '$0.00';
        document.getElementById('dashboard-account').textContent = 'No account selected';
        document.getElementById('current-account-display').textContent = 'Guest Mode';
    }
}

function showAccountSwitcher() {
    const accountList = document.getElementById('account-list');

    if (accounts.length === 0) {
        accountList.innerHTML = '<p style="text-align: center; color: #7f8c8d; padding: 20px;">No accounts available. Please create one.</p>';
    } else {
        // Clear the list first
        accountList.innerHTML = '';

        // Create account items with proper event listeners
        accounts.forEach(acc => {
            const div = document.createElement('div');
            div.className = `account-item ${currentAccount && currentAccount.accountNumber === acc.accountNumber ? 'active' : ''}`;
            div.innerHTML = `
                <div class="account-item-name">${acc.name}</div>
                <div class="account-item-number">Account #${acc.accountNumber} • Balance: ${acc.balance.toFixed(2)}</div>
            `;
            div.addEventListener('click', function () {
                switchAccount(acc.accountNumber);
            });
            accountList.appendChild(div);
        });
    }

    document.getElementById('account-switcher-modal').classList.add('show');
}

function closeAccountSwitcher() {
    document.getElementById('account-switcher-modal').classList.remove('show');
}

function switchAccount(accountNumber) {
    const account = accounts.find(acc => acc.accountNumber === accountNumber);
    if (account) {
        currentAccount = account;
        updateDashboard();
        closeAccountSwitcher();
        showPage('home-page');
    }
}

function logout() {
    currentAccount = null;
    updateDashboard();
    showPage('home-page');
}

function showPage(pageId) {
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => page.classList.remove('active'));
    document.getElementById(pageId).classList.add('active');

    const forms = document.querySelectorAll('form');
    forms.forEach(form => form.reset());

    const balanceResult = document.getElementById('balance-result');
    balanceResult.classList.remove('show');

    updateDashboard();
}

function showModal(title, message, isSuccess) {
    const modal = document.getElementById('result-modal');
    const modalTitle = document.getElementById('modal-title');
    const modalMessage = document.getElementById('modal-message');
    const modalIcon = document.getElementById('modal-icon');

    modalTitle.textContent = title;
    modalMessage.textContent = message;
    modalIcon.innerHTML = isSuccess ? '<div class="success-icon">✓</div>' : '<div class="error-icon">✕</div>';

    modal.classList.add('show');
}

function closeModal() {
    document.getElementById('result-modal').classList.remove('show');
}

async function sendRequest(url, formData) {
    try {
        const params = new URLSearchParams();
        for (const [key, value] of formData.entries()) {
            params.append(key, value);
        }

        const response = await fetch(url, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: params.toString()
        });

        const data = await response.json();
        return data;
    } catch (error) {
        console.error('Request error:', error);
        return {
            success: false,
            message: 'Network error. Please check your connection.'
        };
    }
}

async function handleLogin(event) {
    event.preventDefault();
    const form = event.target;
    const formData = new FormData(form);

    const result = await sendRequest('/api/balance', formData);

    if (result.success) {
        // Check if account already exists in our list
        let account = accounts.find(acc => acc.accountNumber === result.accountNumber);

        if (!account) {
            // Add the account to our list
            account = {
                accountNumber: result.accountNumber,
                name: result.name,
                balance: result.balance
            };
            accounts.push(account);
        } else {
            // Update existing account balance
            account.balance = result.balance;
        }

        currentAccount = account;
        updateDashboard();

        showModal(
            'Login Successful',
            `Welcome back, ${result.name}!\n\nAccount #${result.accountNumber}\nBalance: ${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
        showPage('home-page');
    } else {
        showModal('Login Failed', result.message, false);
    }
}

async function handleCreate(event) {
    event.preventDefault();
    const form = event.target;
    const formData = new FormData(form);

    const result = await sendRequest('/api/create', formData);

    if (result.success) {
        const newAccount = {
            accountNumber: result.accountNumber,
            name: formData.get('name'),
            balance: parseFloat(formData.get('amount'))
        };
        accounts.push(newAccount);
        currentAccount = newAccount;

        showModal(
            'Account Created Successfully!',
            `Welcome to SecureBank, ${newAccount.name}!\n\nYour new account number is: ${result.accountNumber}\n\nInitial balance: $${newAccount.balance.toFixed(2)}\n\nPlease keep your PIN secure.`,
            true
        );
        form.reset();
        updateDashboard();
    } else {
        showModal('Account Creation Failed', result.message, false);
    }
}

async function handleDeposit(event) {
    event.preventDefault();
    const form = event.target;
    const formData = new FormData(form);

    const result = await sendRequest('/api/deposit', formData);

    if (result.success) {
        const account = accounts.find(acc => acc.accountNumber === parseInt(formData.get('account')));
        if (account) {
            account.balance = result.balance;
            if (currentAccount && currentAccount.accountNumber === account.accountNumber) {
                currentAccount.balance = result.balance;
            }
        }

        showModal(
            'Deposit Successful',
            `${result.message}\n\nNew Balance: $${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
        updateDashboard();
    } else {
        showModal('Deposit Failed', result.message, false);
    }
}

async function handleWithdraw(event) {
    event.preventDefault();
    const form = event.target;
    const formData = new FormData(form);

    const result = await sendRequest('/api/withdraw', formData);

    if (result.success) {
        const account = accounts.find(acc => acc.accountNumber === parseInt(formData.get('account')));
        if (account) {
            account.balance = result.balance;
            if (currentAccount && currentAccount.accountNumber === account.accountNumber) {
                currentAccount.balance = result.balance;
            }
        }

        showModal(
            'Withdrawal Successful',
            `${result.message}\n\nNew Balance: $${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
        updateDashboard();
    } else {
        showModal('Withdrawal Failed', result.message, false);
    }
}

async function handleBalance(event) {
    event.preventDefault();
    const form = event.target;
    const formData = new FormData(form);
    const resultDiv = document.getElementById('balance-result');

    const result = await sendRequest('/api/balance', formData);

    if (result.success) {
        resultDiv.innerHTML = `
            <h3>Account Information</h3>
            <p><strong>Account Number:</strong> ${result.accountNumber}</p>
            <p><strong>Account Holder:</strong> ${result.name}</p>
            <p><strong>Current Balance:</strong> $${result.balance.toFixed(2)}</p>
            <p><strong>Account Type:</strong> Checking Account</p>
        `;
        resultDiv.classList.add('show');
    } else {
        showModal('Access Denied', result.message, false);
        resultDiv.classList.remove('show');
    }
}

async function handleTransfer(event) {
    event.preventDefault();
    const form = event.target;
    const formData = new FormData(form);

    const result = await sendRequest('/api/transfer', formData);

    if (result.success) {
        const account = accounts.find(acc => acc.accountNumber === parseInt(formData.get('from_account')));
        if (account) {
            account.balance = result.balance;
            if (currentAccount && currentAccount.accountNumber === account.accountNumber) {
                currentAccount.balance = result.balance;
            }
        }

        showModal(
            'Transfer Successful',
            `${result.message}\n\nYour New Balance: $${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
        updateDashboard();
    } else {
        showModal('Transfer Failed', result.message, false);
    }
}

window.onclick = function (event) {
    const resultModal = document.getElementById('result-modal');
    const switcherModal = document.getElementById('account-switcher-modal');
    if (event.target === resultModal) {
        closeModal();
    }
    if (event.target === switcherModal) {
        closeAccountSwitcher();
    }
}

// Initialize on load
init();
