// Bank1_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct BankClient {
    string accNum;
    short pinCode;
    string name;
    string phone;
    float accBalance;
};

enum enMenuOption {
    eShowAllClient = 1,
    eAddClient = 2,
    eDeleteClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eExit
};

string fileBankClients = "BankClient.txt";

//String functions

bool is_upper(char c) {
    return (int(c) >= int('A') && int(c) <= int('Z'));
}

bool is_lower(char c) {
    return (int(c) >= int('a') && int(c) <= int('z'));
}

bool is_letter(char c) {
    return (is_upper(c) || is_lower(c));
}

char to_lower(char c) {

    if ((int)c >= (int)'a' && (int)c <= (int)'z') return c;

    return (int)c + 32;
}

string to_lower(string str) {
    string strLower = "";

    for (short i = 0; i < str.length(); i++) {
        strLower.push_back(to_lower(str[i]));
    }

    return strLower;
}

char to_upper(char c) {

    if ((int)c >= (int)'A' && (int)c <= (int)'Z') return c;

    return (int)c - 32;
}

string to_upper(string str) {
    string strUpper = "";

    for (short i = 0; i < str.length(); i++) {
        strUpper.push_back(to_upper(str[i]));
    }

    return strUpper;
}

vector<string> SplitString(string orgStr, string delim = " ") {
    short pos = 0;
    string word = "";
    vector<string> vWords;

    while ((pos = orgStr.find(delim)) != string::npos) {
        word = orgStr.substr(0, pos);

        vWords.push_back(word);

        orgStr.erase(0, pos + delim.length());

    }

    if (orgStr != "") vWords.push_back(orgStr);

    return vWords;
}

//Convert vector to string
string JoinString(vector<string>vElmnts, string seperator = " ") {

    string str = "";

    for (string& elmnt : vElmnts) {
        str = str + elmnt;

        if (elmnt != vElmnts.back()) str = str + seperator;
    }

    return str;
}

//Convert array to string
string JoinString(string arrElmnts[], short length, string seperator = " ") {

    string str = "";

    for (short i = 0; i < length; i++) {

        str = str + arrElmnts[i];

        if (i != length - 1) str = str + seperator;
    }

    return str;
}
//Color message

void GreenMessage(string message) {
    cout << "\n\033[32m"<< message <<"\033[0m\n";
}

void RedMessage(string message) {
    cout << "\n\033[31m" << message << "\033[0m\n";
}

//structure Bank client function
void ReadData(string& Data, string DataToRead) {
    cout << "Please Enter " << DataToRead << " :";
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

//file function
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

//CRUD opreation

char GetYesNoAnswer(string question) {
    char response;

    do {

        cout << "\n" << question << " (y / n)?";

        cin >> response;

        if (to_lower(response) != 'n' && to_lower(response) != 'y') {

            RedMessage("Warning: you can enter only n for no or y for yes");
        }

    } while (to_lower(response) != 'n' && to_lower(response) != 'y');

    return to_lower(response);
}

bool AffirmeOperation(string operation) {
    string question = "Are you sure you want to " + operation;

    char response = GetYesNoAnswer(question);

    if (response == 'n') return 0;
    else return 1;
}

bool AskMoreClient() {

    string question = "Do you want to add more clients";

    char response = GetYesNoAnswer(question);

    if (response == 'n') return 0;
    else return 1;
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

string ReadAndCheckExitingOfAccNum() {
    string accNum;
    BankClient stClient;

    cout << "Please enter Account Number :";
    cin >> accNum;
    while (FindClientByAccNumber(accNum, stClient)) {

        RedMessage("The client with the account number " + accNum + " already exist , Please Enter Another :");
        
        cin >> accNum;
    }

    return accNum;
}

void AddClient() {
    BankClient stBankClient;
    string accNum;

    while (1) {
        accNum = ReadAndCheckExitingOfAccNum();

        stBankClient.accNum = accNum;

        ReadClientData(stBankClient , 0);

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

    ReadData(accNum, "Account Number");


    if (FindClientByAccNumber(accNum, stClient)) {

        cout << "\nData of client:\n";

        PrintClientData(stClient);

        if (AffirmeOperation("delete")) {
            DeleteClient(vClients, accNum);

            GreenMessage("Client Deleted Successfully");


            vClients = ReadFromFile(fileBankClients);
        }
        else RedMessage( "The deletion was canceled");

    }
    else {

        RedMessage("The client with the account number " + accNum + " was not found");

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

    ReadData(accNum, "Account Number");


    if (FindClientByAccNumber(accNum, stClient)) {

        cout << "\nData of client:\n";

        PrintClientData(stClient);

        if (AffirmeOperation("update")) {
            UpdateClient(vClients, accNum);

            GreenMessage("Client Updated Successfully");

        }
        else RedMessage("The Update was canceled");

    }
    else {
        RedMessage("The client with the account number " + accNum + " was not found");
    }

}

//Screen display of each operation

void DisplayTitle(string title) {
    cout << "\n*************************************\n";

    cout << title;
        
    cout << "\n*************************************\n";
}

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

    cout << "Please enter Account Number :";
    cin >> accNum;

    if (FindClientByAccNumber(accNum, stBankClient)) {
        PrintClientData(stBankClient);
    }
    else {
        cout << "The client with the account number " << accNum << " dosen't found";
    }
}

//Menu
void PrintMenu() {

    cout << "*************************************\n";
    cout << "Main Menu screen\n";
    cout << "*************************************\n";

    cout << "[1] Show Clients list\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client\n";
    cout << "[5] Find Clients Info\n";
    cout << "[6] Exit\n";

    cout << "*************************************\n";

}

short ReadMenuOption() {
    short menuOpt;

    do {

       cout << "\nChoose what do you want to do[1 - 6]?";
       cin >> menuOpt;

       if (menuOpt < 1 || menuOpt > 6) {

           RedMessage("Warrning: You will must enter only a number between 1 and 6");
        
       }

    } while (menuOpt < 1 || menuOpt > 6);

    return menuOpt;
  
}

void handleMenuSelection(enMenuOption menuOption);

void ShowMainMenuWithOption() {
    PrintMenu();
    handleMenuSelection((enMenuOption)ReadMenuOption());
}

void GoBackToMainMenu() {
    cout << "\nPress any key to go back to the Main Menu...\n";

    system("pause>0");

    system("cls");

    ShowMainMenuWithOption();
}

void handleMenuSelection(enMenuOption menuOption) {

      system("cls");
      switch (menuOption) {
      case (enMenuOption::eShowAllClient): {

          ShowAllClientsScreen();
          break;
      };
        
      case (enMenuOption::eAddClient):{

          ShowAddClientScreen();
          break;
      };

      case (enMenuOption::eDeleteClient): {

          ShowDeleteClientScreen();
          break;
      };

      case(enMenuOption::eUpdateClient): {

          ShowUpdateClientScreen();
          break;
      };

      case(enMenuOption::eFindClient): {
          ShowFindClientScreen();
          break;
      };
      case(enMenuOption::eExit): {
          DisplayTitle("Program End");
          exit(0);
      }

      }
    

    GoBackToMainMenu();

}

int main()
{
    ShowMainMenuWithOption();
   
    system("pause>0");

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
