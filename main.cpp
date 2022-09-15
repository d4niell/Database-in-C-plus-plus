#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdio.h>
int ans = 0;
int isAdmin = 0;
bool isLoginned = false;
int isChecked = false;
const char* dir = "c:\\Database.db";
bool advanced_mode = false;
bool logs(std::string message, int type);
std::string username;
std::string password;
static int createTable(const char* s);
void SettingsTab();
void Checkforsettings();
int main(const char* s);
static int createDB(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int insertData(const char* s, std::string sql);
void SaveData_local(std::string message) {
	std::ofstream SaveData_local;
	SaveData_local.open("C://logs.txt");
	if (SaveData_local.is_open()) {
		logs("", 2);
	}

}

bool CheckForPrivilege() {
	std::ofstream savedata;
	savedata.open("c://adminprivilegetest.txt");
	if (savedata.is_open()) {
		if (remove("c://adminprivilegetest.txt") == 0) {
			isAdmin = 1;
			savedata.close();
			return 0;
		}
	}
	else
	{
		perror("Admin privileges is required");
		Sleep(2000);
		//system("exit");
		return 1;
	}

}

void color(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);

}

bool logs(std::string message, int type) { //prints out logs
	switch (type) {
	case 1:
		color(2);
		std::cout <<"[*] " <<message << "\n";
		color(8);
		break;
	case 2:
		color(4);
		std::cout << "[!] "<<message << "\n";
		color(8);
		break;
	case 3:
		color(6);
		std::cout <<"[x] " <<message << "\n";
		color(8);
		break;
	}


	return 0;
}

void SendQuery(const char* s, std::string Query) {

	sqlite3* db;
	char* Error;
	int exit = 0;
	exit = sqlite3_open(s, &db);
	exit = sqlite3_exec(db, Query.c_str(), NULL, 0, &Error);
	if (exit != SQLITE_OK) {
		std::cout << "\nError While Creating Table:" << Error;
		sqlite3_free(Error);
	}

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
		insertData(dir, save);
		logs("registration succesfull!",1);
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
	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
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
	exit = sqlite3_exec(DB, sql.c_str(), callback, NULL, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		std::cout << "Records selected Successfully!" << std::endl;
	sqlite3_close(DB);

	return 0;
}

bool Login() {
	std::string m_login;
	std::string m_password;
	int exit = 0;
	system("cls");
	sqlite3* DB;
	sqlite3_open(dir, &DB);
	char* Error;
	color(14);
	std::cout << "[*] Login Panel";
	color(8);
	std::cout << "\n		Username:"; std::cin >> username; std::cout << "\n\n		Password:"; std::cin >> password;
	std::string query = "SELECT * FROM User WHERE username ='" + username + "', AND password = '" + password + "';";
	exit = sqlite3_open(dir, &DB);
	exit = sqlite3_exec(DB, query.c_str(), NULL, 0, &Error);
	if (exit != SQLITE_OK) {
		std::cout << "\nError While trying to login:" << Error;
		sqlite3_free(Error);
	}
	if (m_login == username && m_password == password) {
		std::cout << "PERFECT CODENZ";
		system("pause");
	}

	return 0;
}


int main(const char* s) {
	system("title dbincpp");

	if (isChecked == false) {
		Checkforsettings();
	}

	CheckForPrivilege();
	if (isLoginned == false) {
		logs("You're not logged in, please make sure to do so or register by pressing 2", 2);
	}
	sqlite3* db;
	createDB(dir);
	createTable(dir);

start:
	std::cout << "\n\n[0] View Database";
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
	}

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
		"cash INTEGER);";

	char* Error;
	int exit = 0;
	exit = sqlite3_open(s, &db);
	exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
	if (exit != SQLITE_OK) {
		std::cout << "\nError While Creating Table:" << Error;
		sqlite3_free(Error);
	}
	else {
		logs("User table Created Successfully", 1);
		query = "CREATE TABLE IF NOT EXISTS ItemDB ("
			"itemID int PRIMARY KEY AUTOINCREMENT,"
			"itemName TEXT NOT NULL);";
		exit = sqlite3_open(s, &db);
		exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
		if (exit != SQLITE_OK) {
			std::cout << "\nError While Creating Table:" << Error;
			sqlite3_free(Error);
		}
		else {
			logs("Item Database table Created Successfully", 1);
			query = "CREATE TABLE IF NOT EXISTS Marketplace ("
				"userID INTEGER PRIMARY KEY AUTOINCREMENT,"
				"price INTEGER);";

			exit = sqlite3_open(s, &db);
			exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
			if (exit != SQLITE_OK) {
				std::cout << "\nError While Creating Table:" << Error;
				sqlite3_free(Error);
			}
			else {
				logs("Marketplace Database table Created Successfully", 1);
				query = "CREATE TABLE IF NOT EXISTS Inventory ("
					"userID INTEGER PRIMARY KEY AUTOINCREMENT,"
					"item int FOREIGN KEY REFERENCES ItemDB(itemID),"
					"amount INTEGER);";
					
				exit = sqlite3_open(s, &db);
				exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
				if (exit != SQLITE_OK) {
					std::cout << "\nError While Creating Table:" << Error;
					sqlite3_free(Error);
				}
				else {
					logs("Inventory table was created successfully!", 1);
				
				}
			
			}
		}
	}

	sqlite3_close(db);

	return 0;
}
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		// column name and value
		std::cout << azColName[i] << ": " << argv[i] << std::endl;
	}

	std::cout << std::endl;

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
		logs("Your Settings Couldn't Be Loaded Due To Insufficient Permissions.",3);
			Sleep(2000);
			isChecked = true;
			main(dir);
	}

}