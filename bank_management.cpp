#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <string>
#include <random>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <limits>
using namespace std;

// Structure to represent a bank account
struct Account {
    int AccountNumber;
    string AccountHolderName;
    char Gender;
    string DateOfBirth;
    double Balance;
};

vector<Account> Accounts;

// Clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n'); 
}

// Check if account number exists
bool accountExists(int accountNumber) {
    for (const auto& account : Accounts) {
        if (account.AccountNumber == accountNumber) {
            return true;
        }
    }
    return false;
}

/**
 * Checks if a given year is a leap year.
 * @param Year The year to check
 * @return true if leap year, false otherwise
 */
bool LeapYear(int Year) {
    if (Year % 4 != 0) return false;
    if (Year % 100 != 0) return true;
    if (Year % 400 != 0) return false;
    return true;
}

/**
 * Validates a date
 * @param Day The day to check
 * @param Month The month to check
 * @param Year The year to check
 * @return true if valid date, false otherwise
 */
bool ValidDate(int Day, int Month, int Year) {
    time_t now = time(nullptr);
    struct tm timeStruct;

    if (localtime_s(&timeStruct, &now) != 0) {
        cout << "Error getting local time.\n";
        return false;
    }

    int currentYear = 1900 + timeStruct.tm_year;

    if (Year < 1900 || Year > currentYear) {
        cout << "Invalid year. Must be between 1900 and " << currentYear << endl;
        return false;
    }

    if (Month < 1 || Month > 12) {
        cout << "Invalid month. Must be between 1 and 12.\n";
        return false;
    }

    int DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (Month == 2 && LeapYear(Year)) {
        DaysInMonth[1] = 29;
    }

    if (Day < 1 || Day > DaysInMonth[Month - 1]) {
        cout << "Invalid day for the given month.\n";
        return false;
    }

    // Check if user is at least 18
    int age = currentYear - Year;
    if (Month > (timeStruct.tm_mon + 1) ||
        (Month == (timeStruct.tm_mon + 1) && Day > timeStruct.tm_mday)) {
        age--;
    }

    if (age < 18) {
        cout << "You must be 18 years or older.\n";
        return false;
    }

    return true;
}

/**
 * Validates date of birth format (DD-MM-YYYY)
 * @param DateOfBirth The date string to validate
 * @return true if valid, false otherwise
 */
bool ValidDateOfBirth(const string& DateOfBirth) {
    if (DateOfBirth.size() != 10 || DateOfBirth[2] != '-' || DateOfBirth[5] != '-') {
        cout << "Invalid date format. Use DD-MM-YYYY.\n";
        return false;
    }

    try {
        int day = stoi(DateOfBirth.substr(0, 2));
        int month = stoi(DateOfBirth.substr(3, 2));
        int year = stoi(DateOfBirth.substr(6, 4));
        return ValidDate(day, month, year);
    }
    catch (const exception& e) {
        cout << "Invalid date numbers.\n";
        return false;
    }
}

/**
 * Validates name (alphabetic characters and spaces only)
 * @param AccountHolderName The name to validate
 * @return true if valid, false otherwise
 */
bool ValidName(const string& AccountHolderName) {
    if (AccountHolderName.empty()) {
        cout << "Name cannot be empty.\n";
        return false;
    }

    for (char c : AccountHolderName) {
        if (!isalpha(c) && c != ' ') {
            cout << "Name can only contain letters and spaces.\n";
            return false;
        }
    }

    // Check for consecutive spaces or leading/trailing spaces
    if (AccountHolderName.front() == ' ' || AccountHolderName.back() == ' ') {
        cout << "Name cannot start or end with spaces.\n";
        return false;
    }

    for (size_t i = 0; i < AccountHolderName.size() - 1; i++) {
        if (AccountHolderName[i] == ' ' && AccountHolderName[i + 1] == ' ') {
            cout << "Name cannot contain consecutive spaces.\n";
            return false;
        }
    }

    return true;
}

/**
 * Validates gender (m/M or f/F)
 * @param Gender The gender character to validate
 * @return true if valid, false otherwise
 */
bool ValidGender(char Gender) {
    Gender = toupper(Gender);
    if (Gender == 'M' || Gender == 'F') {
        return true;
    }
    cout << "Invalid gender. Use M or F.\n";
    return false;
}

/**
 * Generates unique 6-digit account number
 * @return Unique account number
 */
int GenerateRandomAccountNumber() {
    static mt19937 gen(static_cast<unsigned>(time(0)));
    uniform_int_distribution<> dis(100000, 999999);

    int number;
    do {
        number = dis(gen);
    } while (accountExists(number));

    return number;
}

/**
 * Creates a new account with valid inputs
 */
void CreateAccount() {
    Account newAccount;

    // Get and validate name
    cout << "Enter your name: ";
    clearInputBuffer();
    getline(cin, newAccount.AccountHolderName);
    while (!ValidName(newAccount.AccountHolderName)) {
        cout << "Enter your name: ";
        getline(cin, newAccount.AccountHolderName);
    }

    // Get and validate date of birth
    cout << "Enter date of birth (DD-MM-YYYY): ";
    getline(cin, newAccount.DateOfBirth);
    while (!ValidDateOfBirth(newAccount.DateOfBirth)) {
        cout << "Enter date of birth (DD-MM-YYYY): ";
        getline(cin, newAccount.DateOfBirth);
    }

    // Get and validate gender
    cout << "Enter gender (M/F): ";
    cin >> newAccount.Gender;
    while (!ValidGender(newAccount.Gender)) {
        clearInputBuffer();
        cout << "Enter gender (M/F): ";
        cin >> newAccount.Gender;
    }
    newAccount.Gender = toupper(newAccount.Gender);

    // Get and validate initial deposit
    cout << "Enter initial deposit: ";
    while (!(cin >> newAccount.Balance) || newAccount.Balance <= 0) {
        cout << "Enter valid initial deposit (greater than 0): ";
        clearInputBuffer();
    }

    // Generate account number
    newAccount.AccountNumber = GenerateRandomAccountNumber();

    // Display confirmation
    system("cls");
    cout << "Account created successfully!\n";
    cout << "Your account number is: " << newAccount.AccountNumber << endl;
    cout << "Thanks for choosing our Banking System "
        << (newAccount.Gender == 'M' ? "Mr." : "Ms.")
        << newAccount.AccountHolderName << endl;

    Accounts.push_back(newAccount);

    cout << "Press any key to continue...";
    _getch();
    system("cls");
}

/**
 * Displays account details
 * @param AccountNumber The account number to display
 */
void ViewAccountDetails(int AccountNumber) {
    for (const auto& account : Accounts) {
        if (account.AccountNumber == AccountNumber) {
            cout << "Account Number: " << account.AccountNumber << "\n"
                << "Account Holder: " << account.AccountHolderName << "\n"
                << "Date Of Birth: " << account.DateOfBirth << "\n"
                << "Gender: " << account.Gender << "\n"
                << "Balance: $" << fixed << setprecision(2) << account.Balance << "\n";
            return;
        }
    }
    cout << "Account not found.\n";
}

/**
 * Deposits money into an account
 * @param AccountNumber The account number
 * @param Amount The amount to deposit
 */
void DepositMoney(int AccountNumber, double Amount) {
    for (auto& account : Accounts) {
        if (account.AccountNumber == AccountNumber) {
            account.Balance += Amount;
            cout << "Deposited $" << fixed << setprecision(2) << Amount
                << " successfully. New balance: $" << account.Balance << endl;
            return;
        }
    }
    cout << "Account not found.\n";
}

void WithdrawMoney(int AccountNumber, double Amount) {
    for (auto& account : Accounts) {
        if (account.AccountNumber == AccountNumber) {
            if (account.Balance < Amount) {
                cout << "Insufficient balance.\n";
                return;
            }
            account.Balance -= Amount;
            cout << "Withdrew $" << fixed << setprecision(2) << Amount
                << " successfully. New balance: $" << account.Balance << endl;
            return;
        }
    }
    cout << "Account not found.\n";
}

void Menu() {
    cout << "=== Banking System  ===\n"
        << "1. Create New Account\n"
        << "2. View Account Details\n"
        << "3. Deposit Money\n"
        << "4. Withdraw Money\n"
        << "5. Exit\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    while (true) {
        Menu();
        cout << "Enter your choice: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            cout << "Press any key to continue...";
            _getch();
            system("cls");
            continue;
        }

        system("cls");

        if (choice == 5) {
            cout << "Thank you for using out Banking System. Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 5) {
            cout << "Invalid choice. Please select 1-5.\n";
            cout << "Press any key to continue...";
            _getch();
            system("cls");
            continue;
        }

        if (choice == 1) {
            CreateAccount();
            continue;
        }


        cout << "Enter account number (0 to return to menu): ";
        int accountNumber;
        if (!(cin >> accountNumber)) {
            cout << "Invalid account number.\n";
            clearInputBuffer();
            cout << "Press any key to continue...";
            _getch();
            system("cls");
            continue;
        }

        if (accountNumber == 0) {
            system("cls");
            continue;
        }

        system("cls");

        switch (choice) {
        case 2:
            ViewAccountDetails(accountNumber);
            break;
        case 3: {
            cout << "Enter amount to deposit: ";
            double amount;
            if (!(cin >> amount) || amount <= 0) {
                cout << "Invalid amount. Must be greater than 0.\n";
                clearInputBuffer();
            }
            else {
                DepositMoney(accountNumber, amount);
            }
            break;
        }
        case 4: {
            cout << "Enter amount to withdraw: ";
            double amount;
            if (!(cin >> amount) || amount <= 0) {
                cout << "Invalid amount. Must be greater than 0.\n";
                clearInputBuffer();
            }
            else {
                WithdrawMoney(accountNumber, amount);
            }
            break;
        }
        }

        cout << "Press any key to continue...";
        _getch();
        system("cls");
    }

    return 0;
}