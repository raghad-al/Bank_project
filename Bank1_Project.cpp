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
void handleManageMenuSelection(enManageUsers menuOption);

//to check permission of the current user
BankUser currentUser;

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
    system("cls");
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

    string question = "Would you like to enter data again";

    char response = GetYesNoAnswer(question);

    if (response == 'n') {
        printFarewellMessage();
        return 0;
    }
    else return 1;
}

bool AskMoreEntry(string typeOfEntry) {

    string question = "Do you want to add more "+ typeOfEntry;

    char response = GetYesNoAnswer(question);

    if (response == 'n') return 0;
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

string ConvertDataToRecord(BankClient stBankClient, string seperator = "#//#") {

    vector<string> vClientData;

    vClientData.push_back(stBankClient.accNum);
    vClientData.push_back(stBankClient.pinCode);
    vClientData.push_back(stBankClient.name);
    vClientData.push_back(stBankClient.phone);
    vClientData.push_back(to_string(stBankClient.accBalance));

    return JoinString(vClientData, "#//#");

}

BankClient ConvertRecordToClientData(string record, string seperator = "#//#") {

    BankClient stBankClient;

    vector <string> DataClient = SplitString(record, seperator);

    FillBankClient(stBankClient, DataClient.at(0), DataClient.at(1), DataClient.at(2), DataClient.at(3), stof(DataClient.at(4)));

    return stBankClient;
}

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

bool CheckExistingOfAccNum(string& accNum) {
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

//Client file function
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

            SaveClientToFile(stBankClient, fileBankClients);

            GreenMessage("Client added Successfully");

            if (AskMoreEntry("clients")){
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

void DeleteClient(vector<BankClient> OldClients, string accNum) {

    vector<BankClient> NewClients;

    for (BankClient& client : OldClients) {
        if (client.accNum != accNum) {
            NewClients.push_back(client);
        }
    }

    SaveNewClientsData(NewClients, fileBankClients);
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

                GreenMessage("Client Deleted Successfully");

                vClients = ReadFromClientFile(fileBankClients);
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
    cout << "[7] Manage users\n";
    cout << "[8] Logout\n";

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
    handleMainMenuSelection((enMainMenuOptions)ReadMenuOption(8));
}

void GoBackToMainMenu() {
    cout << "\nPress any key to go back to the Main Menu...\n";

    system("pause>0");

    system("cls");

    ShowMainMenuScreen();
}

bool UserHavePermission(enPermissions persmission) {

    if (currentUser.permission == -1) return 1;
    else {

        if ((persmission & currentUser.permission) == persmission) {
            return 1;
        }
        else {
            RedMessage("-----------------------------------------");
            RedMessage("Access Denied,\nYou don't have permission to do this,\nPlease contact your admin");
            RedMessage("-----------------------------------------");

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
        stClient = GetClientByAccNum(accNum);

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
    vector<BankClient> vBankClients = ReadFromClientFile(fileBankClients);
    DisplayTitle("Deposit Screen");
    DepositAmountTrans(vBankClients);
    SaveNewClientsData(vBankClients, fileBankClients);
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
    vector<BankClient> vBankClients = ReadFromClientFile(fileBankClients);
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

//Bank user structure functions
void ReadUserData(BankUser& stBankUser) {

    ReadData(stBankUser.username, "Username");
    
    ReadData(stBankUser.password, "Password");

}

void SetUserPermissions(BankUser& stBankUser) {
    int persmissions = 0;

    if (GetYesNoAnswer("Do you want to give full access") == 'y') persmissions = -1;

    else {
        if (GetYesNoAnswer("Do you want to give access to show client list") == 'y') {
            persmissions += enPermissions::ePShowAllClient;
        }

        if (GetYesNoAnswer("Do you want to give access to find client") == 'y') {
            persmissions += enPermissions::ePFindClient;
        }

        if (GetYesNoAnswer("Do you want to give access to delete client") == 'y') {
            persmissions += enPermissions::ePDeleteClient;
        }

        if (GetYesNoAnswer("Do you want to give access to update client") == 'y') {
            persmissions += enPermissions::ePUpdateClient;
        }

        if (GetYesNoAnswer("Do you want to give access to manage users") == 'y') {
            persmissions += enPermissions::ePManageUsers;
        }

        if (GetYesNoAnswer("Do you want to give access to transactions") == 'y') {
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

//converting functions 
string ConvertDataToRecord(BankUser stBankUser, string seperator = "#//#") {

    vector<string> vUserData;

    vUserData.push_back(stBankUser.username);
    vUserData.push_back(stBankUser.password);
    vUserData.push_back(to_string(stBankUser.permission));

    return JoinString(vUserData, "#//#");

}

BankUser ConvertRecordToUserData(string record, string seperator = "#//#") {

    BankUser stBankUser;

    vector <string> UserData = SplitString(record, seperator);

    FillBankUser(stBankUser, UserData.at(0), UserData.at(1), stoi(UserData.at(2)));

    return stBankUser;
}

//Bank client file function
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

void SaveUsersToFile(vector<BankUser> vBankUsers, string FileName) {
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

bool ReadUsernameOfUserNotExistBefore(string& username) {

    while (1) {
        ReadData(username, "Username");
        if (UsernameExist(username)) {
            RedMessage("The user with the username :" + username + " is already exist.");

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

bool ReadUsernameOfUserExistBefore(string& username) {

    while (1) {
        ReadData(username, "Username");
        if (!UsernameExist(username)) {
            RedMessage("The user with the username :" + username + " was not found.");

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

            PrintUserData(currentUser);
            cout << endl;
            ShowMainMenuScreen();
            break;
        }
        else {
            RedMessage("Invalid Username and/or password");

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

    cout << "*************************************\n";
    cout << "Manage Users Menu screen\n";
    cout << "*************************************\n";

    cout << "[1] List Users\n";
    cout << "[2] Add New User\n";
    cout << "[3] Delete User\n";
    cout << "[4] Update User\n";
    cout << "[5] Find User\n";
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
        RedMessage("The user with username :" + username + " was not found.");
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

//CRUD opreation functions
void AddUser() {
    BankUser stBankUser;
    string username;

    while (1) {

        if (ReadUsernameOfUserNotExistBefore(stBankUser.username)) {
            ReadData(stBankUser.password, "password");

            SetUserPermissions(stBankUser);

            SaveUserToFile(stBankUser, fileBankUsers);

            GreenMessage("Client added Successfully");

            if (AskMoreEntry("Users")) {
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
void DeleteUser(vector<BankUser> OldUsers, string username) {

    vector<BankUser> NewUsers;

    for (BankUser& User : OldUsers) {
        if (User.username != username) {
            NewUsers.push_back(User);
        }
    }

    SaveUsersToFile(NewUsers, fileBankUsers);
}

void DeleteUserProcess(string fileName) {
    vector<BankUser> vUsers = ReadFromUserFile(fileName);
    string username;
    BankUser user;

    if (ReadUsernameOfUserExistBefore(username)) {
        if (username == "Admin") {
            RedMessage("Admin can't be deleted!!");
        }

        else if (username == currentUser.username) {
            RedMessage("You can't delete yourself!!");
        }
        else {

            user = GetUserByUsername(username);

            cout << "\nData of user:\n";

            PrintUserData(user);

            if (ConfirnOperation("delete")) {
                DeleteUser(vUsers, user.username);

                GreenMessage("User Deleted Successfully");

                vUsers = ReadFromUserFile(fileBankUsers);
            }
            else RedMessage("The deletion was canceled");
        }
    }
}

//Update User
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

    SaveUsersToFile(vUsers, fileBankUsers);
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

            GreenMessage("User Updated Successfully");

        }
        else RedMessage("The Update was canceled");

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
    DisplayTitle("Delete user");
    DeleteUserProcess(fileBankUsers);
}

void ShowUpdateUserScreen() {
    DisplayTitle("Update user");
    UpdateUserProcess(fileBankUsers);
}

void ShowFindUserScreen() {
    DisplayTitle("Find user");
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

