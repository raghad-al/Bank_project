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
    string pinCode;
    string name;
    string phone;
    float accBalance;
};

struct BankUser {
    string username;
    string password;
    int permission = 0;
};

enum enMainMenuOptions {
    eShowAllClient = 1,
    eAddClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransaction = 6,
    eManageUsers = 7,
    eLogout = 8
};

enum enTransMenuOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBanlances = 3,
    eMainMenu = 4
};

enum enManageUsers {
    eShowAllUsers = 1,
    eAddUser = 2,
    eDeleteUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eMainMenuReturn = 6,
};

enum enPermissions {
    ePShowAllClient = 1,
    ePAddClient = 2,
    ePDeleteClient = 4,
    ePUpdateClient = 8,
    ePFindClient = 16,
    ePTransaction = 32,
    ePManageUsers = 64,
};

const string fileBankClients = "BankClients.txt";
const string fileBankUsers = "BankUsers.txt";


void ShowTransMenuScreen();
void ShowLoginScreen();
void ShowManageMenuScreen();
void ShowMainMenuScreen();
void handleMainMenuSelection(enMainMenuOptions menuOption);
void handleTransMenuSelection(enTransMenuOptions menuOption);
void handleManageMenuSelection(enManageUsers menuOption);

//To check permission of the current user
BankUser currentUser;

//Color message
void GreenMessage(string message) {
    cout << "\033[32m"<< message <<"\033[0m";
}

void RedMessage(string message) {
    cout << "\033[31m" << message << "\033[0m";
}

//Display format
void DisplayTitle(string title) {
    cout << "\n*************************************\n";

    cout << title;

    cout << "\n*************************************\n";
}

void DisplayWarningToUser(string message) {
    RedMessage("\nWarning :" + message + "\n");
}

void DisplayMessageToUser(string message) {
    system("cls");
    cout << "\n-------------------------------------\n";
    cout << message << "\n";
    cout << "-------------------------------------\n\n";
}

void DisplayResultMessage(string message, bool succes) {
    if (succes) {
        GreenMessage("\nSuccess :" + message + "\n");
    }
    else {
        RedMessage("\nFailed :" + message + "\n");
    }
}

//Print Farewel Message when user don't want to do operation
void printFarewellMessage() {
    DisplayMessageToUser("Have a good day and Thank you!");
}

//Greet user according to timestamp
void greetAccordingToTime() {
    time_t t = time(0);

    tm* now = localtime(&t);

    int hour = now->tm_hour;
    
    if (hour >= 5 && hour < 12) {
        DisplayTitle("\tGood Morning!");
    }
    else if (hour >= 12 && hour < 17) {
        DisplayTitle("\tGood Afternoon!");
    }
    else if (hour >= 17 && hour < 21) {
        DisplayTitle("\tGood Evening");
    }
    else DisplayTitle("\tGood Night!");

    this_thread::sleep_for(chrono::seconds(3));

    system("cls");
}

//Confirm functions
string GetYesNoAnswer(string question) {
    string response;

    do {

        cout << "\n" << question << " (y / n)?";

        cin >> response;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (to_lower(response) != "n" && to_lower(response) != "y") {

            RedMessage("Warning: you can enter only n for no or y for yes");
        }

    } while (to_lower(response) != "n" && to_lower(response) != "y");

    return to_lower(response);
}

bool ConfirnOperation(string operation) {
    string question = "Are you sure you want to " + operation;

    string response = GetYesNoAnswer(question);

    if (response == "n") return 0;
    else return 1;
}

bool ConfirnTrans(string trans) {

    string question = "Are you sure you want to perform " + trans + " Transaction";

    string response = GetYesNoAnswer(question);

    if (response == "n") return 0;
    else return 1;
}

bool ConfirmReentry() {

    string question = "Would you like to enter data again";

    string response = GetYesNoAnswer(question);

    if (response == "n") {
        printFarewellMessage();
        return 0;
    }
    else return 1;
}

bool AskMoreEntry(string action,string typeOfEntry) {

    string question = "Do you want to "+ action +" another "+ typeOfEntry;

    string response = GetYesNoAnswer(question);

    if (response == "n") {
        printFarewellMessage();
        return 0;
    }

    else return 1;
}

//Read data
void ReadData(string& Data, string DataToRead) {
    cout << "\nPlease Enter " << DataToRead << " :";
    cin >> Data;
}

//Bank client stucture functions
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

void FillBankClient(BankClient& stBankClient, string accNum, string pinCode, string name, string phone, float accBalance) {
    stBankClient.accNum = accNum;
    stBankClient.pinCode = pinCode;
    stBankClient.name = name;
    stBankClient.phone = phone;
    stBankClient.accBalance = accBalance;
}

//Convert Data (client structure format) to Record (in file)
string ConvertDataToRecord(BankClient stBankClient, string seperator = "#//#") {

    vector<string> vClientData;

    vClientData.push_back(stBankClient.accNum);
    vClientData.push_back(stBankClient.pinCode);
    vClientData.push_back(stBankClient.name);
    vClientData.push_back(stBankClient.phone);
    vClientData.push_back(to_string(stBankClient.accBalance));

    return JoinString(vClientData, "#//#");

}

//Convert Record (in file) to Data (client structure format)
BankClient ConvertRecordToClientData(string record, string seperator = "#//#") {

    BankClient stBankClient;

    vector <string> DataClient = SplitString(record, seperator);

    FillBankClient(stBankClient, DataClient.at(0), DataClient.at(1), DataClient.at(2), DataClient.at(3), stof(DataClient.at(4)));

    return stBankClient;
}

//Find client By account number : fill the parameter of bank client structure and return if the client is found
bool FindClientByAccNumber(string accNum, BankClient& stBankClient) {
    fstream fileToRead;

    fileToRead.open(fileBankClients, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient client;

        while (getline(fileToRead, record)) {

            client = ConvertRecordToClientData(record);

            if (client.accNum == accNum) {
                stBankClient = client;

                return 1;
            }
        }

        fileToRead.close();
    }
    return 0;
}

BankClient GetClientByAccNum(string accNum) {
    fstream fileToRead;
    BankClient stBankClient;

    fileToRead.open(fileBankClients, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient client;

        while (getline(fileToRead, record)) {

            client = ConvertRecordToClientData(record);

            if (client.accNum == accNum) {
                stBankClient = client;
            }
        }

        fileToRead.close();
    }
    return stBankClient;
}

bool CheckExistingOfClient(string& accNum) {
    fstream fileToRead;

    fileToRead.open(fileBankClients, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient client;

        while (getline(fileToRead, record)) {

            client = ConvertRecordToClientData(record);

            if (client.accNum == accNum) {

                return 1;
            }
        }

        fileToRead.close();
    }
    return 0;
}

//Function that read the account number of client and check if client is exist before
//if exist before ask user to renter another account number
bool ReadAccNumOfClientNotExistBefore(string& accNum) {

    while (1) {
        ReadData(accNum, "Account Number");
        if (CheckExistingOfClient(accNum)) {
            DisplayWarningToUser("The client with the account number " + accNum + " already exist.");

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

//Function that read the account number of client and check if client isn't exist before
//if isn't exist before ask user to renter another account number
bool ReadAccNumOfClientExistBefore(string& accNum) {

    while (1) {
        ReadData(accNum, "Account Number");
        if (!CheckExistingOfClient(accNum)) {
   
            DisplayWarningToUser("The client with the account number " + accNum + " was not found.");

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

//Clients file functions
vector<BankClient> ReadFromClientFile(string FileName) {
    fstream fileToRead;

    vector<BankClient> vBankClients;

    fileToRead.open(FileName, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankClient stBankClient;

        while (getline(fileToRead, record)) {
            stBankClient = ConvertRecordToClientData(record);

            vBankClients.push_back(stBankClient);
        }

        fileToRead.close();
    }

    return vBankClients;
}

void SaveOneClientToFile(BankClient stBankClient, string FileName) {

    string record = ConvertDataToRecord(stBankClient);

    fstream myFile;

    myFile.open(FileName, ios::out | ios::app);

    if (myFile.is_open()) {
        myFile << record << endl;

        myFile.close();
    }

}

void SaveMultipleClientsToFile(vector<BankClient> vClients, string FileName) {
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

//Show all clients
void DisplayAllClients() {

    vector<BankClient> vBankClients = ReadFromClientFile(fileBankClients);

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

//CRUD opreation functions
void AddClient() {
    BankClient stBankClient;
    string accNum;

    while (1) {

        if (ReadAccNumOfClientNotExistBefore(accNum)) {

            stBankClient.accNum = accNum;

            ReadClientData(stBankClient, 0);

            SaveOneClientToFile(stBankClient, fileBankClients);

            DisplayResultMessage("Client added Successfully", 1);

            if (AskMoreEntry("add", "clients")){
                system("cls");
         
                DisplayTitle("Add New Client");
            }

            else break;
        }
        else {
            break;
        }

    }
}

void ShowDeleteClientScreen();

void DeleteClient(vector<BankClient> OldClients, string accNum) {

    vector<BankClient> NewClients;

    for (BankClient& client : OldClients) {
        if (client.accNum != accNum) {
            NewClients.push_back(client);
        }
    }

    SaveMultipleClientsToFile(NewClients, fileBankClients);
}

void DeleteClientProcess(string fileName) {
    vector<BankClient> vClients = ReadFromClientFile(fileName);

    string accNum;

    BankClient stClient;

    if (ReadAccNumOfClientExistBefore(accNum)) {
            stClient = GetClientByAccNum(accNum);

            cout << "\nData of client:\n";

            PrintClientData(stClient);

            if (ConfirnOperation("delete")) {
                DeleteClient(vClients, accNum);

                DisplayResultMessage("Client Deleted Successfully", 1);

                vClients = ReadFromClientFile(fileBankClients);
            }
          
            else  DisplayResultMessage("The deletion was canceled", 0);

            if (AskMoreEntry("delete", "client")) {
                system("cls");
                ShowDeleteClientScreen();
            }

    }
}


void ShowUpdateClientScreen();
void UpdateClient(vector<BankClient>& vClients, string accNum) {

    BankClient NewClient;

    ReadClientData(NewClient, false);

    for (BankClient& client : vClients) {
        if (client.accNum == accNum) {
            FillBankClient(client, accNum, NewClient.pinCode, NewClient.name, NewClient.phone, NewClient.accBalance);
            break;
        }
    }

    SaveMultipleClientsToFile(vClients, fileBankClients);

}

void UpdateClientProcess(string fileName) {

    vector<BankClient> vClients = ReadFromClientFile(fileName);

    string accNum;

    BankClient stClient;

    if (ReadAccNumOfClientExistBefore(accNum)) {
        stClient = GetClientByAccNum(accNum);

        cout << "\nData of client:\n";

        PrintClientData(stClient);

        if (ConfirnOperation("update")) {
             UpdateClient(vClients, accNum);

             DisplayResultMessage("Client Updated Successfully", 1);
        }
        else DisplayResultMessage("The Update was canceled", 0);

        if (AskMoreEntry("Update", "client")) {
            system("cls");
            ShowUpdateClientScreen();
        }

    }
}

//Main Menu
void PrintMainMenu() {
    DisplayTitle("Main Menu screen");

    cout << "[1] list Clients\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client\n";
    cout << "[5] Find Client Info\n";
    cout << "[6] Transaction\n";
    cout << "[7] Manage users\n";
    cout << "[8] Logout\n";

    cout << "*************************************\n";

}

short ReadMenuOption(short lastOpt) {
    short menuOpt;

    do {

        cout << "\nChoose what do you want to do[1 - " << lastOpt << "]?";
        cin >> menuOpt;

        if (menuOpt < 1 || menuOpt > lastOpt) {

            DisplayWarningToUser("You will must enter only a number between 1 and " + to_string(lastOpt));

        }

    } while (menuOpt < 1 || menuOpt > lastOpt);

    return menuOpt;

}

void GoBackToMainMenu() {
    cout << "\nPress any key to go back to the Main Menu...\n";

    system("pause>0");

    system("cls");

    ShowMainMenuScreen();
}

//Screen display of each operation in main menu
void ShowAllClientsScreen() {
    DisplayTitle("List of Clients");
    DisplayAllClients();
}

void ShowAddClientScreen() {
    DisplayTitle("Add New Client");
    AddClient();
}

void ShowDeleteClientScreen() {
    DisplayTitle("Delete Client");
    DeleteClientProcess(fileBankClients);
}

void ShowUpdateClientScreen() {
    DisplayTitle("Update Client");
    UpdateClientProcess(fileBankClients);
}

void ShowFindClientScreen() {

    DisplayTitle("Find Client Info");
    
    string accNum;
    BankClient stBankClient;


    if (ReadAccNumOfClientExistBefore(accNum)) {
        stBankClient = GetClientByAccNum(accNum);

        PrintClientData(stBankClient);
    }
}

void ShowMainMenuScreen() {
    PrintMainMenu();
    handleMainMenuSelection((enMainMenuOptions)ReadMenuOption(8));
}

//Handle Main menu selection according to user permission
bool UserHavePermission(enPermissions persmission) {

    if (currentUser.permission == -1) return 1;
    else {

        if ((persmission & currentUser.permission) == persmission) {
            return 1;
        }
        else {

            DisplayMessageToUser("Access Denied,\nYou don't have permission to do this,\nPlease contact your admin");

            return 0;
        }
    }
}

void handleMainMenuSelection(enMainMenuOptions menuOption) {

      system("cls");
      switch (menuOption) {
      case (enMainMenuOptions::eShowAllClient): {
          if (UserHavePermission(enPermissions::ePShowAllClient)) ShowAllClientsScreen();
              
          GoBackToMainMenu();
          break;
      };
        
      case (enMainMenuOptions::eAddClient):{
          if (UserHavePermission(enPermissions::ePAddClient)) ShowAddClientScreen();
              
          GoBackToMainMenu();
          break;
      };

      case (enMainMenuOptions::eDeleteClient): {
          if (UserHavePermission(enPermissions::ePDeleteClient)) ShowDeleteClientScreen();
              
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eUpdateClient): {
          if (UserHavePermission(enPermissions::ePUpdateClient)) ShowUpdateClientScreen();
              
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eFindClient): {
          if (UserHavePermission(enPermissions::ePFindClient)) ShowFindClientScreen();
              
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eTransaction): {
          if (UserHavePermission(enPermissions::ePTransaction)) ShowTransMenuScreen();
          
          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eManageUsers): {
          if (UserHavePermission(enPermissions::ePManageUsers)) ShowManageMenuScreen();

          GoBackToMainMenu();
          break;
      };

      case(enMainMenuOptions::eLogout): {
          ShowLoginScreen();
      }

      }
}


//Transaction Menu
void PrintTransMenu() {

    DisplayTitle("Transaction Menu screen");

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

//Functions of actions in transaction menu
void DepositAmountTrans(vector<BankClient> &vBankClients) {
    float depositAmount = 0;
    BankClient stClient;

    string accNum;

    if (ReadAccNumOfClientExistBefore(accNum)) {
        stClient = GetClientByAccNum(accNum);

        cout << "\nData of client:\n";

        PrintClientData(stClient);
        cout << "\nPlease enter the amount to deposit :";
        cin >> depositAmount;

        if (ConfirnTrans("deposit")) {
            for (BankClient& client : vBankClients) {
                if (client.accNum == accNum) {
                    client.accBalance = client.accBalance + depositAmount;
                    
                    DisplayResultMessage("The transaction was succesfully done!",1);

                    break;
                }
            }
        }
        else {
           
            DisplayResultMessage("The Transaction was canceled", 0);

        }
    }
}

void WithDrawAmountTrans(vector<BankClient>& vBankClients) {
    float WithDrawAmount = 0;
    string accNum;

    BankClient stClient;

    if (ReadAccNumOfClientExistBefore(accNum)) {

        stClient = GetClientByAccNum(accNum);

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
                            DisplayResultMessage("The transaction was succesfully done!", 1);
                            
                            break;
                        }
                    }
                }
                else {
                    DisplayResultMessage("The Transaction was canceled", 0);
                }
                break;
            }
            else {
                DisplayWarningToUser("Amount Exceeds The Balance ,You can withdraw up to :" + to_string(stClient.accBalance));

                if (!ConfirmReentry()) {
                    break;
                }
            }
        }
    }
}

float CalculateTotal(vector<float> accBalances) {

    float totalAccBalances = 0;

    for (float& accBalance : accBalances) {
        totalAccBalances += accBalance;
    }

    return totalAccBalances;
}

void DisplayTotalBalances() {
    vector<BankClient> vBankClients = ReadFromClientFile(fileBankClients);
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

//Screen display of each operation in transaction menu
void ShowTransMenuScreen() {
    PrintTransMenu();
    handleTransMenuSelection((enTransMenuOptions)ReadMenuOption(4));
}

void ShowDepositTransScreen() {
    vector<BankClient> vBankClients = ReadFromClientFile(fileBankClients);
    DisplayTitle("Deposit Screen");
    DepositAmountTrans(vBankClients);
    SaveMultipleClientsToFile(vBankClients, fileBankClients);
}

void ShowWithDrawTransScreen() {
    vector<BankClient> vBankClients = ReadFromClientFile(fileBankClients);
    DisplayTitle("Withdraw Screen");
    WithDrawAmountTrans(vBankClients);
    SaveMultipleClientsToFile(vBankClients, fileBankClients);
}

void ShowTotalBalancesScreen() {

    DisplayTitle("Total Balances");
    DisplayTotalBalances();

}

//handle transaction menu's options
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

//Bank user structure functions
void ReadUserData(BankUser& stBankUser) {

    ReadData(stBankUser.username, "Username");
    
    ReadData(stBankUser.password, "Password");

}

void SetUserPermissions(BankUser& stBankUser) {
    int persmissions = 0;

    if (GetYesNoAnswer("Do you want to give full access") == "y") persmissions = -1;

    else {
        if (GetYesNoAnswer("Do you want to give access to show client list") == "y") {
            persmissions += enPermissions::ePShowAllClient;
        }

        if (GetYesNoAnswer("Do you want to give access to find client") == "y") {
            persmissions += enPermissions::ePFindClient;
        }

        if (GetYesNoAnswer("Do you want to give access to delete client") == "y") {
            persmissions += enPermissions::ePDeleteClient;
        }

        if (GetYesNoAnswer("Do you want to give access to update client") == "y") {
            persmissions += enPermissions::ePUpdateClient;
        }

        if (GetYesNoAnswer("Do you want to give access to manage users") == "y") {
            persmissions += enPermissions::ePManageUsers;
        }

        if (GetYesNoAnswer("Do you want to give access to transactions") == "y") {
            persmissions += enPermissions::ePTransaction;
        }

    }

    stBankUser.permission = persmissions;
}

void PrintUserData(BankUser stBankUser) {
    cout << "--------------------------------" << endl;

    cout << setw(15) << left << "Username" << ":" << stBankUser.username << endl;

    cout << setw(15) << left << "Password" << ":" << stBankUser.password << endl;

    cout << setw(15) << left << "Permission" << ":" << stBankUser.permission << endl;

    cout << "--------------------------------";
}

void FillBankUser(BankUser& stBankUser, string username, string password, int permission) {
    stBankUser.username = username;
    stBankUser.password = password;
    stBankUser.permission = permission;
}

//convert Data (user structure format) to Record (in file)
string ConvertDataToRecord(BankUser stBankUser, string seperator = "#//#") {

    vector<string> vUserData;

    vUserData.push_back(stBankUser.username);
    vUserData.push_back(stBankUser.password);
    vUserData.push_back(to_string(stBankUser.permission));

    return JoinString(vUserData, "#//#");

}

//convert Record (in file) to Data (user structure format)
BankUser ConvertRecordToUserData(string record, string seperator = "#//#") {

    BankUser stBankUser;

    vector <string> UserData = SplitString(record, seperator);

    FillBankUser(stBankUser, UserData.at(0), UserData.at(1), stoi(UserData.at(2)));

    return stBankUser;
}

//Bank client file functions
vector<BankUser> ReadFromUserFile(string FileName) {
    fstream fileToRead;

    vector<BankUser> vBankUsers;

    fileToRead.open(FileName, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankUser stBankUser;

        while (getline(fileToRead, record)) {
            stBankUser = ConvertRecordToUserData(record);

            vBankUsers.push_back(stBankUser);
        }

        fileToRead.close();
    }

    return vBankUsers;
}

void SaveUserToFile(BankUser stBankUser, string FileName) {

    string record = ConvertDataToRecord(stBankUser);

    fstream myFile;

    myFile.open(FileName, ios::out | ios::app);

    if (myFile.is_open()) {
        myFile << record << endl;

        myFile.close();
    }

}

void SaveMultipleUsersToFile(vector<BankUser> vBankUsers, string FileName) {
    fstream myFile;
    string record;

    myFile.open(FileName, ios::out);

    if (myFile.is_open()) {

        for (BankUser& user : vBankUsers) {
            SaveUserToFile(user, FileName);
        }

        myFile.close();
    }

}

bool UserExist(BankUser user) {
    vector<BankUser> vBankUsers = ReadFromUserFile(fileBankUsers);

    for (BankUser& CurrentUser : vBankUsers) {
        if (user.username == CurrentUser.username && user.password == CurrentUser.password) {
            return 1;
        }
    }

    return 0;
}

bool UsernameExist(string username) {
    vector<BankUser> vBankUsers = ReadFromUserFile(fileBankUsers);

    for (BankUser& CurrentUser : vBankUsers) {
        if (username == CurrentUser.username) {
            return 1;
        }
    }

    return 0;
}

//Function that read the username of user and check if client is exist before
//if is exist before ask user to renter another username
bool ReadUsernameOfUserNotExistBefore(string& username) {

    while (1) {
        ReadData(username, "Username");
        if (UsernameExist(username)) {
  
            DisplayWarningToUser("The user with the username :" + username + " is already exist.");
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

//Function that read the username of user and check if client isn't exist before
//if isn't exist before ask user to renter another username
bool ReadUsernameOfUserExistBefore(string& username) {

    while (1) {
        ReadData(username, "Username");
        if (!UsernameExist(username)) {
            DisplayWarningToUser("The user with the username :" + username + " was not found.");

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

BankUser GetUserByUsername(string username) {
    fstream fileToRead;
    BankUser stBankUser;

    fileToRead.open(fileBankUsers, ios::in);

    if (fileToRead.is_open()) {
        string record;
        BankUser user;

        while (getline(fileToRead, record)) {

            user = ConvertRecordToUserData(record);

            if (user.username == username) {
                stBankUser = user;
            }
        }

        fileToRead.close();
    }
    return stBankUser;
}

//Login
void LoginProcess() {
    BankUser user;

    while (1) {

        ReadUserData(user);

        if (UserExist(user)) {
            system("cls");

            currentUser = GetUserByUsername(user.username);

            ShowMainMenuScreen();
            break;
        }
        else {
            DisplayWarningToUser("Invalid Username and/or password");

            if (!ConfirmReentry()) {
                printFarewellMessage();
                break;
            }
        }
    }
}

void ShowLoginScreen() {
    DisplayTitle("Login");
    LoginProcess();
}

//Manage User Menu
void PrintManageMenu() {

    DisplayTitle("Manage Users Menu screen");

    cout << "[1] List Users\n";
    cout << "[2] Add New User\n";
    cout << "[3] Delete User\n";
    cout << "[4] Update User\n";
    cout << "[5] Find User Info\n";
    cout << "[6] Main Menu\n";


    cout << "*************************************\n";

}

void GoBackToManageMenu() {
    cout << "\nPress any key to go back to the Manage Users Menu...\n";

    system("pause>0");

    system("cls");

    ShowManageMenuScreen();
}

//Find User And Print user data
void FindUser() {
    string username;
    BankUser user;

    if (ReadUsernameOfUserExistBefore(username)) {
        user = GetUserByUsername(username);

        cout << "\nData of User:\n";

        PrintUserData(user);
    }
    else {
        DisplayResultMessage("The user with username :" + username + " was not found.", 0);
    }
}

//Show all clients
void DisplayAllUsers() {

    vector<BankUser> vBankUsers = ReadFromUserFile(fileBankUsers);

    cout << endl << setw(40) << " " << "List of (" << vBankUsers.size() << ") Users" << endl;
    cout << setw(40) << " " << "-------------------" << endl << endl;
    cout << left << setw(20) << "Username";
    cout << "| " << left << setw(12) << "Password";
    cout << "| " << left << setw(15) << "Persmission" << "|" << endl;

    for (BankUser& user : vBankUsers) {
        cout << left << setw(20) << user.username;
        cout << "| " << left << setw(12) << user.password;
        cout << "| " << left << setw(15) << user.permission << "|" << endl;
    }
}

//CRUD opreation functions of user
void AddUser() {
    BankUser stBankUser;
    string username;

    while (1) {

        if (ReadUsernameOfUserNotExistBefore(stBankUser.username)) {
            ReadData(stBankUser.password, "password");

            SetUserPermissions(stBankUser);

            SaveUserToFile(stBankUser, fileBankUsers);

            DisplayResultMessage("Client added Successfully", 1);
            if (AskMoreEntry("add", "user")) {
                system("cls");
                DisplayTitle("Add New User");
            }

            else break;
        }
        else {
            break;
        }

    }
}

//Delete User
void ShowDeleteUserScreen();
void DeleteUser(vector<BankUser> OldUsers, string username) {

    vector<BankUser> NewUsers;

    for (BankUser& User : OldUsers) {
        if (User.username != username) {
            NewUsers.push_back(User);
        }
    }

    SaveMultipleUsersToFile(NewUsers, fileBankUsers);
}

void DeleteUserProcess(string fileName) {
    vector<BankUser> vUsers = ReadFromUserFile(fileName);
    string username;
    BankUser user;
    
    if (ReadUsernameOfUserExistBefore(username)) {
        if (username == "Admin") {
            DisplayWarningToUser("Admin can't be deleted!!");
        }

        else if (username == currentUser.username) {
            DisplayWarningToUser("You can't delete yourself!!");
        }
        else {

            user = GetUserByUsername(username);

            cout << "\nData of user:\n";

            PrintUserData(user);

            if (ConfirnOperation("delete")) {
                DeleteUser(vUsers, user.username);

                DisplayResultMessage("User Deleted Successfully", 1);
                vUsers = ReadFromUserFile(fileBankUsers);
            }
            else DisplayResultMessage("The deletion was canceled", 0);
        }

        if (AskMoreEntry("delete", "user")) {
            system("cls");
            ShowDeleteUserScreen();
        }
    }
}

//Update User
void ShowUpdateUserScreen();
void UpdateUser(vector<BankUser>& vUsers, string username) {

    BankUser NewUser;

    ReadData(NewUser.password, "password");

    SetUserPermissions(NewUser);

    for (BankUser& User : vUsers) {
        if (User.username == username) {
            FillBankUser(User, username, NewUser.password, NewUser.permission);
            break;
        }
    }

    SaveMultipleUsersToFile(vUsers, fileBankUsers);
}


void UpdateUserProcess(string fileName) {

    vector<BankUser> vUsers = ReadFromUserFile(fileName);
    string username;
    BankUser user;

    if (ReadUsernameOfUserExistBefore(username)) {
        user = GetUserByUsername(username);

        cout << "\nData of User:\n";

        PrintUserData(user);

        if (ConfirnOperation("update")) {
            UpdateUser(vUsers, user.username);

            DisplayResultMessage("User Updated Successfully", 1);

        }
        else DisplayResultMessage("The Update was canceled", 0);

        if (AskMoreEntry("update", "user")) {
            system("cls");
            ShowUpdateUserScreen();
        }

    }
}



//Screen display of each operation in manage user menu
void ShowAllUsersScreen() {
    DisplayTitle("List of Users");
    DisplayAllUsers();
}

void ShowAddUserScreen() {
    DisplayTitle("Add new user");
    AddUser();
}

void ShowManageMenuScreen() {
    PrintManageMenu();
    handleManageMenuSelection((enManageUsers)ReadMenuOption(6));
}

void ShowDeleteUserScreen() {
    DisplayTitle("Delete User");
    DeleteUserProcess(fileBankUsers);
}

void ShowUpdateUserScreen() {
    DisplayTitle("Update User");
    UpdateUserProcess(fileBankUsers);
}

void ShowFindUserScreen() {
    DisplayTitle("Find User Info");
    FindUser();
}

//handle selections of manage users menu
void handleManageMenuSelection(enManageUsers menuOption) {
    system("cls");

    switch (menuOption) {
    case(enManageUsers::eShowAllUsers): {
        ShowAllUsersScreen();
        GoBackToManageMenu();
        break;
    }

    case(enManageUsers::eAddUser): {
        ShowAddUserScreen();
        GoBackToManageMenu();
        break;
    }

    case(enManageUsers::eDeleteUser): {
        ShowDeleteUserScreen();
        GoBackToManageMenu();
        break;
    }

    case(enManageUsers::eUpdateUser): {
        ShowUpdateUserScreen();
        GoBackToManageMenu();
        break;
    }

    case(enManageUsers::eFindUser): {
        ShowFindUserScreen();
        GoBackToManageMenu();
        break;
    }

    case(enManageUsers::eMainMenuReturn): {
        ShowMainMenuScreen();
    }

    }
}

int main()
{
    greetAccordingToTime();

    ShowLoginScreen();
   
    system("pause>0");

    return 0;
}

