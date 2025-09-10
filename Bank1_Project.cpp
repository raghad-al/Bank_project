#pragma warning(disable:4996)

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>
#include "stringra.h"

using namespace std;
using namespace check;
using namespace convert;
using namespace formating;

struct BankClient {
    string accNum;
    short pinCode;
    string name;
    string phone;
    float accBalance;
};

enum enMainMenuOptions {
    eShowAllClient = 1,
    eAddClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransaction = 6,
    eExit = 7
};

enum enTransMenuOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBanlances = 3,
    eMainMenu = 4
};

string fileBankClients = "BankClient.txt";

void ShowTransMenuScreen();



//Color message
void GreenMessage(string message) {
    cout << "\n\033[32m"<< message <<"\033[0m\n";
}

void RedMessage(string message) {
    cout << "\n\033[31m" << message << "\033[0m\n";
}

//Display format
void DisplayTitle(string title) {
    cout << "\n*************************************\n";

    cout << title;

    cout << "\n*************************************\n";
}

void printFarewellMessage() {
    cout << "\n****************************************\n";
    cout << "        Have a good day and Thank you!  \n";
    cout << "****************************************\n\n";
}

void greetAccordingToTime() {
    time_t t = time(0);

    tm* now = localtime(&t);

    int hour = now->tm_hour;
    
    if (hour > 5 && hour < 12) {
        DisplayTitle("\tGood Morning!");
    }
    else if (hour > 12 && hour < 17) {
        DisplayTitle("\tGood Afternoon!");
    }
    else if (hour > 17 && hour < 21) {
        DisplayTitle("\tGood Evening");
    }
    else DisplayTitle("\tGood Night!");

    this_thread::sleep_for(chrono::seconds(3));

    system("cls");
}

//Confirm functions
char GetYesNoAnswer(string question) {
    char response;

    do {

        cout << "\n" << question << " (y / n)?";

        cin >> response;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (to_lower(response) != 'n' && to_lower(response) != 'y') {

            RedMessage("Warning: you can enter only n for no or y for yes");
        }

    } while (to_lower(response) != 'n' && to_lower(response) != 'y');

    return to_lower(response);
}

bool ConfirnOperation(string operation) {
    string question = "Are you sure you want to " + operation;

    char response = GetYesNoAnswer(question);

    if (response == 'n') return 0;
    else return 1;
}

bool ConfirnTrans(string trans) {

    string question = "Are you sure you want to perform " + trans + " Transaction";

    char response = GetYesNoAnswer(question);

    if (response == 'n') return 0;
    else return 1;
}

bool ConfirmReentry() {

    string question = "Would you like to enter the data again";

    char response = GetYesNoAnswer(question);

    if (response == 'n') {
        printFarewellMessage();
        return 0;
    }
    else return 1;
}

bool AskMoreClient() {

    string question = "Do you want to add more clients";

    char response = GetYesNoAnswer(question);

    if (response == 'n') return 0;
    else return 1;
}

//structure Bank client functions
void ReadData(string& Data, string DataToRead) {
    cout << "\nPlease Enter " << DataToRead << " :";
    cin >> Data;
}

void ReadClientData(BankClient& stBankClient, bool withAccNum = 1) {

    if (withAccNum) {
        cout << "Account Number :";
        cin >> stBankClient.accNum;
    }

    cout << "Pin code :";
    cin >> stBankClient.pinCode;

    cout << "Name :";
    getline(cin >> ws, stBankClient.name);

    cout << "Phone :";
    getline(cin, stBankClient.phone);

    cout << "Account Balance :";
    cin >> stBankClient.accBalance;
}

void PrintClientData(BankClient stBankClient) {
    cout << "--------------------------------" << endl;

    cout << setw(15) << left <<"Account Number" << ":" << stBankClient.accNum << endl;

    cout << setw(15) << left << "Pin code" << ":" << stBankClient.pinCode << endl;

    cout << setw(15) << left << "Name" << ":" << stBankClient.name << endl;

    cout << setw(15) << left << "Phone" << ":" << stBankClient.phone << endl;

    cout << setw(15) << left << "Account Balance" << ":" << stBankClient.accBalance << endl;

    cout << "--------------------------------";
}

void FillBankClient(BankClient& stBankClient, string accNum, short pinCode, string name, string phone, float accBalance) {
    stBankClient.accNum = accNum;
    stBankClient.pinCode = pinCode;
    stBankClient.name = name;
    stBankClient.phone = phone;
    stBankClient.accBalance = accBalance;
}

string ConvertDataToRecord(BankClient stBankClient, string seperator = "#//#") {

    vector<string> vClientData;

    vClientData.push_back(stBankClient.accNum);
    vClientData.push_back(to_string(stBankClient.pinCode));
    vClientData.push_back(stBankClient.name);
    vClientData.push_back(stBankClient.phone);
    vClientData.push_back(to_string(stBankClient.accBalance));

    return JoinString(vClientData, "#//#");

}

BankClient ConvertRecordToData(string record, string seperator = "#//#") {

    BankClient stBankClient;

    vector <string> DataClient = SplitString(record, seperator);

    FillBankClient(stBankClient, DataClient.at(0), stoi(DataClient.at(1)), DataClient.at(2), DataClient.at(3), stof(DataClient.at(4)));

    return stBankClient;
}

bool FindClientByAccNumber(string accNum, BankClient& stBankClient) {
    fstream fileToRead;

    fileToRead.open(fileBankClients, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient client;

        while (getline(fileToRead, record)) {

            client = ConvertRecordToData(record);

            if (client.accNum == accNum) {
                stBankClient = client;

                return 1;
            }
        }

        fileToRead.close();
    }
    return 0;
}

BankClient ReturnClientByAccNum(string accNum) {
    fstream fileToRead;
    BankClient stBankClient;

    fileToRead.open(fileBankClients, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient client;

        while (getline(fileToRead, record)) {

            client = ConvertRecordToData(record);

            if (client.accNum == accNum) {
                stBankClient = client;
            }
        }

        fileToRead.close();
    }
    return stBankClient;
}

bool CheckExistingOfAccNum(string& accNum) {
    fstream fileToRead;

    fileToRead.open(fileBankClients, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient client;

        while (getline(fileToRead, record)) {

            client = ConvertRecordToData(record);

            if (client.accNum == accNum) {

                return 1;
            }
        }

        fileToRead.close();
    }
    return 0;
}

bool ReadAccNumOfClientNotExistBefore(string& accNum) {

    while (1) {
        ReadData(accNum, "Account Number");
        if (CheckExistingOfAccNum(accNum)) {
            RedMessage("The client with the account number " + accNum + " already exist.");

            if (!ConfirmReentry()) {
                break;
            }
        }
        else {
            return 1;
            break;
        }
    }
    return 0;
}

bool ReadAccNumOfClientExistBefore(string& accNum) {

    while (1) {
        ReadData(accNum, "Account Number");
        if (!CheckExistingOfAccNum(accNum)) {
            RedMessage("The client with the account number " + accNum + " was not found.");

            if (!ConfirmReentry()) {
                break;
            }
        }
        else {
            return 1;
            break;
        }
    }
    return 0;
}

//file functions
void SaveClientToFile(BankClient stBankClient, string FileName) {

    string record = ConvertDataToRecord(stBankClient);

    fstream myFile;

    myFile.open(FileName, ios::out | ios::app);

    if (myFile.is_open()) {
        myFile << record << endl;

        myFile.close();
    }

}

void SaveNewClientsData(vector<BankClient> vClients, string FileName) {
    fstream myFile;
    string record;

    myFile.open(FileName, ios::out);

    if (myFile.is_open()) {

        for (BankClient& client : vClients) {
            record = ConvertDataToRecord(client);

            myFile << record << endl;
        }

        myFile.close();
    }

}

vector<BankClient> ReadFromFile(string FileName) {
    fstream fileToRead;

    vector<BankClient> vBankClients;

    fileToRead.open(FileName, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient stBankClient;

        while (getline(fileToRead, record)) {
            stBankClient = ConvertRecordToData(record);

            vBankClients.push_back(stBankClient);
        }

        fileToRead.close();
    }

    return vBankClients;
}

//CRUD opreation functions

void AddClient() {
    BankClient stBankClient;
    string accNum;

    while (1) {

        if (ReadAccNumOfClientNotExistBefore(accNum)) {

            stBankClient.accNum = accNum;

            ReadClientData(stBankClient, 0);

            SaveClientToFile(stBankClient, fileBankClients);

            GreenMessage("Client added Successfully");

            if (AskMoreClient()) {
                system("cls");
                cout << "*************************************\n";
                cout << "Add New Client\n";
                cout << "*************************************\n";
            }

            else break;
        }
        else {
            break;
        }

    }
}

//Show all clients
void DisplayAllClients() {

    vector<BankClient> vBankClients = ReadFromFile(fileBankClients);

    cout << endl << setw(40) << " " << "List of (" << vBankClients.size() << ") clients" << endl;
    cout << setw(40) << " " << "-------------------" << endl << endl;
    cout << left << setw(20) << "Account Number";
    cout << "| " << left << setw(12) << "Pin Code";
    cout << "| " << left << setw(35) << "Client Name";
    cout << "| " << left << setw(13) << "Phone";
    cout << "| " << left << setw(15) << "Balance" << "|" << endl;

    for (BankClient& client : vBankClients) {
        cout <<left << setw(20) << client.accNum;
        cout << "| " << left << setw(12) << client.pinCode;
        cout << "| " << left << setw(35) << client.name;
        cout << "| " << left << setw(13) << client.phone;
        cout << "| " << left << setw(15) << client.accBalance << "|" << endl;
    }
}

void DeleteClient(vector<BankClient> OldClients, string accNum) {

    vector<BankClient> NewClients;

    for (BankClient& client : OldClients) {
        if (client.accNum != accNum) {
            NewClients.push_back(client);
        }
    }

    SaveNewClientsData(NewClients, fileBankClients);
}

void DeleteProcess(string fileName) {
    vector<BankClient> vClients = ReadFromFile(fileName);

    string accNum;

    BankClient stClient;

    if (ReadAccNumOfClientExistBefore(accNum)) {
            stClient = ReturnClientByAccNum(accNum);

            cout << "\nData of client:\n";

            PrintClientData(stClient);

            if (ConfirnOperation("delete")) {
                DeleteClient(vClients, accNum);

                GreenMessage("Client Deleted Successfully");

                vClients = ReadFromFile(fileBankClients);
            }
            else RedMessage("The deletion was canceled");

    }
}

void UpdateClient(vector<BankClient>& vClients, string accNum) {

    BankClient NewClient;

    ReadClientData(NewClient, false);

    for (BankClient& client : vClients) {
        if (client.accNum == accNum) {
            FillBankClient(client, accNum, NewClient.pinCode, NewClient.name, NewClient.phone, NewClient.accBalance);
            break;
        }
    }

    SaveNewClientsData(vClients, fileBankClients);
}

void UpdateProcess(string fileName) {

    vector<BankClient> vClients = ReadFromFile(fileName);

    string accNum;

    BankClient stClient;

    if (ReadAccNumOfClientExistBefore(accNum)) {
        stClient = ReturnClientByAccNum(accNum);

        cout << "\nData of client:\n";

        PrintClientData(stClient);

        if (ConfirnOperation("update")) {
             UpdateClient(vClients, accNum);

             GreenMessage("Client Updated Successfully");

        }
        else RedMessage("The Update was canceled");


    }
}

//Screen display of each operation in main menu

void ShowAllClientsScreen() {
    DisplayTitle("Clients list");
    DisplayAllClients();
}

void ShowAddClientScreen() {
    DisplayTitle("Add New Client");
    AddClient();
}

void ShowDeleteClientScreen() {
    DisplayTitle("Delete Client");
    DeleteProcess(fileBankClients);
}

void ShowUpdateClientScreen() {
    DisplayTitle("Update Client");
    UpdateProcess(fileBankClients);
}

void ShowFindClientScreen() {

    DisplayTitle("Find Client Info");
    
    string accNum;
    BankClient stBankClient;


    if (ReadAccNumOfClientExistBefore(accNum)) {
        stBankClient = ReturnClientByAccNum(accNum);

        PrintClientData(stBankClient);
    }
}

//Main Menu
void PrintMainMenu() {

    cout << "*************************************\n";
    cout << "Main Menu screen\n";
    cout << "*************************************\n";

    cout << "[1] Show Clients list\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client\n";
    cout << "[5] Find Clients Info\n";
    cout << "[6] Transaction\n";
    cout << "[7] Exit\n";

    cout << "*************************************\n";

}

short ReadMenuOption(short lastOpt) {
    short menuOpt;

    do {

       cout << "\nChoose what do you want to do[1 - "<< lastOpt << "]?";
       cin >> menuOpt;

       if (menuOpt < 1 || menuOpt > lastOpt) {

           RedMessage("Warrning: You will must enter only a number between 1 and " + to_string(lastOpt));
        
       }

    } while (menuOpt < 1 || menuOpt > lastOpt);

    return menuOpt;
  
}

void handleMainMenuSelection(enMainMenuOptions menuOption);

void ShowMainMenuScreen() {
    PrintMainMenu();
    handleMainMenuSelection((enMainMenuOptions)ReadMenuOption(7));
}

void GoBackToMainMenu() {
    cout << "\nPress any key to go back to the Main Menu...\n";

    system("pause>0");

    system("cls");

    ShowMainMenuScreen();
}

void handleMainMenuSelection(enMainMenuOptions menuOption) {

      system("cls");
      switch (menuOption) {
      case (enMainMenuOptions::eShowAllClient): {

          ShowAllClientsScreen();
          GoBackToMainMenu();
          break;
      };
        
      case (enMainMenuOptions::eAddClient):{

          ShowAddClientScreen();
          GoBackToMainMenu();
          break;
      };

      case (enMainMenuOptions::eDeleteClient): {

          ShowDeleteClientScreen();
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eUpdateClient): {

          ShowUpdateClientScreen();
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eFindClient): {
          ShowFindClientScreen();
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eTransaction): {
          ShowTransMenuScreen();
          break;
      };

      case(enMainMenuOptions::eExit): {
          DisplayTitle("Program End");
          exit(0);
      }

      }
}


//transaction Menu

void PrintTransMenu() {

    cout << "*************************************\n";
    cout << "Transaction Menu screen\n";
    cout << "*************************************\n";

    cout << "[1] Deposit\n";
    cout << "[2] Withdraw\n";
    cout << "[3] Total Balances\n";
    cout << "[4] Return to Main Menu\n";

    cout << "*************************************\n";

}

void GoBackToTransMenu() {
    cout << "\nPress any key to go back to the Transaction Menu...\n";

    system("pause>0");

    system("cls");

    ShowTransMenuScreen();
}

void handleTransMenuSelection(enTransMenuOptions menuOption);

void ShowTransMenuScreen() {
    PrintTransMenu();
    handleTransMenuSelection((enTransMenuOptions)ReadMenuOption(4));
}

//transactions functions
void DepositAmountTrans(vector<BankClient> &vBankClients) {
    float depositAmount = 0;
    BankClient stClient;

    string accNum;

    if (ReadAccNumOfClientExistBefore(accNum)) {
        stClient = ReturnClientByAccNum(accNum);

        cout << "\nData of client:\n";

        PrintClientData(stClient);
        cout << "\nPlease enter the amount to deposit :";
        cin >> depositAmount;

        if (ConfirnTrans("deposit")) {
            for (BankClient& client : vBankClients) {
                if (client.accNum == accNum) {
                    client.accBalance = client.accBalance + depositAmount;
                    GreenMessage("\nThe transaction was succesfully done!");
                    break;
                }
            }
        }
        else {
            RedMessage("\nThe Transaction was canceled");
        }
    }
}

void ShowDepositTransScreen() {
    vector<BankClient> vBankClients = ReadFromFile(fileBankClients);
    DisplayTitle("Deposit Screen");
    DepositAmountTrans(vBankClients);
    SaveNewClientsData(vBankClients, fileBankClients);
}

void WithDrawAmountTrans(vector<BankClient>& vBankClients) {
    float WithDrawAmount = 0;
    string accNum;

    BankClient stClient;

    if (ReadAccNumOfClientExistBefore(accNum)) {

        stClient = ReturnClientByAccNum(accNum);

        cout << "\nData of client:\n";

        PrintClientData(stClient);


        while (1) {

            cout << "\nPlease enter the withdraw amount :";
            cin >> WithDrawAmount;

            if (WithDrawAmount <= stClient.accBalance) {
                if (ConfirnTrans("WithDraw")) {
                    for (BankClient& client : vBankClients) {
                        if (client.accNum == stClient.accNum) {
                            client.accBalance = client.accBalance - WithDrawAmount;
                            GreenMessage("\nThe transaction was succesfully done!");
                            break;
                        }
                    }
                }
                else {
                    RedMessage("\nThe Transaction was canceled");
                }
                break;
            }
            else {
                RedMessage("Amount Exceeds The Balance ,You can withdraw up to :" + to_string(stClient.accBalance));
                if (!ConfirmReentry()) {
                    break;
                }
            }
        }
    }
}

void ShowWithDrawTransScreen() {
    vector<BankClient> vBankClients = ReadFromFile(fileBankClients);
    DisplayTitle("Withdraw Screen");
    WithDrawAmountTrans(vBankClients);
    SaveNewClientsData(vBankClients, fileBankClients);
}

float CalculateTotal(vector<float> accBalances) {

    float totalAccBalances = 0;

    for (float& accBalance : accBalances) {
        totalAccBalances += accBalance;
    }

    return totalAccBalances;
}

void DisplayTotalBalances() {
    vector<BankClient> vBankClients = ReadFromFile(fileBankClients);
    vector<float> accBalances;

    cout << endl << setw(40) << " " << "Balances List of (" << vBankClients.size() << ") client(s)" << endl;
    cout << right << setw(40) << " " << "------------------------------" << endl << endl;
    cout << setw(20) << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(35) << "Client Name";
    cout << "| " << left << setw(15) << "Balance" << "|" << endl;

    for (BankClient& client : vBankClients) {
        cout << right << setw(20) << "| " << left << setw(20) << client.accNum;
  
        cout << "| " << left << setw(35) << client.name;
        
        cout << "| " << left << setw(15) << client.accBalance << "|" << endl;
    }

    for (BankClient& client : vBankClients) {
        accBalances.push_back(client.accBalance);
    }

    cout << endl << setw(40) << " " << "Total Balances = " << CalculateTotal(accBalances);
}

void ShowTotalBalancesScreen() {

    DisplayTitle("Total Balances");
    DisplayTotalBalances();

}

// handle transaction menu's options
void handleTransMenuSelection(enTransMenuOptions menuOption) {
    system("cls");

    switch (menuOption) {
    case(enTransMenuOptions::eDeposit): {
        ShowDepositTransScreen();
        GoBackToTransMenu();
        break;
    }

    case(enTransMenuOptions::eWithdraw): {
        ShowWithDrawTransScreen();
        GoBackToTransMenu();
        break;
    }

    case(enTransMenuOptions::eTotalBanlances): {
        ShowTotalBalancesScreen();
        GoBackToTransMenu();
        break;
    }

    case(enTransMenuOptions::eMainMenu): {
        ShowMainMenuScreen();
    }
    }
}

int main()
{
    greetAccordingToTime();

    ShowMainMenuScreen();
   
    system("pause>0");

    return 0;
}

