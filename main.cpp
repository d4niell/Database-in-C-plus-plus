#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdio.h>
int ans = 0;

bool logs(std::string message, int type);
std::string username;
std::string password;
static int createTable(const char* s);
static int createDB(const char* s);
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
			return 0;
		}
	}
	else
	{
		perror("Admin privileges is required");
		Sleep(2000);
		system("exit");
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
		std::cout << message << "\n";
		color(8);
		break;
	case 2:
		color(4);
		std::cout << message << "\n";
		color(8);
		break;
	case 3:
		color(6);
		std::cout << message << "\n";
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




int main(const char* s) {
	const char* dir = "c:\\Database.db";
	CheckForPrivilege();
	sqlite3* db;
	createDB(dir);
	createTable(dir);

start:
	std::cout << "\n[0] View Database";
	std::cout << "\n[1] Register.\n";
	std::cout << "[2] Login.\n\n";
	std::cout << "[3] Exit.\n\n:";
	std::cin >> ans;
	if (ans == 0) {

		char* error;
		logs("Attempting to view Database", 1);
		SendQuery(s, "SELECT * FROM User;");

	}
	if (ans == 3) {
		system("exit");

	}
	if (ans == 1) {

	}
	else {
		logs("Invalid Input", 2);
		system("cls");

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
		logs("Table Created Successfully", 1);
	}



	return 0;
}