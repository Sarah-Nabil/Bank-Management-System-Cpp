
#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions { eListClients = 1, eAddNewClients = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5, eTransactions = 6, eExit = 7 };
enum enTransactionsOpition { eDeposit = 1, eWithdraw = 2, eShowtotalbalance = 3, eShowMainMenue = 4 };

void ShowMainMenue();
void ShowTransactionsMenue();

struct stClient
{
	string AccountNumber;
	string Name;
	string PinCode;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

vector<string>SpliteString(string Line, string delim)
{

	vector<string>Vstring;
	int pos = 0;
	string word;

	while ((pos = Line.find(delim)) != std::string::npos)
	{

		word = Line.substr(0, pos);
		if (word != "")
		{

			Vstring.push_back(word);


		}

		Line.erase(0, pos + delim.length());

	}


	if (Line != "")
	{

		Vstring.push_back(Line);


	}



	return Vstring;


}
stClient ConvertLineToRecord(string Line, string separator = "#//#")
{

	vector<string>Vstring = SpliteString(Line, separator);

	stClient Client;

	Client.AccountNumber = Vstring[0];
	Client.Name = Vstring[1];
	Client.PinCode = Vstring[2];
	Client.Phone = Vstring[3];
	Client.AccountBalance = stod(Vstring[4]);

	return Client;

}

string ConvertRecordToLine(stClient Client, string separator = "#//#")
{
	string ClientRecord = "";
	ClientRecord += Client.AccountNumber + separator;
	ClientRecord += Client.Name + separator;
	ClientRecord += Client.PinCode + separator;
	ClientRecord += Client.Phone + separator;
	ClientRecord += to_string(Client.AccountBalance);


	return ClientRecord;



}

vector<stClient>LoadClientDataFromFile(string ClientsFileName)
{

	vector<stClient>VClient;
	fstream MyFile;

	MyFile.open(ClientsFileName, ios::in);

	if (MyFile.is_open())
	{

		string Line;
		stClient Client;

		while (getline(MyFile, Line))
		{


			Client = ConvertLineToRecord(Line);

			VClient.push_back(Client);

		}

		MyFile.close();

	}

	return VClient;

}

void PrintClientCard(stClient Client)
{
	cout << "\nThe following are the client details : \n\n";
	cout << "--------------------------------------------------\n";
	cout << "Account Number  : " << Client.AccountNumber;
	cout << "\nName            : " << Client.Name;
	cout << "\nPin Code        : " << Client.PinCode;
	cout << "\nPhone           : " << Client.Phone;
	cout << "\nAccount Balance : " << Client.AccountBalance << endl;

	cout << "\n----------------------------------------------------\n";

}

void PrintClientRecordLine(stClient Client)
{

	cout << "|" << setw(20) << left << Client.AccountNumber;
	cout << "|" << setw(40) << left << Client.Name;
	cout << "|" << setw(15) << left << Client.PinCode;
	cout << "|" << setw(15) << left << Client.Phone;
	cout << "|" << setw(15) << left << Client.AccountBalance;

}

void ShowAllClientsScreen(vector<stClient>Vclient)
{

	cout << "\t\t\t\tClients List ( " << Vclient.size() << " ) Client(s) .\t\t\t\t";
	cout << "--------------------------------------------------------------------------------\n";


	cout << "|" << setw(10) << left << "Account Number";
	cout << "|" << setw(30) << left << "Name";
	cout << "|" << setw(15) << left << "Pin Code";
	cout << "|" << setw(15) << left << "Phone";
	cout << "|" << setw(15) << left << "Account Balance";

	cout << "\n----------------------------------------------------------------------------------------" << endl;


	for (stClient& Client : Vclient)
	{

		PrintClientRecordLine(Client);

		cout << endl;


	}

	cout << "\n------------------------------------------------------------------------------------------------" << endl;


}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient>Vclient, stClient& Client)
{

	for (stClient C : Vclient)
	{

		if (C.AccountNumber == AccountNumber)
		{

			Client = C;
			return true;


		}

	}

	return false;

}

string ReadAccountNumber()
{

	string AccountNumber = "";
	cout << "Please enter AccountNumber ? ";
	cin >> AccountNumber;

	return AccountNumber;


}
stClient ChangeClientRecord(string Accountnumber)
{

	stClient Client;
	Client.AccountNumber = Accountnumber;

	cout << "\n\nEnter Name ? ";
	getline(cin >> ws, Client.Name);
	cout << "\nEnter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "\nEnter phone?";
	getline(cin, Client.Phone);
	cout << "\nEnter AccountBalance? ";
	cin >> Client.AccountBalance;


	return Client;

}
vector<stClient>SaveClientsDataToFile(string FileName, vector<stClient>Vclient)
{

	fstream MyFile;
	string Line;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{

		for (stClient& Client : Vclient)
		{
			if (Client.MarkForDelete == false)
			{

				Line = ConvertRecordToLine(Client);
				MyFile << Line << endl;

			}

		}


		MyFile.close();
	}


	return Vclient;


}
bool UpdateClientByAccountNumber(string AccountNumber, vector<stClient>& Vclient)
{
	stClient Client;

	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, Vclient, Client))
	{

		PrintClientCard(Client);


		cout << "\n\nAre you sure you want to update this client ? y/n ? ";
		cin >> Answer;


		if (Answer == 'Y' || Answer == 'y')
		{
			for (stClient& Client : Vclient)
			{

				if (Client.AccountNumber == AccountNumber)
				{

					Client = ChangeClientRecord(AccountNumber);
					break;
				}

			}



			SaveClientsDataToFile(ClientsFileName, Vclient);


			cout << "\nClient Updated successfully." << endl;


			return true;

		}

	}
	else
	{

		cout << "\nClient with Account Number ( " << AccountNumber << " ) is Not Found !" << endl;
		return false;
	}

}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stClient>& Vclient)
{

	for (stClient& Client : Vclient)
	{

		if (Client.AccountNumber == AccountNumber)
		{

			Client.MarkForDelete = true;
			return true;



		}

	}

	return false;

}

bool DeleteClientByAccountNumber(string AccountNumber, vector<stClient>& Vclient)
{

	stClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, Vclient, Client))
	{

		PrintClientCard(Client);


		cout << "\nAre you sure you want to delete this client ? y/n ?";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y')
		{

			MarkClientForDeleteByAccountNumber(AccountNumber, Vclient);

			SaveClientsDataToFile(ClientsFileName, Vclient);



			Vclient = LoadClientDataFromFile(ClientsFileName);


			cout << "\nClient Deleted Successfully ." << endl;

			return true;


		}

	}
	else
	{


		cout << "\nClient With Accout Number ( " << AccountNumber << " ) Is Not Found ! \n";

		return false;


	}



}

bool ClientExistsByAccountNumber(string öööAccountNumber, string FileName)
{

	vector<stClient>Vclient;

	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		stClient Client;

		while (getline(MyFile, Line))

		{

			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == öööAccountNumber)
			{

				MyFile.close();
				return true;

			}
			Vclient.push_back(Client);

		}


		MyFile.close();



	}


	return false;








}
stClient ReadNewClient()
{

	stClient Client;

	cout << "\nPlease enter your Account Number ? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{

		cout << "\nClient with Account Number ( " << Client.AccountNumber << " ) already exists , Enter another Account Number ?";
		getline(cin >> ws, Client.AccountNumber);

	}



	cout << "\nPlease enter your Name ?";
	getline(cin, Client.Name);

	cout << "\nPlease enter your Pin Code ? ";
	getline(cin, Client.PinCode);

	cout << "\nPlease enter your Phone ? ";
	getline(cin, Client.Phone);

	cout << "\nPlease enter your Account Balance ? ";
	cin >> Client.AccountBalance;



	return Client;






}
void AddDataLineToFile(string FileName, string stDataLine)
{

	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();

	}

}

void AddNewClient()
{


	stClient Client;

	Client = ReadNewClient();

	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));


}
void AddNewClients()
{
	char AddMore = 'Y';

	do
	{

		system("cls");

		cout << "\nAdding New Client :\n";

		AddNewClient();

		cout << "\nClient Added Successfully , Do you want to add more clients ? y /n ?";
		cin >> AddMore;

	} while (AddMore == 'Y' || AddMore == 'y');

}

bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector<stClient>& Vclient)
{

	char Answer = 'n';

	cout << "\nAre you sure you want to perform this transaction ? y/n ?";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{

		for (stClient& Client : Vclient)
		{
			if (Client.AccountNumber == AccountNumber)
			{

				Client.AccountBalance += Amount;

				SaveClientsDataToFile(ClientsFileName, Vclient);

				cout << "\nDone Successfully . New balance is : " << Client.AccountBalance << endl;
				return true;

			}
		}



		return false;

	}
}

void PrintClientBalanceLine(stClient Client)
{



	cout << "|" << setw(15) << left << Client.AccountNumber;
	cout << "|" << setw(40) << left << Client.Name;
	cout << "|" << setw(15) << left << Client.AccountBalance;


}
void TotalBalances()
{


	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t Balances List ( " << Vclient.size() << " ) Client (s) .\n";

	cout << "--------------------------------------------------------------------------\n";

	cout << "|" << setw(15) << left << "Account Number ";
	cout << "|" << setw(40) << left << "Client Name ";
	cout << "|" << setw(15) << left << "Balance";

	cout << "\n--------------------------------------------------------------------------\n";

	double TotalBalances = 0;


	if (Vclient.size() == 0)
	{

		cout << "\n\t\t\t\tNo Clients Available In The System ! ";


	}
	else
	{

		for (stClient& Client : Vclient)
		{

			PrintClientBalanceLine(Client);

			TotalBalances += Client.AccountBalance;

			cout << endl;
		}
	}

	cout << "\n---------------------------------------------------------------------------\n";

	cout << "\n\t\t\t Total Balances =  " << TotalBalances;




}

void ShowAllClientsScreen()
{
	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);


	cout << "\t\t\t\tClients List ( " << Vclient.size() << " ) Client(s) .\t\t\t\t";
	cout << "\n-----------------------------------------------------------------------------------------------------------------\n";


	cout << "|" << setw(20) << left << "Account Number";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(15) << left << "Pin Code";
	cout << "|" << setw(15) << left << "Phone";
	cout << "|" << setw(15) << left << "Account Balance";

	cout << "\n-----------------------------------------------------------------------------------------------------------------" << endl;

	if (Vclient.size() == 0)
	{

		cout << "\t\t\t No Clients Available In the system !";

	}
	else
	{

		for (stClient& Client : Vclient)
		{

			PrintClientRecordLine(Client);

			cout << endl;


		}

	}


	cout << "\n------------------------------------------------------------------------------------------------------------------" << endl;


}
void ShowAddNewClient()
{

	cout << "--------------------------------------------\n";
	cout << "\tAdd New Client ";
	cout << "\n---------------------------------------------\n";

	AddNewClients();

}
void ShowUpdateClient()
{

	cout << "\n-----------------------------------------\n";
	cout << "\t Update Client Information ";
	cout << "\n-------------------------------------------\n";

	string AccountNumber = ReadAccountNumber();

	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);

	UpdateClientByAccountNumber(AccountNumber, Vclient);

}
void ShowDeleteClient()
{

	cout << "\n-----------------------------------------\n";
	cout << "\t Delete Client Information ";
	cout << "\n-------------------------------------------\n";

	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	DeleteClientByAccountNumber(AccountNumber, Vclient);



}
void ShowFindClient()
{

	cout << "\n-----------------------------------------\n";
	cout << "\t Find Client Information ";
	cout << "\n-------------------------------------------\n";

	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);

	string AccountNumber = ReadAccountNumber();
	stClient Client;
	if (FindClientByAccountNumber(AccountNumber, Vclient, Client))

	{

		PrintClientCard(Client);

	}
	else
	{

		cout << "\nClient with Account Number ( " << AccountNumber << " ) is Not Found !" << endl;



	}

}
void ShowExitscreen()
{
	cout << "\n--------------------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n----------------------------------------------\n";


}
void ShowDepositeScreen()
{


	cout << "---------------------------------------\n";
	cout << "\tDeposite  Screen \n";
	cout << "----------------------------------------\n";

	stClient Client;

	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, Vclient, Client))
	{

		cout << "\nClient with Account Number ( " << AccountNumber << " ) doesnt exist .\n";

		AccountNumber = ReadAccountNumber();

	}

	PrintClientCard(Client);

	double Amount = 0;

	cout << "\nPlease enter deposite amount ? ";
	cin >> Amount;

	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, Vclient);


}
void ShowWithDrawScreen()
{


	cout << "---------------------------------------\n";
	cout << "\tWithDraw  Screen \n";
	cout << "----------------------------------------\n";

	stClient Client;

	vector<stClient>Vclient = LoadClientDataFromFile(ClientsFileName);

	string AccountNumber = ReadAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, Vclient, Client))
	{

		cout << "\nClient with Account Number ( " << AccountNumber << " ) doesnt exist .\n";

		AccountNumber = ReadAccountNumber();

	}

	PrintClientCard(Client);

	double Amount = 0;

	cout << "\nPlease enter withdraw amount ? ";
	cin >> Amount;


	while (Amount > Client.AccountBalance)
	{

		cout << "\nAmount Exceed the balance ,you can withdraw up to : " << Client.AccountBalance << endl;

		cout << "\nPlease enter another amount ? ";
		cin >> Amount;

	}


	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, Vclient);


}
void ShowTotalBalance()
{

	TotalBalances();

}

void GoBackToTransactionsMenue()
{

	cout << "\n\n Press any key to go back to Transactions Menue ...";
	system("pause>0");

	ShowTransactionsMenue();

}
void GoBackToMainMenue()
{
	cout << "\nPress any Key to go back to main menue ...";
	system("pause>0");
	ShowMainMenue();

}
short ReadMainMenueOpition()
{
	short Choice = 0;
	cout << "Choose what do you want to do ? [1 to 7]?";
	cin >> Choice;
	return Choice;

}
short ReadTransactionsMenueOpition()
{
	short Choice = 0;

	cout << "\n\nChoose what do you want to do ? [1 to 4 ] ? ";
	cin >> Choice;

	return Choice;


}

void PerformTransactionsOpition(enTransactionsOpition TransactionsOpition)
{

	switch (TransactionsOpition)
	{

	case enTransactionsOpition::eDeposit:
	{
		system("cls");

		ShowDepositeScreen();

		GoBackToTransactionsMenue();

	}
	case enTransactionsOpition::eWithdraw:
	{
		system("cls");

		ShowWithDrawScreen();

		GoBackToTransactionsMenue();


	}
	case enTransactionsOpition::eShowtotalbalance:
	{


		system("cls");

		ShowTotalBalance();

		GoBackToTransactionsMenue();


	}
	case enTransactionsOpition::eShowMainMenue:

	{


		system("cls");

		ShowMainMenue();


	}

	}

}
void ShowTransactionsMenue()
{

	system("cls");

	cout << "===========================================\n";
	cout << "\t Transactions Menue \n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit .\n";
	cout << "\t[2] Withdraw .\n";
	cout << "\t[3] Total Balances .\n";
	cout << "\t[4] Main Menue .\n";
	cout << "===========================================\n";


	PerformTransactionsOpition((enTransactionsOpition)ReadTransactionsMenueOpition());





}
void PerFormMainMenueOpition(enMainMenueOptions MainMenueOpition)
{

	switch (MainMenueOpition)
	{

	case enMainMenueOptions::eListClients:
	{
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;

	}
	case enMainMenueOptions::eAddNewClients:
	{

		system("cls");
		ShowAddNewClient();
		GoBackToMainMenue();
		break;


	}
	case enMainMenueOptions::eDeleteClient:
	{
		system("cls");
		ShowDeleteClient();
		GoBackToMainMenue();
		break;


	}
	case enMainMenueOptions::eUpdateClient:
	{

		system("cls");
		ShowUpdateClient();
		GoBackToMainMenue();
		break;

	}
	case enMainMenueOptions::eFindClient:
	{

		system("cls");
		ShowFindClient();
		GoBackToMainMenue();
		break;


	}case enMainMenueOptions::eTransactions:
	{

		system("cls");

		ShowTransactionsMenue();
		break;

	}

	case enMainMenueOptions::eExit:
	{

		system("cls");
		ShowExitscreen();
		GoBackToMainMenue();
		break;


	}

	}


}

void ShowMainMenue()
{


	system("cls");

	cout << "============================================\n";
	cout << "\t\tMain Menue Screen \n";
	cout << "=============================================\n";
	cout << "\t[1] Show Client List .\n";
	cout << "\t[2] Add New Client .\n";
	cout << "\t[3] Delete Client .\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions .\n";
	cout << "\t[7] Exit .\n";
	cout << "====================================================\n";

	PerFormMainMenueOpition((enMainMenueOptions)ReadMainMenueOpition());

}

int main()

{
	ShowMainMenue();
	return 0;
}
