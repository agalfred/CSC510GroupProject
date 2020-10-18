// TestProjectConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cctype>
#include<iomanip>
#include <string>
#include<sstream>
#include <stdio.h>

using namespace std;

char ch;
string st;
int num;

class account
{
public:
	bool active;
	static int totalAccounts;
	int accountNum;
	char name[50];
	int balance;
	char accountType;
	void create_acct();
	void print_account();
	void mod();
	void report_table() const;
};

int account::totalAccounts = 0;

void account::create_acct()
{
	accountNum = totalAccounts++;
	printf("\n\nEnter the account holder's name: ");
	cin.ignore();
	cin.getline(name, 50);
	printf("\nEnter account type ( [\'S\']avings or [\'C\']hecking, default is savings): ");
	cin >> st;
	if (st == "C" || st == "c") {
		accountType = 'C';
	}
	else {
		accountType = 'S';
	}
	do {
		printf("\nEnter initial deposit amount: ");
		cin >> st;
		int stlen = st.length();
		for (int i = 0; i < stlen; i++) {
			if (!isdigit(st[i])) {// If the initial deposit is not a simple positive integer, user is prompted to try again.
				continue;
			}
		}
		break;
	} while (1);
	balance = stoi(st);
}

void account::print_account()
{
	printf("\nAccount number: %d", accountNum);
	printf("\nPrimary account holder: %s", name);
	printf("\nAccount type: %c", accountType);
	printf("\nAccount balance: %d", balance);
}

void account::mod()
{
	printf("\nAccount number: %d", accountNum);
	printf("\nChange account holder's name: ");
	cin.ignore();
	cin.getline(name, 50);
	printf("\nChange account type ( [\'S\']avings or [\'C\']hecking, default is no change): ");
	cin >> st;
	if (st == "C" || st =="c") {
		accountType = 'C';
	}
	else if (st == "S" || st == "s") {
		accountType = 'S';
	}
	do {
		printf("\Change balance, default is no change: ");
		cin >> st;
		int stlen = st.length();
		if (!stlen) {
			st = balance;
			break;
		}
		for (int i = 0; i < stlen; i++) {
			if (!isdigit(st[i])) {// If the input is not a simple positive integer, user is prompted to try again.
				continue;
			}
		}
		break;
	} while (1);
	balance = stoi(st);
}


void account::report_table() const
{
	printf("        %d         %s        %c          %d \n", accountNum, name, accountType, balance);
}

account* acs;// main account array

void save_account();
void disp_sp(int);
void mod_account(int);
void delete_account(int);
void show_accounts();
void deposit_withdraw(int, int);
void home();
void update_file();
void fetch_accounts();

int main()
{
	home();
	fetch_accounts();
	do
	{
		system("cls");
		printf("\n\n\nMain Menu\n\n\t1. New Account\n\n\t2. Deposit\n\n\t3. Withdraw\n\n\t4. Balance\n\n\t5. List Accounts \n\n\t6. Close Account\n\n\t7. Change Account Details\n\n\t8 Exit\n\n\tSelect menu option (1-8) ");
		cin >> st;
		system("cls");
		if (!(st.length() - 1)) {// if only one character is entered, as anticipated
			ch = st[0];
		}
		else {// more characters automatically trigger the default option, rather than using the last character of whatever input
			ch = '9';
		}
		switch (ch)
		{
		case '1':
			save_account();
			break;
		case '2':
			printf("\n\n\tEnter the account number: ");
			cin >> num;
			deposit_withdraw(num, 0);
			break;
		case '3':
			printf("\n\n\tEnter the account number:");
			cin >> num;
			deposit_withdraw(num, 1);
			break;
		case '4':
			printf("\n\n\tEnter the account number:");
			cin >> num;
			disp_sp(num);
			break;
		case '5':
			show_accounts();
			break;
		case '6':
			printf("\n\n\tEnter account number:");
			cin >> num;
			delete_account(num);
			break;
		case '7':
			printf("\n\n\tEnter account number:");
			cin >> num;
			mod_account(num);
			break;
		case '8':
			printf("\n\n\tExiting banking system");
			break;
		default:printf("\a");
		}
		cin.ignore();
		cin.get();
	} while (ch != '8');
	return 0;
}
void save_account()
{
	account ac;
	ofstream outFile;
	outFile.open("account.dat", ios::binary | ios::app);
	ac.create_acct();
	outFile.write(reinterpret_cast<char*> (&ac), sizeof(account));
	outFile.close();
	printf("\nAccount has been created");
	fetch_accounts();
}
void fetch_accounts() {
	ifstream inFile;
	inFile.open("account.dat", ios::binary);
	if (!inFile)
	{
		printf("File not found....press any key");
		return;
	}
	if (account::totalAccounts == 0) {
		account tmp;
		while (inFile.read(reinterpret_cast<char*>(&tmp), sizeof(account))) {
			account::totalAccounts += 1;
		}
		inFile.clear();
		inFile.seekg(0, ios::beg);
	}
	int i = 0;
	delete acs;
	acs = new account[account::totalAccounts];
	while (inFile.read(reinterpret_cast<char*>(&acs[i++]), sizeof(account))) {}
	inFile.close();
}
void disp_sp(int n)
{
	if (n < account::totalAccounts && acs[n].active) {
		printf("\nBalance Details\n");
		acs[n].print_account();
	}
	else {
		printf("\nThis account number is not valid\n");
	}
}
void mod_account(int n)
{
	if (n < account::totalAccounts && acs[n].active) {
		acs[n].print_account();
		printf("\n\nEnter the new account details");
		acs[n].mod();
		update_file();
		printf("\n\n\tAccount Updated");
	}
}
void delete_account(int n)
{
	acs[n].active = 0;
	update_file;
	printf("\n\n\tAccount deleted");

}
void show_accounts()
{
	printf("\n\n\t\tList of Account Holders\n\n");
	printf("==============================================================\n");
	printf("Account Number      NAME       Account type  Balance\n");
	printf("==============================================================\n");
	for (int i = 0; i < account::totalAccounts; i++)
	{
		if(acs[i].active)
			acs[i].report_table();
	}
}
void deposit_withdraw(int n, int option)
{
	if (n < account::totalAccounts && acs[n].active) {
		acs[n].print_account();
		string options[2] = { "deposit", "withdraw" };
		int amount;
		do {
			cout << "\n\n\tEnter the amount to " << options[option] << ": ";
			cin >> st;
			int stlen = st.length();
			for (int i = 0; i < stlen; i++) {
				if (!isdigit(st[i])) {// If the input is not a simple positive integer, user is prompted to try again.
					continue;
				}
			}
			break;
		} while (1);
		amount = stoi(st);
		if (option == 0)
			acs[n].balance += amount;
		else
		{
			int bal = acs[n].balance - amount;
			if ((bal < 500 && acs[n].accountType == 'S') || (bal < 1000 && acs[n].accountType == 'C'))
				printf("Insufficient balance");
			else
				acs[n].balance -= amount;
		}
		update_file();
		printf("\n\n\t Account Updated");
	}
}
void home()
{
	printf("Account \n\n\tManagement \n\n\tSystem \n\n\tVersion 1.0.0 \n\n\tTyrell Corporation Olympia Washington\n\n\tPress any key to enter....");
	cin.get();
}
void update_file() {
	ofstream outFile;
	remove("account.dat");
	outFile.open("account.dat", ios::binary);
	for (int i = 0; i < account::totalAccounts; i++)
	{
		outFile.write(reinterpret_cast<char*>(&acs[i++]), sizeof(account));
	}
	outFile.close();
}




class Mortgage
{
private:
	int Amount, Term, monthlyTerm;
	double Interest, Payment, InterestPaid, setInterest;
public:
	void GetAmount();
	void termRate();
	void getrateTerm();
	void amoritzation();
	void Payments();
};

void Mortgage::GetAmount()
{
	system("cls");
	printf("Tyrell Financial Services Mortgage Calculator\n");
	printf("===============================================\n\n");
	printf("Enter Loan Amount: ");
	cin >> Amount;
}


void Mortgage::getrateTerm()
{
	printf("\nEnter Term Length (Years): ");
	cin >> Term;
	printf("\nEnter Interest Rate: ");
	cin >> Interest;
}
void Mortgage::termRate()
{
	int check = 0;
	int choice = 0;
	int arr;
	int termYr[3] = { 7,15,30 };
	double interestYr[3] = { 5.25,5.5,5.75 };

	do
	{
		printf("\n--------Loan Menu--------");
		printf("\n1. 7 years at 5.25 \n2. 15 years at 5.5 \n3. 30 years at 5.75");
		printf("\n-------------------------");
		cin >> choice;
		check = 0;
		if (choice <= 0)
		{
			system("cls");
			printf("Tyrell Financial Services Mortgage Calculator\n");
			printf("===============================================\n\n");
			printf("Enter Loan Amount: ");
			printf("\n\nEnter only 1,2,3 to choose term and rate\n");
			check = 1;
		}
		else if (choice == 1)
			arr = 0;
		else if (choice == 2)
			arr = 1;
		else if (choice == 3)
			arr = 2;
		else if (choice > 3)
		{
			system("cls");
			printf("Tyrell Financial Services Mortgage Calculator\n");
			printf("===============================================\n\n");
			printf("Enter Loan Amount: ");
			printf("\n\nEnter only 1,2,3 to choose term and rate\n");
			check = 1;
		}

	} while (check == 1);
	Term = termYr[arr];
}
void Mortgage::Payments()
{
	system("cls");
	Payment = Amount * (((Interest / 100.0) / 12) / (1 - pow(1 + ((Interest / 100.0) / 12), -Term * 12)));
	monthlyTerm = (Term * 12);
	InterestPaid = ((Interest / 100.0) / 12);
}
void Mortgage::amoritzation()
{
	int Month = 1;
	int listPause = 5;
	double NewBal;
	NewBal = Amount;

	printf("\n----------------------------------------------");
	printf("\nWith a loan of $%d for %d years\n at a rate of %lf \nyour monthly payment will be $%lf", Amount, Term, Interest, Payment);
	printf("\n----------------------------------------------");
	while (monthlyTerm > 0)
	{
		double Interestp = InterestPaid * NewBal;
		double endBal = (NewBal - NewBal);

		if (NewBal > Payment)
		{
			cout.setf(ios::fixed);
			printf("\n%d\t $ %lf \t\t $ %lf", Month, Interestp, NewBal - Payment + Interestp);


			if (listPause < 19)
			{
				++listPause;
			}
			else
			{
				printf("\n");
				system("PAUSE");
				listPause = 0;
				system("cls");
				printf("Payment \tInterest Paid \tNew Balance\n");
			}
		}
		else if (NewBal <= Payment)
		{
			cout.setf(ios::fixed);
			printf("\n %d \t $ %lf \t\t $%lf", Month, Interestp, endBal);
			listPause = 0;
		}
		NewBal = NewBal - Payment + Interestp;
		Month = ++Month;
		monthlyTerm = --monthlyTerm;
	}

}

