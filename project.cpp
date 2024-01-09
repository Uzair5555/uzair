#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class BudgetItem {
public:
    string name;
    double amount;

    BudgetItem(const string& n, double a) : name(n), amount(a) {}

    virtual void display() const = 0;
    virtual double getAmount() const {
        return amount;
    }

    virtual ~BudgetItem() {}
};

class FinancialGoal : public BudgetItem {
public:
    double targetAmount;
    double currentAmount;

    FinancialGoal(const string& n, double target) : BudgetItem(n, 0.0), targetAmount(target), currentAmount(0.0) {}

    void display() const override {
        cout << left << setw(25) << name << ": PKR" << currentAmount << " / PKR" << targetAmount << " (Progress: " << fixed << setprecision(2) << calculateProgress() << "%)" << endl;
    }

    double calculateProgress() const {
        return (currentAmount / targetAmount) * 100.0;
    }

    bool isGoalAchieved() const {
        return currentAmount >= targetAmount;
    }
};

class BudgetCategory : public BudgetItem {
public:
    bool isEssential;

    BudgetCategory(const string& n, double a, bool essential) : BudgetItem(n, a), isEssential(essential) {}

    void display() const override {
        cout << left << setw(25) << name << ": PKR" << amount << endl;
    }

    bool isEssentialCategory() const {
        return isEssential;
    }
};

class BudgetManager {
public:
    void showMenu();
    void addFamilyMembers();
    void addIncome();
    void addExpenses();
    void viewBudget();
    void analyzeExpenses();
    void addWishList();
    void saveExpenseAnalysisToFile();
    void calculateExpensePercentage();
    void manageFinancialGoals();
    void addFinancialGoal();
    void viewFinancialGoals();
    void exitProgram();

    ~BudgetManager();  // Destructor added for memory cleanup

private:
    int numFamilyMembers;
    double totalIncome;
    double totalExpenses;
    vector<BudgetItem*> wishListAndExpenses;  // Combined list of wishList and expenseCategories
    vector<FinancialGoal*> financialGoals;

    double convertIncome(const string& incomeStr);
};

void BudgetManager::showMenu() {
    cout << "===== Home Budget Management System =====" << endl;
    cout << "1. Add Family Members" << endl;
    cout << "2. Add Monthly Income" << endl;
    cout << "3. Add Expenses" << endl;
    cout << "4. View Budget" << endl;
    cout << "5. Analyze Expenses" << endl;
    cout << "6. Add Wish List" << endl;
    cout << "7. Save Expense Analysis to File" << endl;
    cout << "8. Calculate Expense Percentage" << endl;
    cout << "9. Manage Financial Goals" << endl;
    cout << "10. Exit" << endl;
    cout << "========================================" << endl;
}

void BudgetManager::addFamilyMembers() {
    cout << "Enter the number of family members: ";
    cin >> numFamilyMembers;
}

void BudgetManager::addIncome() {
    totalIncome = 0.0;

    for (int i = 0; i < numFamilyMembers; ++i) {
        cout << "Enter monthly income for person " << i + 1 << ": PKR";
        string incomeStr;
        cin >> incomeStr;
        double income = convertIncome(incomeStr);
        totalIncome += income;
    }

    cout << "Income added successfully!" << endl;
}

void BudgetManager::addExpenses() {
    cout << "\nEnter expenses for each category:\n";

    string categoryNames[] = {"House Rent", "House Maintenance", "Electricity Bills", "Transportation", "Food Expenses", "Utilities Bills", "Other Essential Bills",
                              "Entertainment", "Shopping", "Gifts"};

    totalExpenses = 0.0;

    for (int i = 0; i < 10; ++i) {
        cout << "Enter amount for " << categoryNames[i] << ": PKR";
        double amount;
        cin >> amount;

        wishListAndExpenses.push_back(new BudgetCategory(categoryNames[i], amount, true));
        totalExpenses += amount;
    }

    cout << "Expenses added successfully!" << endl;
}

void BudgetManager::viewBudget() {
    cout << "\nMonthly Budget Summary:\n";
    cout << "Total Income: PKR" << totalIncome << endl;
    cout << "Total Expenses: PKR" << totalExpenses << endl;
    cout << "Savings: PKR" << (totalIncome - totalExpenses) << endl;

    cout << "\nWish List and Expenses:\n";
    for (const auto& item : wishListAndExpenses) {
        item->display();
    }

    cout << "\nFinancial Goals:\n";
    for (const auto& goal : financialGoals) {
        goal->display();
        if (goal->isGoalAchieved()) {
            cout << "Congratulations! The goal is achieved." << endl;
        }
    }

    cout << endl;
}

void BudgetManager::analyzeExpenses() {
    cout << "\nExpense Analysis:\n";
    cout << "----------------------------------------" << endl;
    cout << left << setw(25) << "Category" << setw(15) << "Amount" << setw(20) << "Essential" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& category : wishListAndExpenses) {
        cout << left << setw(25) << category->name << ": PKR" << setw(15) << category->getAmount();
        cout << (dynamic_cast<BudgetCategory*>(category)->isEssentialCategory() ? "Essential" : "Non-Essential") << endl;
    }

    cout << "----------------------------------------" << endl;
    cout << "Total Expenses: PKR" << totalExpenses << endl;
    cout << "Savings: PKR" << (totalIncome - totalExpenses) << endl;
    cout << "Remaining Balance: PKR" << max(0.0, totalIncome - totalExpenses) << endl;
    cout << "========================================" << endl;
}

void BudgetManager::addWishList() {
    cout << "Enter items for your wish list:\n";
    string itemName;
    double itemPrice;
    char addMore;

    do {
        cout << "Item name: ";
        cin >> itemName;
        cout << "Item price: PKR";
        cin >> itemPrice;

        wishListAndExpenses.push_back(new BudgetCategory(itemName, itemPrice, false));

        cout << "Do you want to add more items? (y/n): ";
        cin >> addMore;

    } while (addMore == 'y' || addMore == 'Y');

    totalExpenses = 0.0;

    for (const auto& item : wishListAndExpenses) {
        totalExpenses += item->getAmount();
    }

    cout << "Wish list items added to expenses successfully!" << endl;
}

void BudgetManager::saveExpenseAnalysisToFile() {
    ofstream outFile("expense_analysis.txt");

    if (outFile.is_open()) {
        outFile << "\nExpense Analysis:\n";
        outFile << "----------------------------------------" << endl;
        outFile << left << setw(25) << "Category" << setw(15) << "Amount" << setw(20) << "Essential" << endl;
        outFile << "----------------------------------------" << endl;

        for (const auto& category : wishListAndExpenses) {
            outFile << left << setw(25) << category->name << ": PKR" << setw(15) << category->getAmount();
            outFile << (dynamic_cast<BudgetCategory*>(category)->isEssentialCategory() ? "Essential" : "Non-Essential") << endl;
        }

        outFile << "----------------------------------------" << endl;
        outFile << "Total Expenses: PKR" << totalExpenses << endl;
        outFile << "Savings: PKR" << (totalIncome - totalExpenses) << endl;
        outFile << "Remaining Balance: PKR" << max(0.0, totalIncome - totalExpenses) << endl;
        outFile << "========================================" << endl;

        cout << "Expense analysis saved to 'expense_analysis.txt' successfully!" << endl;

        outFile.close();
    } else {
        cout << "Unable to open the file for saving expense analysis." << endl;
    }
}

void BudgetManager::calculateExpensePercentage() {
    cout << "\nExpense Budget Analysis (%):\n";
    cout << "----------------------------------------" << endl;
    cout << left << setw(25) << "Category" << setw(15) << "Percentage" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& category : wishListAndExpenses) {
        double percentage = (category->getAmount() / totalExpenses) * 100.0;
        cout << left << setw(25) << category->name << ": " << setw(15) << fixed << setprecision(2) << percentage << "%" << endl;
    }

    cout << "========================================" << endl;
}

void BudgetManager::manageFinancialGoals() {
    int goalChoice;

    do {
        cout << "\nFinancial Goals Management:\n";
        cout << "1. Add Financial Goal" << endl;
        cout << "2. View Financial Goals" << endl;
        cout << "3. Exit Financial Goals Management" << endl;
        cout << "Enter your choice (1-3): ";
        cin >> goalChoice;

        switch (goalChoice) {
            case 1:
                addFinancialGoal();
                break;
            case 2:
                viewFinancialGoals();
                break;
            case 3:
                cout << "Exiting Financial Goals Management." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
        }
    } while (goalChoice != 3);
}

void BudgetManager::addFinancialGoal() {
    cout << "Enter the name of the financial goal: ";
    string goalName;
    cin.ignore();
    getline(cin, goalName);

    cout << "Enter the target amount for the financial goal: PKR";
    double targetAmount;
    cin >> targetAmount;

    financialGoals.push_back(new FinancialGoal(goalName, targetAmount));
    cout << "Financial goal added successfully!" << endl;
}

void BudgetManager::viewFinancialGoals() {
    if (financialGoals.empty()) {
        cout << "No financial goals set yet." << endl;
    } else {
        cout << "\nFinancial Goals:\n";
        for (const auto& goal : financialGoals) {
            goal->display();
            if (goal->isGoalAchieved()) {
                cout << "Congratulations! The goal is achieved." << endl;
            }
        }
    }
}

void BudgetManager::exitProgram() {
    cout << "Exiting the program. Goodbye!" << endl;

    // Clean up dynamically allocated memory
    for (auto item : wishListAndExpenses) {
        delete item;
    }

    for (auto goal : financialGoals) {
        delete goal;
    }
}

double BudgetManager::convertIncome(const string& incomeStr) {
    size_t kPos = incomeStr.find('k');
    return (kPos != string::npos) ? stod(incomeStr.substr(0, kPos)) * 1000.0 : stod(incomeStr);

}

BudgetManager::~BudgetManager() {
    // Clean up dynamically allocated memory in the destructor
    for (auto item : wishListAndExpenses) {
        delete item;
    }

    for (auto goal : financialGoals) {
        delete goal;
    }
}

int main() {
    BudgetManager budgetManager;
    int choice;

    do {
        budgetManager.showMenu();
        cout << "Enter your choice (1-10): ";
        cin >> choice;

        switch (choice) {
            case 1:
                budgetManager.addFamilyMembers();
                break;
            case 2:
                budgetManager.addIncome();
                break;
            case 3:
                budgetManager.addExpenses();
                break;
            case 4:
                budgetManager.viewBudget();
                break;
            case 5:
                budgetManager.analyzeExpenses();
                break;
            case 6:
                budgetManager.addWishList();
                break;
            case 7:
                budgetManager.saveExpenseAnalysisToFile();
                break;
            case 8:
                budgetManager.calculateExpensePercentage();
                break;
            case 9:
                budgetManager.manageFinancialGoals();
                break;
            case 10:
                budgetManager.exitProgram();
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 10." << endl;
        }

    } while (choice != 10);

    return 0;
}
