🏦 Banking System (C++ OOP + AVL Tree Project)
📖 Overview

This project is a Bank Account Management System built in C++ using Object-Oriented Programming (OOP) concepts and an AVL Tree for efficient account storage and management.
It provides separate access for Managers and Users, supports authentication, and includes file handling for persistent data storage.

✨ Features
👩‍💼 Manager Features

Add new user accounts with account number, username, and initial balance.

Delete existing accounts.

Search for an account by account number.

View all users (in-order traversal of AVL tree).

👤 User Features

Deposit money.

Withdraw money.

View current balance.

Send money to other users.

View personal transaction history.

🔑 Authentication

Users and Managers must register with a username and password.

Credentials are saved in credentials.txt.

Login validation ensures role-based access.

💾 File Handling

accounts.txt → Stores account details (AccountNo Username Balance).

credentials.txt → Stores login credentials (Username Password Role).

transactions.txt → Stores transaction logs (SenderAccNo ReceiverAccNo Amount).

🛠️ Technologies Used

C++

AVL Tree (self-balancing BST for accounts)

OOP Concepts

Encapsulation

Classes & Structs (AccountTree, AccountNode)

File Handling (fstream)

Menu-driven Interface

📂 Project Structure
├── Dsa Project (Banking System).cpp   # Main source code
├── accounts.txt                       # Stores accounts data
├── credentials.txt                    # Stores login details
├── transactions.txt                   # Stores transaction logs
└── README.md                          # Documentation

🚀 How to Run

Compile the program:

g++ "Dsa Project ( Banking System ).cpp" -o banking


Run:

./banking

📋 Menu Flow
Main Menu
1. Login as Manager
2. Login as User
3. Register as Manager
4. Register as User
0. Exit

Manager Menu
1. Add User Account
2. Delete User Account
3. Search User Account
4. View All Users
0. Logout

User Menu
1. Deposit
2. Withdraw
3. View Balance
4. Send Money
5. View Transaction History
0. Logout

📂 Sample File Contents
accounts.txt
101 Alice 5000
102 Bob 3000

credentials.txt
Alice 1234 user
Bob 5678 user
Manager1 admin123 manager

transactions.txt
101 102 500
102 101 200

✅ Key Points

Uses AVL Tree to keep account operations (insert/search/delete) efficient (O(log n)).

Maintains persistent storage of accounts, transactions, and credentials.

Supports role-based access control (Manager vs User).

Logs all money transfers in transactions.txt.
