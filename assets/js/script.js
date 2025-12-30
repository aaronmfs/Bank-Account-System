// Page Navigation
function showPage(pageId) {
    // Hide all pages
    const pages = document.querySelectorAll('.page');
    pages.forEach(page => page.classList.remove('active'));

    // Show selected page
    document.getElementById(pageId).classList.add('active');

    // Clear forms and results
    const forms = document.querySelectorAll('form');
    forms.forEach(form => form.reset());

    const balanceResult = document.getElementById('balance-result');
    balanceResult.classList.remove('show');
}

// Show Modal
function showModal(title, message, isSuccess) {
    const modal = document.getElementById('result-modal');
    const modalContent = modal.querySelector('.modal-content');
    const modalTitle = document.getElementById('modal-title');
    const modalMessage = document.getElementById('modal-message');

    modalTitle.textContent = title;
    modalMessage.textContent = message;

    // Add success or error styling
    modalContent.classList.remove('success', 'error');
    modalContent.classList.add(isSuccess ? 'success' : 'error');

    modal.classList.add('show');
}

// Close Modal
function closeModal() {
    const modal = document.getElementById('result-modal');
    modal.classList.remove('show');
}

// Helper function to send form data
async function sendRequest(url, formData) {
    try {
        const response = await fetch(url, {
            method: 'POST',
            body: formData
        });

        const data = await response.json();
        return data;
    } catch (error) {
        return {
            success: false,
            message: 'Network error. Please check your connection.'
        };
    }
}

// Handle Create Account
async function handleCreate(event) {
    event.preventDefault();

    const form = event.target;
    const formData = new FormData(form);

    // Add loading state
    form.classList.add('loading');

    const result = await sendRequest('/api/create', formData);

    // Remove loading state
    form.classList.remove('loading');

    if (result.success) {
        showModal(
            'Success!',
            `Account created successfully! Your account number is: ${result.accountNumber}`,
            true
        );
        form.reset();
    } else {
        showModal('Error', result.message, false);
    }
}

// Handle Deposit
async function handleDeposit(event) {
    event.preventDefault();

    const form = event.target;
    const formData = new FormData(form);

    form.classList.add('loading');

    const result = await sendRequest('/api/deposit', formData);

    form.classList.remove('loading');

    if (result.success) {
        showModal(
            'Success!',
            `${result.message}\nNew Balance: $${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
    } else {
        showModal('Error', result.message, false);
    }
}

// Handle Withdraw
async function handleWithdraw(event) {
    event.preventDefault();

    const form = event.target;
    const formData = new FormData(form);

    form.classList.add('loading');

    const result = await sendRequest('/api/withdraw', formData);

    form.classList.remove('loading');

    if (result.success) {
        showModal(
            'Success!',
            `${result.message}\nNew Balance: $${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
    } else {
        showModal('Error', result.message, false);
    }
}

// Handle Check Balance
async function handleBalance(event) {
    event.preventDefault();

    const form = event.target;
    const formData = new FormData(form);
    const resultDiv = document.getElementById('balance-result');

    form.classList.add('loading');

    const result = await sendRequest('/api/balance', formData);

    form.classList.remove('loading');

    if (result.success) {
        resultDiv.innerHTML = `
            <h3>Account Information</h3>
            <p><strong>Account Number:</strong> ${result.accountNumber}</p>
            <p><strong>Name:</strong> ${result.name}</p>
            <p><strong>Balance:</strong> $${result.balance.toFixed(2)}</p>
        `;
        resultDiv.classList.add('show');
    } else {
        showModal('Error', result.message, false);
        resultDiv.classList.remove('show');
    }
}

// Handle Transfer
async function handleTransfer(event) {
    event.preventDefault();

    const form = event.target;
    const formData = new FormData(form);

    form.classList.add('loading');

    const result = await sendRequest('/api/transfer', formData);

    form.classList.remove('loading');

    if (result.success) {
        showModal(
            'Success!',
            `${result.message}\nYour New Balance: $${result.balance.toFixed(2)}`,
            true
        );
        form.reset();
    } else {
        showModal('Error', result.message, false);
    }
}

// Close modal when clicking outside
window.onclick = function (event) {
    const modal = document.getElementById('result-modal');
    if (event.target === modal) {
        closeModal();
    }
}
