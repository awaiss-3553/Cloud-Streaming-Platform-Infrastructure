#include <iostream>
#include <string>
using namespace std;

class paymentgateway {
protected:
    string senderAccount;
    string receiverAccount;
    double amount;
    string transactionID;

public:
    static int totalTransactions;
    static double totalProcessedVolume;

    paymentgateway(string senderAccount, string receiverAccount, double amount, string transactionID) {
        this->amount = amount;
        this->receiverAccount = receiverAccount;
        this->senderAccount = senderAccount;
        this->transactionID = transactionID;

        totalTransactions++;
        totalProcessedVolume += amount; //  volume adds actual amount
    }

    virtual double calculateFee() = 0;
    virtual void printReceipt() = 0;

    virtual ~paymentgateway() {} //  virtual destructor
};

// Static definitions
int paymentgateway::totalTransactions = 0;
double paymentgateway::totalProcessedVolume = 0.0;

class jazzcashGateway : public paymentgateway {
public:
    jazzcashGateway(string senderAccount, string receiverAccount, double amount, string transactionID)
        : paymentgateway(senderAccount, receiverAccount, amount, transactionID) {}

    double calculateFee() override {
        return (amount * 0.015) + 20.0;
    }

    void printReceipt() override {
        double fee = calculateFee();
        cout << "\n--- [JazzCash Receipt] ---" << endl;
        cout << "TxID: " << transactionID << " | From: " << senderAccount << " -> To: " << receiverAccount << endl;
        cout << "Amount: " << amount << " PKR | Fee: " << fee << " PKR" << endl;
        cout << "Total Deducted: " << (amount + fee) << " PKR" << endl;
    }
};

class BankTransferGateway : public paymentgateway {
private:
    string ibanNumber; // Fixed: added member

public:
    BankTransferGateway(string senderAccount, string receiverAccount, double amount, string transactionID, string ibanNumber)
        : paymentgateway(senderAccount, receiverAccount, amount, transactionID), ibanNumber(ibanNumber) {}

    double calculateFee() override {
        return (amount * 0.025) + 20.0;
    }

    void printReceipt() override {
        double fee = calculateFee();
        cout << "\n--- [Bank Transfer Receipt] ---" << endl;
        cout << "TxID: " << transactionID << " | IBAN: " << ibanNumber << endl;
        cout << "Amount: " << amount << " PKR | Fee: " << fee << " PKR" << endl;
        cout << "Total Deducted: " << (amount + fee) << " PKR" << endl;
    }
};

int main() {
    // Base class pointer array (Runtime Polymorphism)
    paymentgateway* cart[2];

    cart[0] = new jazzcashGateway("03001234567", "03219876543", 5000.0, "JC-991");
    cart[1] = new BankTransferGateway("03001234567", "HBL-Main", 20000.0, "BT-552", "PK36HABB000123456789");

    for (int i = 0; i < 2; i++) {
        cart[i]->printReceipt();
    }

    cout << "\n=====================================" << endl;
    cout << "Total Transactions: " << paymentgateway::totalTransactions << endl;
    cout << "Total Processed Volume: " << paymentgateway::totalProcessedVolume << " PKR" << endl;
    cout << "=====================================" << endl;

    // Cleanup
    for (int i = 0; i < 2; i++) {
        delete cart[i];
    }

    return 0;
}
