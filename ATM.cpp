#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
using namespace std;

// ==================== CLASS Bank ====================
class Bank {
private:
    string name;
    string password;
    string phone;
    float balance;

public:
    Bank(string n, string p, string ph, float b)
        : name(n), password(p), phone(ph), balance(b) {}

    // --- Getters & Setters ---
    string get_name() { return name; }
    string get_pass() { return password; }
    string get_phone() { return phone; }
    float get_bal() { return balance; }

    void set_name(string n) { name = n; }
    void set_pass(string p) { password = p; }
    void set_phone(string ph) { phone = ph; }
    void set_bal(float b) { balance = b; }

    // --- Banking Functions ---
    void deposit(float amt) { balance += amt; }

    bool withdraw(float amt) {
        if (amt > balance) {
            cout << "❌ Insufficient Balance!" << endl;
            return false;
        }
        balance -= amt;
        return true;
    }
};

// ==================== CLASS Manage ====================
class Manage {
private:
    vector<Bank> accounts;
    Bank* loggedIn = nullptr; // Currently logged-in user

public:
    // 🔹 Add New Account
    void add() {
        string n, p, ph;
        float amt;
        cout << "👤 Enter Account Holder Name       : ";
        cin >> n;
        cout << "🔒 Enter Password                 : ";
        cin >> p;
        cout << "📱 Enter Phone Number (e.g. 0300...) : ";
        cin >> ph;
        cout << "💰 Enter Initial Deposit Amount   : ";
        cin >> amt;

        accounts.push_back(Bank(n, p, ph, amt));
        cout << "✅ Account Created Successfully!\n";
    }

    // 🔹 Login System
    void login() {
        string ph, p;
        cout << "📱 Enter Phone Number: ";
        cin >> ph;
        cout << "🔒 Enter Password: ";
        cin >> p;

        for (auto& acc : accounts) {
            if (acc.get_phone() == ph && acc.get_pass() == p) {
                loggedIn = &acc;
                cout << "✅ Login Successful! Welcome, " << acc.get_name() << " 🎉\n";
                return;
            }
        }
        cout << "❌ Invalid Phone or Password!\n";
    }

    // 🔹 Logout
    void logout() {
        if (loggedIn) {
            cout << "👋 Logged out successfully from " << loggedIn->get_name() << "\n";
            loggedIn = nullptr;
        } else {
            cout << "⚠️ You are not logged in!\n";
        }
    }

    // 🔹 Show All Accounts (Admin View)
    void show() {
        if (accounts.empty()) {
            cout << "⚠️ No Accounts Found!\n";
            return;
        }

        cout << "\n================ ACCOUNT LIST ================\n";
        cout << left << setw(5) << "No" << setw(15) << "Name" << setw(15) << "Phone" << setw(10) << "Balance" << endl;
        cout << "---------------------------------------------\n";
        for (size_t i = 0; i < accounts.size(); ++i) {
            cout << setw(5) << i + 1
                 << setw(15) << accounts[i].get_name()
                 << setw(15) << accounts[i].get_phone()
                 << setw(10) << accounts[i].get_bal()
                 << endl;
        }
        cout << "=============================================\n";
    }

    // 🔹 Search Account (By Phone)
    void search() {
        string ph;
        cout << "📞 Enter Phone Number to Search: ";
        cin >> ph;

        Bank* acc = find_by_phone(ph);
        if (acc) {
            cout << "\n✅ Account Found:\n";
            cout << "👤 Name: " << acc->get_name() << endl;
            cout << "📱 Phone: " << acc->get_phone() << endl;
            cout << "💰 Balance: " << acc->get_bal() << endl;
        } else {
            cout << "❌ No Account Found with This Phone Number!\n";
        }
    }

    // 🔹 Update Account Info
    void update() {
        if (!loggedIn) {
            cout << "⚠️ Please Login First!\n";
            return;
        }

        int opt;
        cout << "\nWhat do you want to update?\n";
        cout << "1. Name\n2. Password\n3. Phone Number\n";
        cout << "Enter option: ";
        cin >> opt;

        string newValue;
        switch (opt) {
        case 1:
            cout << "Enter New Name: ";
            cin >> newValue;
            loggedIn->set_name(newValue);
            cout << "✅ Name Updated Successfully!\n";
            break;
        case 2:
            cout << "Enter New Password: ";
            cin >> newValue;
            loggedIn->set_pass(newValue);
            cout << "✅ Password Updated Successfully!\n";
            break;
        case 3:
            cout << "Enter New Phone Number: ";
            cin >> newValue;
            loggedIn->set_phone(newValue);
            cout << "✅ Phone Number Updated Successfully!\n";
            break;
        default:
            cout << "⚠️ Invalid Option!\n";
        }
    }

    // 🔹 Deposit Money
    void deposit() {
        if (!loggedIn) {
            cout << "⚠️ Please Login First!\n";
            return;
        }

        float amt;
        cout << "💰 Enter Amount to Deposit: ";
        cin >> amt;
        loggedIn->deposit(amt);
        cout << "✅ " << amt << " Deposited Successfully!\n";
        cout << "💰 New Balance: " << loggedIn->get_bal() << endl;
    }

    // 🔹 Withdraw Money
    void withdraw() {
        if (!loggedIn) {
            cout << "⚠️ Please Login First!\n";
            return;
        }

        float amt;
        cout << "💸 Enter Amount to Withdraw: ";
        cin >> amt;
        if (loggedIn->withdraw(amt)) {
            cout << "✅ Withdraw Successful!\n";
            cout << "💰 Remaining Balance: " << loggedIn->get_bal() << endl;
        }
    }

    // 🔹 Transfer Money (Like JazzCash)
    void transfer() {
        if (!loggedIn) {
            cout << "⚠️ Please Login First!\n";
            return;
        }

        string receiverPhone;
        float amt;

        cout << "📱 Enter Receiver Phone Number: ";
        cin >> receiverPhone;
        Bank* receiver = find_by_phone(receiverPhone);

        if (!receiver) {
            cout << "❌ Receiver Not Found!\n";
            return;
        }

        if (receiver == loggedIn) {
            cout << "⚠️ Cannot Transfer to Your Own Account!\n";
            return;
        }

        cout << "👤 Receiver Name: " << receiver->get_name() << endl;
        cout << "💰 Enter Amount to Send: ";
        cin >> amt;

        if (amt <= 0) {
            cout << "❌ Invalid Amount!\n";
            return;
        }

        if (amt > loggedIn->get_bal()) {
            cout << "❌ Insufficient Balance!\n";
            return;
        }

        char confirm;
        cout << "⚡ Confirm Transfer of " << amt << " to " << receiver->get_name() << " (y/n)? ";
        cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            loggedIn->withdraw(amt);
            receiver->deposit(amt);
            cout << "✅ Transfer Successful!\n";
            cout << "📤 Sent From: " << loggedIn->get_name() << " (" << loggedIn->get_phone() << ")\n";
            cout << "📥 Received By: " << receiver->get_name() << " (" << receiver->get_phone() << ")\n";
            cout << "💰 Remaining Balance: " << loggedIn->get_bal() << endl;
        } else {
            cout << "❌ Transfer Cancelled!\n";
        }
    }

private:
    Bank* find_by_phone(string ph) {
        for (auto& acc : accounts) {
            if (acc.get_phone() == ph)
                return &acc;
        }
        return NULL;
    }
};

// ==================== MAIN FUNCTION ====================
int main() {
    Manage m;
    int choice;
    string again;

    do {
        system("cls");
        cout << "\n============== BANK MANAGEMENT SYSTEM ==============\n";
        cout << "1. Add New Account\n";
        cout << "2. Login to Account\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Transfer Money (By Phone)\n";
        cout << "6. Search Account\n";
        cout << "7. Update Account Info\n";
        cout << "8. Show All Accounts\n";
        cout << "9. Logout\n";
        cout << "10. Exit\n";
        cout << "===================================================\n";
        cout << "👉 Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: m.add(); break;
        case 2: m.login(); break;
        case 3: m.deposit(); break;
        case 4: m.withdraw(); break;
        case 5: m.transfer(); break;
        case 6: m.search(); break;
        case 7: m.update(); break;
        case 8: m.show(); break;
        case 9: m.logout(); break;
        case 10:
            cout << "👋 Exiting Program...\n";
            exit(0);
        default:
            cout << "⚠️ Invalid Option! Try Again.\n";
        }

        cout << "\nDo You Want to Continue? (y/n): ";
        cin >> again;

    } while (again == "y" || again == "Y");

    return 0;
}
