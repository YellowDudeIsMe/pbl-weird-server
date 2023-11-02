#include <iostream> // for i/o
#include <fstream> // for file interactions
#include <algorithm> // for find functions
#include <cstdio> // for renaming files
#include <cstdlib> // for random numbers
#include <sstream> // for reading files through streams
#include <cstring> // for strings
#include <windows.h> // for the delay func
#include <vector> // no purpose yet
#include <map> // for storing wallet data

#define ACCOUNT_CLIENT "database/account_client.txt"
#define WALLET_CLIENT "database/wallet_client.txt"

using namespace std;

const int WALLET_ITEM_COUNT = 5;
const string WALLET_ITEM[WALLET_ITEM_COUNT] = {"Cash", "Key", "Credit Card", "Debit Card", "Driver License"};

class Client{
	protected:
		int id;
		string username;
		string password;
		map<string, int> wallet = {
		{"Cash", 0},
		{"Key", 0},
		{"Credit Card", 0},
		{"Debit Card", 0},
		{"Driver License", 0}
		};
	public:
		void setupClient(int id){
			this->id = id;
			
			string acc;
			string wal;
			
			ifstream client (ACCOUNT_CLIENT);
			if (client.is_open()){
				for (int c = 0; c <= id; c++){
					getline(client, acc);
				}
			}
			istringstream ss(acc);
			ss >> this->username >> this->password; 
			
			client.close();
			ss.clear();

			
			client.open(WALLET_CLIENT);
			if (client.is_open()){
				for (int c = 0; c <= id; c++){
					getline(client, wal);
				}
			}
			
			ss.str(wal);
			for (int i = 0; i < WALLET_ITEM_COUNT; i++){
				ss >> this->wallet[WALLET_ITEM[i]];
			}
			
			client.close();
		}
		
		string getUsername(){
			return this->username;
		}
		string getPassword(){
			return this->password;
		}
		int getWalletItem(string item){
			return this->wallet[item];
		}
		void setUsername(string x){
			this->username = x;
		}
		void setPassword(string x){
			this->password = x;
		}
		void setWalletItem(string item, int value){
			this->wallet[item] = value;
		}

};

class LocalClient : public Client{
	public:	
		

};

class UI{
	protected: 
		//Variables
		int clientID;
		Client client;
		
		//Hidden functions
		int findClient(string, string);
		void modifyDatabase(string, int, int, string);
		void displayWallet();
		void changeUsername(string);
		void changePassword(string);
		void updateWallet(string, int);
		void work();
	public:
		
		UI(){
			welcomeMenu();
		}
		
		//Menus
		void mainMenu();
		void changeUsernameMenu();
		void changePasswordMenu();
		void loginMenu();
		void registerMenu();
		void welcomeMenu();
		void exitMenu();
		
		//Popups
		void loading(string msg){
			system("cls");
			cout << msg;
			for (int i = 0; i < 5; i++){
				Sleep(500);
				cout << ".";
				Sleep(500);
			}			
		}
		
		void popup(int time, string msg){
			system("cls");
			cout << msg;
		}
};

//==================================
//          UI FUNCTIONS
//==================================

void UI::work(){
	system("cls");
	srand(time(nullptr));
	int salaryMin = 0;
	int salaryMax = 6;
	int gap = salaryMax - salaryMin;
	
	int salary = rand() % (gap + 1) + salaryMin;
	
	if (salary <= salaryMin + gap / 3){
		int argC = 3;
		string args[] = {
			"Oops, that's a mistake that we won't tolerate...",
			"You look down today, are you okay?...",
			"Woah, I think you kinda messed up there..."
		};
		cout << args[rand() % argC];
		
	} else if (salary <= salaryMin + gap / 3 * 2){
		int argC = 3;
		string args[] = {
			"I think you did good. Definitely not your best though...",
			"Decent work right there...",
			"Not bad for today..."
		};
		cout << args[rand() % argC];
	} else {
		int argC = 5;
		string args[] = {
			"Excellent work today! Can you be like this everyday?...",
			"That mistake would have costed us, but so glad you fixed it on time!...",
			"If you keep working like this, you could be our employee of the month!...",
			"You know, I think that's some hard work from you, so maybe a raise today for you...",
			"Oh woah, I feel like in a good mood today! Hang on for a bit..."
		};
		cout << args[rand() % argC];
	}
	
	Sleep(2500);
	cout << endl << "-------------------------------------------" << endl;
	cout << "After some consideration, we will give you $" << salary << " today!" << endl;
	cout << "Okay, wrap it up, go home everyone...";
	
	client.setWalletItem("Cash", client.getWalletItem("Cash") + salary);
	updateWallet("Cash", client.getWalletItem("Cash"));
	
	Sleep(2500); 
}
void UI::updateWallet(string key, int value){
	modifyDatabase(WALLET_CLIENT, clientID, find(WALLET_ITEM, WALLET_ITEM + WALLET_ITEM_COUNT, key) - WALLET_ITEM, to_string(value));
}

void UI::changeUsername(string username){
	modifyDatabase(ACCOUNT_CLIENT, clientID, 0, username);
}

void UI::changePassword(string password){
	modifyDatabase(ACCOUNT_CLIENT, clientID, 1, password);	
}

void UI::displayWallet(){
	system("cls");
	
	cout << "	         MY WALLET" << endl;
	cout << "========================================" << endl;
	string s;
	for (int i = 0; i < WALLET_ITEM_COUNT; i++){
		string key = WALLET_ITEM[i];
		int value = client.getWalletItem(key);
		
//				cout << key << " : " << value << endl;
		if (key == "Cash"){
			s = s + key + ": $" + to_string(value);
		} else if (value != 0) {
			s = s + "\n" + key + ": " + to_string(value);
		}
	}
	cout << s;
	Sleep(5000);	
}

void UI::modifyDatabase(string file_dir, int line, int word, string update){
	ifstream from (file_dir);
	ofstream to (file_dir + "_$tmp");

	if (from.is_open() && to.is_open()){
		string tmp_line;
		string tmp_word;
		
		for (int i = 0; i < line; i++){
			getline(from, tmp_line);
			to << tmp_line << '\n';
		}
	
		for (int i = 0; i < word; i++){
			getline(from, tmp_word, ' ');
			to << tmp_word << ' ';
		}
		
		from.ignore(1000, ' ');
		to << update << ' ';
		
		to << from.rdbuf();
		
		from.close();
		to.close();
		remove(file_dir.c_str());
		rename((file_dir + "_$tmp").c_str(), file_dir.c_str());
	}
}

int UI::findClient(string username, string password){
	int ret;
	ifstream clientbase (ACCOUNT_CLIENT);
	
	if (clientbase.is_open()){
		string line;
		while (getline(clientbase, line)){
			string u, p;
			istringstream ss(line);
			ss >> u >> p;
			if (u == username && p == password){
				return ret;
			} else ret++;
		}
		clientbase.close();
	}
	
	return -1;
}

//==================================
//              MENUS
//==================================

void UI::mainMenu(){
	system("cls");
	this->client.setupClient(this->clientID);
	
	cout << "	        THE WEIRD SERVER" << endl;
	cout << "========================================" << endl;
	cout << "1: Check your wallet" << endl;
	cout << "2: Work" << endl;
	cout << "3: Change your username" << endl;
	cout << "4: Change your password" << endl;
	cout << "5: Log out" << endl;
	int n;
	cin >> n;
	switch (n){
		case 1:
			displayWallet();
			mainMenu();
			break;
		case 2:
			work();
			mainMenu();
			break;
		case 3:
			changeUsernameMenu();
			mainMenu();
			break;
		case 4:
			changePasswordMenu();
			mainMenu();
			break;
		case 5:
			welcomeMenu();
	}	
}

void UI::changeUsernameMenu(){
	system("cls");
	string username;
	
	cout << "	         CHANGE USERNAME" << endl;
	cout << "========================================" << endl;
	cout << "New username: ";
	
	cin >> username;
	changeUsername(username);

	cout << endl << "SUCCESS!" << endl;
	Sleep(1000);
	loading("Updating username");
};

void UI::changePasswordMenu(){
	system("cls");
	string password;
	
	cout << "	         CHANGE PASSWORD" << endl;
	cout << "========================================" << endl;
	cout << "New password: ";
	
	cin >> password;
	changePassword(password);

	cout << endl << "SUCCESS!" << endl;
	Sleep(1000);
	loading("Updating password");
};

void UI::loginMenu(){
	system("cls");
	string username, password;
	
	cout << "	           LOGIN MENU" << endl;
	cout << "========================================" << endl;
	cout << "Please enter accordingly using no space:" << endl;
	cout << " " << endl;
	cout << "|__Username: ";
	cin >> username;
	cout << "   |__Password: ";
	cin >> password;
	
	int id = findClient(username, password);
	
	if (id > -1){
		//MAIN MENU
		this->clientID = id;
		loading("Joining");
		mainMenu();
	} else {
		cout << endl;
		cout << "Looks like we couldn't find such account, please press:" << endl;
		cout << "1: Retry logging in" << endl;
		cout << "2: Register if you haven't had an account before" << endl;
		int n;
		cin >> n;
		switch (n){
			case 1:
				loginMenu();
				break;
			case 2:
				registerMenu();
		}
	}
}

void UI::registerMenu(){
	system("cls");
	string username, password;
	
	cout << "	          REGISTER MENU" << endl;
	cout << "========================================" << endl;
	cout << "Please enter accordingly using no space:" << endl;
	cout << " " << endl;
	cout << "|__Username: ";
	cin >> username;
	cout << "   |__Password: ";
	cin >> password;
	
	if (findClient(username, password) == -1){
		ofstream clientbase (ACCOUNT_CLIENT, ios::app);
		if (clientbase.is_open()){
			clientbase << username << ' ' << password << '\n';
			clientbase.close();
		}
		
		ofstream clientwallet (WALLET_CLIENT, ios::app);
		if (clientwallet.is_open()){
			for (int i = 0; i < WALLET_ITEM_COUNT; i++){
				clientwallet << "0 ";
			}
			clientwallet << '\n';
			clientwallet.close();
		}
		
		loading("Updating our database");
		welcomeMenu();
	} else {
		cout << "Account already existed, please press:" << endl;
		cout << "1: Retry registering" << endl;
		cout << "2: Log in if you already have an account" << endl;
		int n;
		cin >> n;
		switch (n){
			case 1:
				registerMenu();	
				break;
			case 2:
				loginMenu();
		}
	}	
}

void UI::welcomeMenu(){
	system("cls");
	
	cout << "      WELCOME TO THE WEIRD SERVER!" << endl;
	cout << "========================================" << endl;
	cout << "1: Login if you already have an account" << endl;
	cout << "2: Register if you don't have one yet" << endl;
	cout << "3: Exit" << endl;
	
	int n; cin >> n;
	system("cls");
	switch (n){
		case 1: //Login
			loginMenu();
			break;			
		case 2: //Register
			registerMenu();
			break;
		case 3:
			exitMenu();
	}
}

void UI::exitMenu(){
	cout << "      EXITING THE WEIRD SERVER!..." << endl;
	cout << "========================================" << endl;
	cout << "Thanks for tuning by! See you next time. :)" << endl;
}

//=========================================
//              MAIN FUNCTION
//=========================================

int main(){
	UI localUI;
	
	return 0;
}
