#include "sqlite/sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <stdio.h>
int ans = 0;
int isAdmin = 0;
bool isLoginned = false;
int isChecked = false;
bool database_created = false;
char** argv;
const char* dir = "c:\\Database.db";
bool isBeta = true;
bool advanced_mode = false;
std::string version = "1.0.7";
bool logs(std::string message, int type);
std::string username;
std::string password;
static int createTable(const char* s);
void SettingsTab();
std::vector<std::string> lines;
void fetchcashAmount();
void Checkforsettings();
void userPanel();
std::string line;
void userInput(int cls, int type, std::string o1, std::string o2, std::string o3, std::string o4);
int main(const char* s);
static int createDB(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int insertData(const char* s, std::string sql);
void SaveData_local(std::string message) {
	std::ofstream SaveData_local;
	SaveData_local.open("C://logs.txt", std::ios::app);
	if (SaveData_local.is_open()) {
		//logs("", 2);
		SaveData_local.close();
	}

}
/*void streamFile(std::string filename, std::string fileData) {
	std::ofstream ofFile;
	//std::fstream fFile; TODO
		ofFile.open(filename);
		if (ofFile.is_open()) {
			ofFile << fileData;
		}
		else {
			perror("Whoops ");
		}

}*/
struct {
public:
	int mainAns;
	int userpanelAns;
	int ATMAns;
	int cash;
	int marketplaceAns;
	std::string item;
	int view_marketplace;
	int add_item_marketplace;
	int view_marketplace_edit;

}user;

BOOL CheckforPrivilege() {
	BOOL fRet = FALSE;
	HANDLE hToken = NULL;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		TOKEN_ELEVATION Elevation;
		DWORD cbSize = sizeof(TOKEN_ELEVATION);
		if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
			fRet = Elevation.TokenIsElevated;
		}
	}
	if (hToken) {
		CloseHandle(hToken);
	}
	if (fRet == 0) {
		logs("Admin privileges are necessary", 2);
	}
	return fRet;

}

void color(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);

}

bool logs(std::string message, int type) { //prints out logs
	switch (type) {
	case 1:
		color(2);
		std::cout << "[*] > " << message << "\n";
		color(8);
		break;
	case 2:
		color(4);
		std::cout << "[!] > " << message << "\n";
		color(8);
		break;
	case 3:
		color(6);
		std::cout << "[x] > " << message << "\n";
		color(8);
		break;
	}
	std::ofstream logs;
	logs.open("dbincpp_logs.txt", std::ios::app);
	if (logs.is_open()) {
		logs << "dbincpp << [" << message << "]\n\n";
	}

	return 0;
}
bool Register() {
	system("cls");
	color(14);
	std::cout << "[*] Registration Panel";
	color(8);
	std::cout << "\n		Username:"; std::cin >> username; std::cout << "\n\n		Password:"; std::cin >> password;
	if (username.length() < 3) {
		logs("username length is too short", 2);
		if (password.length() < 3) {
			logs("password length is too short", 2);
			return 1;
		}

	}
	else {
		//selectData(dir, "SELECT * FROM User WHERE username ='" + username + "';");

		//logs("Saving to Database", 1);
		std::string save = "INSERT INTO User (username, password) VALUES ('" + username + "','" + password + "');";
		sqlite3* DB;
		char* messageError;



		int exit = sqlite3_open(dir, &DB);
		/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
		exit = sqlite3_exec(DB, save.c_str(), callback, 0, &messageError);
		if (exit != SQLITE_OK) {
			std::cout << "oops:" << messageError;
			logs("unsuccessfull register attempt.", 2);

		}
		else
		logs("registration succesfull!", 1);
		Sleep(2000);
		main(dir);
		return 0;
	}



}
static int insertData(const char* s, std::string sql)
{
	sqlite3* DB;
	char* messageError;



	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
	exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in insertData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		std::cout << "Records inserted Successfully!" << std::endl;

	return 0;
}
static int selectData(const char* s, std::string sql)
{
	sqlite3* DB;
	char* messageError;



	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		std::cout << "Records selected Successfully!" << std::endl;


	return 0;
}
void highlighter(std::string message, std::string highlightedMessage) {
	std::cout << message; color(14); std::cout << highlightedMessage << "\n\n"; color(8);
}
void Addmoney(std::string username, std::string amount) {
	std::string::size_type sz;
	std::cout << username;
	system("pause");
	std::string sql = "UPDATE User SET cash =" + amount + " WHERE username = '" + username + "';";
	int i_amount = std::stoi(amount, &sz);
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(dir, &DB);
	exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Database error" << std::endl;

		sqlite3_free(messageError);
		system("pause");
	}
	else {
		logs("Money was added successfully!", 1);
		Sleep(1000);
		userPanel();
	}
	sqlite3_close(DB);
}
void ATM() {
	//	fetchcashAmount();
		//highlighter("Welcome back, ", username);
		//std::cout << "\nYou Currently have: $" << user.cash << "\n";
	userInput(1, 3, "Withdraw", "Deposit", "Manipulate money", "Exit");
	std::string amount;
	switch (user.ATMAns) {
	case 1:

		break;
	case 2:

		break;
	case 3:
		//std::string amount;
		color(8);
		std::cout << "\n\nEnter the amount you want to change your cash to:"; color(14); std::cin >> amount;	color(8);

		Addmoney(username, amount);
		break;
	case 4:

		break;

	}


}
void fetchcashAmount() {
	std::string query = "SELECT cash FROM User WHERE username = '" + username + "';";
	selectData(dir, query);
	std::fstream data;
	data.open("c://data.txt");
	if (data.is_open()) {
		std::string line;
		while (getline(data, line)) {
			std::vector<std::string> lines;
			lines.push_back(line);
			for (const auto& i : lines) {
				int i_line = std::stoi(line, nullptr, 2);
				user.cash = i_line;
			}
		}

	}
	system("pause");
}
void additem_marketplace() {
	std::string item_name;
	sqlite3* DB;
	std::string item_price;
	char* messageError;
	int exit = sqlite3_open(dir, &DB);
	userInput(1, 6, "select item", "a", "b", "c");
	switch (user.add_item_marketplace) {
	case 1:
		std::cout << "Select a name:"; std::cin >> item_name;
		if (item_name.length() >= 1) {
			std::cout << "\nSelect a price for:" << item_name << "\n> "; std::cin >> item_price;
			if (item_price.length() > 0) {
				logs("adding item.", 1);
				std::string query = "INSERT INTO Marketplace (itemName, price) VALUES ('" + item_name + "'," + item_price + ");";
				exit = sqlite3_exec(DB, query.c_str(), callback, 0, &messageError);
				if (exit != SQLITE_OK) {
					std::cout << "oops:" << messageError;
					Sleep(2000);
				}
				//std::cout << query;
				system("pause");
			}
			
		}
		break;
	}

}
void view_marketplace() {
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(dir, &DB);
	std::string query = "SELECT * FROM Marketplace";
	int select_item;
	std::string item_name;
	std::string item_price;
	int items = 10; //item amount (marketplace)
	logs(items + " found.", 1);
	std::cout << "=============ITEM LIST=============\n";
	exit = sqlite3_exec(DB, query.c_str(), callback, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cout << "oops:" << messageError;
	}
	std::cout << "=============ITEM LIST=============";
	userInput(0, 7,"Buy Item", "Delete Item", "c", "d");
	switch (user.view_marketplace_edit) {
	case 1:
		
		std::cout << "name of the item:"; std::cin >> item_name;
		query = "SELECT price FROM Marketplace WHERE itemName = '" + item_name + "'";
		exit = sqlite3_exec(DB, query.c_str(), callback, 0, &messageError);
		std::fstream price_data;
		price_data.open("c://data.txt");
		if (price_data.is_open()) {
			std::string line;
			while (getline(price_data, line)) {
				item_price = line;
				std::cout << "this item costs $" << item_price << " do you want to buy this?";
				system("pause");
			}
		}
		break;

	}
}
void Marketplace() {

	logs("Here you can buy/sell items. This feature is still in beta!", 1);
	userInput(0, 4, "View Marketplace", "Add Item", "Delete Item", "Exit");
	switch (user.marketplaceAns) {
	case 1:
		view_marketplace();
		break;
	case 2:
			additem_marketplace();
		break;
	case 3:
		break;
	case 4:
		userPanel();
		break;



	}

}

void userPanel() {
	system("cls");
	std::string welcome_message = "Welcome back, " + username;
	if (username == "admin" || "d4niel") { //checks for admin
		isAdmin == 1;
	}
	//fetchcashAmount();
	//int userans;
	if (isAdmin == 1) {
		welcome_message += " isAdmin = true";
	}
	std::cout << welcome_message;


	userInput(0, 2, "ATM", "Marketplace", "Inventory", "Log out");

	system("title dbincpp Userpanel");
	switch (user.userpanelAns) {
	case 1:
		//system("cls");
		ATM();

		break;
	case 2:
		Marketplace();
		break;
	case 3:
		break;
	case 4:
		main(dir);
		break;

	}

}
bool Login() {

	sqlite3* DB;
	char* messageError;
	//sqlite3_stmt* stmt;
	std::string m_username;
	std::string m_password;
	int exit = sqlite3_open(dir, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	system("cls");
	color(14);
	std::cout << "[*] Login Panel";
	color(8);
	std::cout << "\n		Username:"; std::cin >> username; std::cout << "\n\n		Password:"; std::cin >> password;
	std::string sql = "SELECT username, password FROM User WHERE username = '" + username + "' AND password = '" + password + "';";

	exit = sqlite3_exec(DB, sql.c_str(), callback, 0, &messageError);
	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;

		sqlite3_free(messageError);
	}
	else
		if (username.length() > 3 && password.length() > 3) {
			std::fstream credentials;
			credentials.open("c://data.txt"); //this will be changed to some better way (not urgent as of now)
			//std::string line;
			if (!credentials.is_open()) {
				perror("Something bad happened:");
			}
			else {
				while (getline(credentials, line)) {
					std::vector<std::string> lines;
					lines.push_back(line);
					for (const auto& i : lines) {
						if (line.find(username)) {
							Sleep(200);
							if (line.find(password)) {
								credentials.close();
								//	system("pause");
								remove("c://data.txt");
								userPanel();
								}
							

						}
						else {
							//std::cout << "Invalid Credentials...";
							logs("Invalid Credentials", 2);
							Sleep(2000);
							main(dir);
						}

					}
				}


			}

		}
		else {
			logs("username/password length is invalid", 2);
			Sleep(2000);
			Login();
		}

	//system("pause");
	sqlite3_close(DB);
	return 0;
}

void userInput(int cls, int type, std::string o1, std::string o2, std::string o3, std::string o4) {

	std::string title = "\n>	dbincpp v[" + version + "]";
	int ans;
	switch (cls)
	{
	case 0:
		break;
	case 1:
		system("cls");
		break;

	}
	if (isBeta == false) {
		title += " Beta";
	}
	color(5);
	std::cout << title;
	color(8);
	std::cout << "\n	[1]"; color(7); std::cout << o1; color(8);
	std::cout << "\n	[2]"; color(7); std::cout << o2; color(8);
	std::cout << "\n	[3]"; color(7); std::cout << o3; color(8);
	std::cout << "\n	[4]"; color(7); std::cout << o4; color(8);
	color(14); std::cout << "\n		> "; std::cin >> ans;
	switch (ans) {
	case 1:
		if (o1 == "")
			logs("Invalid Input", 2);
		else

			break;
	case 2:
		if (o2 == "")
			logs("Invalid Input", 2);
		else

			break;
	case 3:
		if (o3 == "")
			logs("Invalid Input", 2);
		else

			break;
	case 4:
		if (o4 == "")
			logs("Invalid Input", 2);
		else

			break;
	}
	switch (type) {
	case 1:
		user.mainAns = ans;
		break;
	case 2:
		user.userpanelAns = ans;
		break;
	case 3:
		user.ATMAns = ans;
		break;
	case 4:
		user.marketplaceAns = ans;
		break;
	case 5:
		user.view_marketplace = ans;
	case 6:
		user.add_item_marketplace = ans;
	case 7:
		user.view_marketplace_edit = ans;
	}

}


int main(const char* s) {
	//view_marketplace();
	system("title dbincpp (top codenz $$$$$)");

	if (isChecked == false) {
		Checkforsettings();
	}

	CheckforPrivilege();
	if (isLoginned == false) {
		logs("You're not logged in, please make sure to do so or register by pressing 2", 2);
	}
	sqlite3* db;
	createDB(dir);
	createTable(dir);

start:
	userInput(1, 1, "Register.", "Login.", "Settings.", "Exit.");
	switch (user.mainAns) {
	case 1:
		Register();
		break;
	case 2:
		Login();
		break;
	case 3:
		SettingsTab();
		break;
	case 4:
		system("exit");
		break;

	}



	/*std::cout << "\n\n[0] View Database";
	std::cout << "\n[1] Register.\n";
	std::cout << "[2] Login.\n";
	std::cout << "[3] Settings.\n";
	std::cout << "[4] Exit.\n\n-> ";
	color(14);
	std::cin >> ans;
	if (ans == 3) {
		SettingsTab();
	}
	if (ans == 2) {
		Login();
	}
	if (ans == 1) {

		Register();
	}
	if (ans == 0) {

		char* error;
		logs("Attempting to view Database", 1);
		selectData(dir, "SELECT * FROM User;");
		Sleep(2000);
		goto start;
	}
	if (ans == 3) {
		system("exit");

	}
	if (ans == 1) {
		system("cls");
		Register();
	}
	else {
		logs("Invalid Input", 2);
		system("cls");
		goto start;
	}*/

	return 0;
}

static int createDB(const char* s) {
	sqlite3* db;
	int exit = 0;
	exit = sqlite3_open(s, &db);
	if (exit != SQLITE_OK) {
		std::cout << "Something went wrong..";
	}
	sqlite3_close(db);

	return 0;

}

static int createTable(const char* s) {

	int exception;
	sqlite3* db;
	std::string query = "CREATE TABLE IF NOT EXISTS User ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"username TEXT NOT NULL,"
		"password TEXT NOT NULL,"
		"cash INTEGER NOT NULL DEFAULT 0);";

	char* Error;
	int exit = 0;
	exit = sqlite3_open(s, &db);
	exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
	if (exit != SQLITE_OK) {
		std::cout << "\nError While Creating Table:" << Error;
		logs("error on user table", 2);
		sqlite3_free(Error);
	}
	else {
		logs("User table Created Successfully", 1);	
			query = "CREATE TABLE IF NOT EXISTS Marketplace ("
				"userID INTEGER PRIMARY KEY,"
				"itemName TEXT,"
				"price INTEGER);";

			exit = sqlite3_open(s, &db);
			exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
			if (exit != SQLITE_OK) {
				std::cout << "\nError While Creating Table:" << Error;
				logs("error on Marketplace table", 2);
				sqlite3_free(Error);
			}
			else {
				logs("Marketplace Database table Created Successfully", 1);
				query = "CREATE TABLE IF NOT EXISTS Inventory ("
					"userID INTEGER PRIMARY KEY AUTOINCREMENT,"
					"item INTEGER,"
					"amount INTEGER);";

				exit = sqlite3_open(s, &db);
				exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
				if (exit != SQLITE_OK) {
					std::cout << "\nError While Creating Table:" << Error;
					logs("error on  table", 2);
					sqlite3_free(Error);
				}
				else {
					logs("Inventory table was created successfully!", 1);



				}
			}
	}

	sqlite3_close(db);

	return 0;
}
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	std::ofstream data;
	for (int i = 0; i < argc; i++) {

		data.open("c://data.txt");
		if (data.is_open()) {
			data << " " << argv[i];
				i++;
		
		}
		std::cout << "|" << azColName[i] << " -> " << argv[i] << "|" << std::endl;
		std::cout << "-------------\n";
	}

	//std::cout << std::endl;
	data.close();
	return 0;
}
void SettingsTab() {
	int sans = 0;
	system("cls");
	logs("Here you can customize this application as you desire", 1);
	std::cout << "\n\n[0] Advanced Mode (enable commands) \n\n-> ";
	color(14);
	std::cin >> sans;
	if (sans == 0) {
		logs("note that this will disable all the visual options to make the application look cleaner but this is mainly for dev", 1);
		color(8);
		logs("[1 yes/ 2 no]Are you sure you want to enable this? You can revert this at any time in settings.", 2);
		int sans1 = 0;
		color(14);
		std::cin >> sans1;
		if (sans1 == 1) {
			std::ofstream advanced_mode;
			advanced_mode.open("c://dbincpp_settings.txt");
			if (advanced_mode.is_open()) {
				advanced_mode << "1";
				advanced_mode.close();

			}

		}

	}


}
void Checkforsettings() {
	std::ofstream data;
	data.open("c://dbincpp_settings.txt", std::ios::app);
	if (data.is_open()) {
		data.close();
		std::fstream data;
		data.open("c://dbincpp_settings.txt");
		std::string line;
		if (data.is_open()) {
			while (getline(data, line)) {
				if (line != "1") {
					data.close();
					isChecked = true;
				}
				else {
					advanced_mode = true;
					isChecked = true;
				}
			}
		}
	}
	else {
		logs("Your Settings Couldn't Be Loaded Due To Insufficient Permissions.", 3);
		Sleep(2000);
		isChecked = true;
		main(dir);
	}

}