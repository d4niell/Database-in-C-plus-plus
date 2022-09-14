#include "sqlite/sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdio.h>
int ans = 0;

bool logs(std::string message, int type);


void SaveData_local(std::string message) {
	std::ofstream SaveData_local;
	SaveData_local.open("C://logs.txt");
	if (SaveData_local.is_open()) {
		logs("",2);
}

}

void CheckForPrivilege() {
	std::ofstream savedata;
	savedata.open("c://sqlprojekti.txt");
	if (savedata.is_open()) {
		if (remove("c://sqlprojekti.txt") == 0) {
		}
	}
	else
	{
		perror("Admin privileges is required");
		Sleep(2000);
		system("exit");
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






int main() {
	char* err;
	sqlite3* db;
	const sqlite3_stmt* stmt;
	std::string username;
	std::string password;
	color(8);
	CheckForPrivilege();
	sqlite3_open("Kouluhomma.db", &db);
	std::string b2a = "SELECT * FROM UserDB";
	int b2 = sqlite3_exec(db, b2a.c_str(), NULL, NULL, NULL);
	if (b2 != SQLITE_OK) {
		std::cout << &err;
	}
	int a1 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS UserDB (id integer PRIMARY KEY, name varchar(20), password varchar(20));", NULL, NULL, &err);
	int a2 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ItemDB (id integer PRIMARY KEY, itemID varchar(20));", NULL, NULL, &err);
	int a3 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS InventoryDB (id integer PRIMARY KEY, itemID integer FOREIGN KEY, amount integer)", NULL, NULL, &err);
	int a4 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS MarketplaceDB (id integer PRIMARY KEY, price integer)",NULL,NULL, &err);
	if (a1 || a2 || a3 || a4 != SQLITE_OK) {
		logs("Creating Database Failed. Reason:", 3);
		std::cout << &err;
	}

	start:
	std::cout << "\n[1] Register.\n";
	std::cout << "[2] Login.\n\n";
	std::cout << "[3] Exit.\n\n:";
	std::cin >> ans;
	if (ans == 3) {
		system("exit");
	
	}
	if (ans == 1) {
		
			system("cls");
			std::cout << "Enter Username:"; std::cin >> username;
			std::cout << "\nEnter Password:"; std::cin >> password;
			if (username.length() < 3) {
				std::cout << "Username's length is not long enough. Expected Length (>3)";
			}
			if (password.length() < 3) {
				std::cout << "Password's length is not long enough. Expected Length (>3)";
			}
			else {
				username = username;
				password = password;
				logs("Account Creation was successful", 1);
				std::string b1 = "INSERT INTO UserDB (name, password) VALUES (" + username + "," + password + ")";
				int Createacc = sqlite3_exec(db, b1.c_str(), NULL, NULL, NULL);
				if (Createacc != SQLITE_OK) {
					std::cout << "Couldn't create account. Reason:" << &err;

				}
			}
		
	}
	else {

	}

	return 0;
}