# Bank Account System Simulator

A school project simulating a basic banking system with core functionalities like account management, transactions, and user authentication.

## Overview

This is a learning project that demonstrates how banking systems work. It lets you create accounts, make deposits and withdrawals, transfer money, and view transaction history. Perfect for understanding banking concepts and practicing programming skills!

## Features

### Account Management

- Create new customer accounts with unique account numbers
- Support for multiple account types (savings, checking, etc.)
- Store comprehensive customer information including name, ID, and contact details
- Real-time balance tracking and updates
- Account status management (active, frozen, closed)

### Transaction Processing

- **Deposits**: Add funds to any account
- **Withdrawals**: Remove funds with overdraft protection
- **Transfers**: Move money between accounts instantly
- Real-time balance updates across all operations
- Transaction validation to prevent insufficient fund errors
- Automatic timestamp recording for all transactions

### Authentication & Security

- Secure login system with username and password
- PIN-based verification for sensitive operations
- Session management to track active users
- Identity verification before allowing account access
- Password encryption and secure credential storage
- Automatic logout after period of inactivity

### Transaction History

- Complete transaction log for every account
- Detailed records including date, time, amount, and transaction type
- Filter transactions by date range, type, or amount
- Export transaction history for record-keeping
- Permanent audit trail for all financial activities

### Data Storage

- Persistent database storage for all system data
- Customer profile management
- Account information retention
- Complete transaction record archive
- Data integrity checks and validation
- Backup and recovery capabilities

### User Interface

- Clean, intuitive interface for easy navigation
- Menu-driven system for all operations
- Clear prompts and error messages
- Account summary dashboard
- Transaction confirmation screens
- Help system and documentation access

## Getting Started

### What You Need

```
- Python 3.x (or whatever language you're using)
- A database (SQLite works great for this)
- Basic programming environment
```

### How to Run

```bash
# Download or clone the project
git clone https://github.com/yourusername/bank-simulator.git

# Go to the project folder
cd bank-simulator

# Install what you need
pip install -r requirements.txt

# Set up the database
python setup_database.py

# Run it!
python main.py
```

## System Architecture

```
┌─────────────────────────────────────┐
│        User Interface Layer         │
├─────────────────────────────────────┤
│     Authentication & Security       │
├─────────────────────────────────────┤
│  Account Management │ Transaction   │
│                     │ Processing    │
├─────────────────────┴───────────────┤
│       Transaction History           │
├─────────────────────────────────────┤
│         Data Storage Layer          │
└─────────────────────────────────────┘
```

## Testing

```bash
# Run tests (if you added them)
python -m pytest tests/
```

## What I Learned

Building this project helped me understand:
- How databases store and manage information
- User authentication and basic security
- Transaction processing and data validation
- Keeping track of money flows
- Designing user-friendly interfaces

## Contributing

This is a school project, but suggestions are welcome! Feel free to fork it and make it your own.

## Future Ideas

If I have time, I might add:
- Interest calculation for savings accounts
- Account statements
- Better error handling
- A nicer looking interface
- More account types

## License

MIT License - feel free to use this for your own learning!

## Note

This is a **school project** for learning purposes. Don't use it for real money or actual banking! 😅
