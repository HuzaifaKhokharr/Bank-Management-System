
#include<string>
#include <iostream>
#include <fstream>
using namespace std;

class AccountTree {
public:
    struct AccountNode {
        string username;
        int accountNumber;
        double balance;
        int height;
        AccountNode* left;
        AccountNode* right;
        AccountNode(const string& user, int accNo, double bal);
    };

private:
    AccountNode* root;
    int maxVal(int a, int b);
    int getHeight(AccountNode* node);
    int getBalanceFactor(AccountNode* node);
    bool compareString(const string& s1, const string& s2);
    AccountNode* rightRotate(AccountNode* y);
    AccountNode* leftRotate(AccountNode* x);
    AccountNode* insertNode(AccountNode* node, const string& username, int accNo, double balance);
    AccountNode* minValueNode(AccountNode* node);
    AccountNode* deleteNode(AccountNode* node, int accNo);
    AccountNode* searchNode(AccountNode* node, int accNo);
    void displayInOrder(AccountNode* node);
    void saveToFile(AccountNode* node, ofstream& file);
    void loadFromFile(AccountNode*& node, ifstream& file);

public:
    AccountTree();
    void insert(const string& username, int accNo, double balance);
    void deleteAccount(int accNo);
    AccountNode* search(int accNo);
    void display();
    void writeToFile();
    void loadFromFile();
    void sendMoney(const string& senderUsername, int senderAccNo);
    void viewTransactionHistory(int accNo);
};

// ========== IMPLEMENTATION ==========

AccountTree::AccountNode::AccountNode(const string& user, int accNo, double bal) {
    username = user;
    accountNumber = accNo;
    balance = bal;
    height = 1;
    left = NULL;
    right = NULL;
}

AccountTree::AccountTree() { root = NULL; }

int AccountTree::maxVal(int a, int b) { return (a > b) ? a : b; }

int AccountTree::getHeight(AccountNode* node) {
    if (node == NULL) return 0;
    return node->height;
}

int AccountTree::getBalanceFactor(AccountNode* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

bool AccountTree::compareString(const string& s1, const string& s2) {
    int i = 0;
    while (i < s1.size() && i < s2.size()) {
        if (s1[i] != s2[i])
            return false;
        i++;
    }
    if (s1.size() != s2.size())
        return false;
    return true;
}

AccountTree::AccountNode* AccountTree::rightRotate(AccountNode* y) {
    AccountNode* x = y->left;
    AccountNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AccountTree::AccountNode* AccountTree::leftRotate(AccountNode* x) {
    AccountNode* y = x->right;
    AccountNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = maxVal(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = maxVal(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

AccountTree::AccountNode* AccountTree::insertNode(AccountNode* node, const string& username, int accNo, double balance) {
    if (node == NULL)
        return new AccountNode(username, accNo, balance);

    if (accNo < node->accountNumber)
        node->left = insertNode(node->left, username, accNo, balance);
    else if (accNo > node->accountNumber)
        node->right = insertNode(node->right, username, accNo, balance);
    else
        return node;

    node->height = 1 + maxVal(getHeight(node->left), getHeight(node->right));
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1 && accNo < node->left->accountNumber)
        return rightRotate(node);
    if (balanceFactor < -1 && accNo > node->right->accountNumber)
        return leftRotate(node);
    if (balanceFactor > 1 && accNo > node->left->accountNumber) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balanceFactor < -1 && accNo < node->right->accountNumber) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void AccountTree::insert(const string& username, int accNo, double balance) {
    root = insertNode(root, username, accNo, balance);
}

AccountTree::AccountNode* AccountTree::minValueNode(AccountNode* node) {
    AccountNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

AccountTree::AccountNode* AccountTree::deleteNode(AccountNode* node, int accNo) {
    if (node == NULL)
        return node;

    if (accNo < node->accountNumber)
        node->left = deleteNode(node->left, accNo);
    else if (accNo > node->accountNumber)
        node->right = deleteNode(node->right, accNo);
    else {
        if (node->left == NULL || node->right == NULL) {
            AccountNode* temp = (node->left) ? node->left : node->right;
            if (temp == NULL) {
                temp = node;
                node = NULL;
            }
            else
                *node = *temp;
            delete temp;
        }
        else {
            AccountNode* temp = minValueNode(node->right);
            node->username = temp->username;
            node->accountNumber = temp->accountNumber;
            node->balance = temp->balance;
            node->right = deleteNode(node->right, temp->accountNumber);
        }
    }

    if (node == NULL) return node;

    node->height = 1 + maxVal(getHeight(node->left), getHeight(node->right));
    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
        return rightRotate(node);
    if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
        return leftRotate(node);
    if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void AccountTree::deleteAccount(int accNo) {
    root = deleteNode(root, accNo);
}

AccountTree::AccountNode* AccountTree::searchNode(AccountNode* node, int accNo) {
    if (node == NULL) return NULL;
    if (node->accountNumber == accNo) return node;
    if (accNo < node->accountNumber)
        return searchNode(node->left, accNo);
    else
        return searchNode(node->right, accNo);
}

AccountTree::AccountNode* AccountTree::search(int accNo) {
    return searchNode(root, accNo);
}

void AccountTree::displayInOrder(AccountNode* node) {
    if (node == NULL) return;
    displayInOrder(node->left);
    cout << "Account No: " << node->accountNumber << ", Username: " << node->username << ", Balance: $" << node->balance << endl;
    displayInOrder(node->right);
}

void AccountTree::display() {
    displayInOrder(root);
}

void AccountTree::saveToFile(AccountNode* node, ofstream& file) {
    if (node == NULL) return;
    saveToFile(node->left, file);
    file << node->accountNumber << " " << node->username << " " << node->balance << endl;
    saveToFile(node->right, file);
}

void AccountTree::writeToFile() {
    ofstream outFile("accounts.txt");
    saveToFile(root, outFile);
    outFile.close();
}

void AccountTree::loadFromFile(AccountNode*& node, ifstream& file) {
    int accNo;
    string username;
    double balance;
    while (file >> accNo >> username >> balance) {
        node = insertNode(node, username, accNo, balance);
    }
}

void AccountTree::loadFromFile() {
    ifstream file("accounts.txt");
    if (file.is_open()) {
        loadFromFile(root, file);
        file.close();
    }
}

void AccountTree::sendMoney(const string& senderUsername, int senderAccNo) {
    int receiverAccNo;
    double amount;

    cout << "Enter receiver's account number: ";
    cin >> receiverAccNo;

    if (receiverAccNo == senderAccNo) {
        cout << "You cannot send money to yourself.\n";
        return;
    }

    AccountNode* sender = search(senderAccNo);
    AccountNode* receiver = search(receiverAccNo);

    if (receiver == NULL) {
        cout << "Receiver account not found.\n";
        return;
    }

    cout << "Enter amount to send: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Invalid amount.\n";
        return;
    }

    if (amount > sender->balance) {
        cout << "Insufficient balance.\n";
        return;
    }

    sender->balance -= amount;
    receiver->balance += amount;
    writeToFile();

    cout << "Transaction successful. $" << amount << " sent to " << receiver->username << " (Acc#: " << receiverAccNo << ").\n";

    ofstream logFile("transactions.txt", ios::app);
    if (logFile.is_open()) {
        logFile << senderAccNo << " " << receiverAccNo << " " << amount << endl;
        logFile.close();
    }
}

void AccountTree::viewTransactionHistory(int accNo) {
    ifstream logFile("transactions.txt");
    if (!logFile.is_open()) {
        cout << "No transaction history found.\n";
        return;
    }

    int sender, receiver;
    double amount;
    bool found = false;

    cout << "\n--- Transaction History ---\n";

    while (logFile >> sender >> receiver >> amount) {
        if (sender == accNo || receiver == accNo) {
            cout << "From: " << sender << " To: " << receiver << " | Amount: $" << amount << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No transactions found for this account.\n";
    }

    logFile.close();
}
// -------- Authentication and Main Menu --------

void registerUser(const string& role) {
    string username, password;
    cout << "Enter username: ";
    cin >> username;

    ifstream file("credentials.txt");
    string storedUser, storedPass, storedRole;
    while (file >> storedUser >> storedPass >> storedRole) {
        bool same = true;
        int i = 0;
        while (i < username.size() && i < storedUser.size()) {
            if (username[i] != storedUser[i]) {
                same = false;
                break;
            }
            i++;
        }
        if (username.size() != storedUser.size())
            same = false;

        if (same) {
            cout << "Username already exists!\n";
            file.close();
            return;
        }
    }
    file.close();

    cout << "Enter password: ";
    cin >> password;

    ofstream outFile("credentials.txt", ios::app);
    outFile << username << " " << password << " " << role << "\n";
    outFile.close();

    cout << "Registered successfully as " << role << "!\n";
}

bool authenticate(const string& username, const string& password, string& role) {
    ifstream file("credentials.txt");
    if (!file.is_open())
        return false;

    string storedUser, storedPass, storedRole;
    while (file >> storedUser >> storedPass >> storedRole) {
        bool userMatch = true, passMatch = true;

        int i = 0;
        while (i < username.size() && i < storedUser.size()) {
            if (username[i] != storedUser[i]) {
                userMatch = false;
                break;
            }
            i++;
        }
        if (username.size() != storedUser.size())
            userMatch = false;

        i = 0;
        while (i < password.size() && i < storedPass.size()) {
            if (password[i] != storedPass[i]) {
                passMatch = false;
                break;
            }
            i++;
        }
        if (password.size() != storedPass.size())
            passMatch = false;

        if (userMatch && passMatch) {
            role = storedRole;
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void managerMenu(AccountTree& tree) {
    int choice, accNo;
    string username;
    double amount;

    while (true) {
        cout << "\n=== Manager Menu ===\n";
        cout << "1. Add User Account\n2. Delete User Account\n3. Search User Account\n4. View All Users\n0. Logout\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter new account number: ";
            cin >> accNo;
            if (tree.search(accNo) != NULL) {
                cout << "Account already exists!\n";
                continue;
            }
            cout << "Enter username: ";
            cin.ignore();
            getline(cin, username);
            cout << "Enter initial balance: ";
            cin >> amount;
            tree.insert(username, accNo, amount);
            tree.writeToFile();
            cout << "User account created.\n";
        }
        else if (choice == 2) {
            cout << "Enter account number to delete: ";
            cin >> accNo;
            if (tree.search(accNo) == NULL) {
                cout << "Account not found.\n";
            }
            else {
                tree.deleteAccount(accNo);
                tree.writeToFile();
                cout << "Account deleted.\n";
            }
        }
        else if (choice == 3) {
            cout << "Enter account number to search: ";
            cin >> accNo;
            AccountTree::AccountNode* acc = tree.search(accNo);
            if (acc != NULL) {
                cout << "Found: " << acc->username << " | Balance: $" << acc->balance << "\n";
            }
            else {
                cout << "Account not found.\n";
            }
        }
        else if (choice == 4) {
            tree.display();
        }
        else if (choice == 0) {
            return;
        }
        else {
            cout << "Invalid option.\n";
        }
    }
}

void userMenu(AccountTree& tree, const string& username) {
    int accNo;
    cout << "Enter your account number: ";
    cin >> accNo;

    AccountTree::AccountNode* acc = tree.search(accNo);
    if (acc == NULL) {
        cout << "Account not found.\n";
        return;
    }

    bool same = true;
    int i = 0;
    while (i < username.size() && i < acc->username.size()) {
        if (username[i] != acc->username[i]) {
            same = false;
            break;
        }
        i++;
    }
    if (username.size() != acc->username.size())
        same = false;

    if (!same) {
        cout << "Access denied. Username does not match.\n";
        return;
    }

    int choice;
    double amount;

    while (true) {
        cout << "\n=== User Menu ===\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. View Balance\n";
        cout << "4. Send Money\n";
        cout << "5. View Transaction History\n";
        cout << "0. Logout\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Amount to deposit: ";
            cin >> amount;
            acc->balance += amount;
            tree.writeToFile();
            cout << "Deposit successful. Balance: $" << acc->balance << "\n";
        }
        else if (choice == 2) {
            cout << "Amount to withdraw: ";
            cin >> amount;
            if (amount > acc->balance) {
                cout << "Insufficient balance.\n";
            }
            else {
                acc->balance -= amount;
                tree.writeToFile();
                cout << "Withdrawal successful. Balance: $" << acc->balance << "\n";
            }
        }
        else if (choice == 3) {
            cout << "Current Balance: $" << acc->balance << "\n";
        }
        else if (choice == 4) {
            tree.sendMoney(acc->username, acc->accountNumber);
        }
        else if (choice == 5) {
            tree.viewTransactionHistory(acc->accountNumber);
        }
        else if (choice == 0) {
            return;
        }
        else {
            cout << "Invalid option.\n";
        }
    }
}

void mainMenu(AccountTree& tree) {
    tree.loadFromFile();

    while (true) {
        cout << "\n=== Welcome to Bank Account Manager ===\n";
        cout << "1. Login as Manager\n2. Login as User\n3. Register as Manager\n4. Register as User\n0. Exit\nChoose: ";
        int choice;
        cin >> choice;

        string username, password, role;

        if (choice == 1 || choice == 2) {
            cout << "Username: ";
            cin >> username;
            cout << "Password: ";
            cin >> password;
            if (authenticate(username, password, role)) {
                if (choice == 1 && role == "manager")
                    managerMenu(tree);
                else if (choice == 2 && role == "user")
                    userMenu(tree, username);
                else
                    cout << "Access denied: Incorrect role.\n";
            }
            else {
                cout << "Login failed.\n";
            }
        }
        else if (choice == 3) {
            registerUser("manager");
        }
        else if (choice == 4) {
            registerUser("user");
        }
        else if (choice == 0) {
            tree.writeToFile();
            cout << "Goodbye!\n";
            return;
        }
        else {
            cout << "Invalid option.\n";
        }
    }
}

int main() {
    AccountTree tree;
    mainMenu(tree);
    return 0;
}
