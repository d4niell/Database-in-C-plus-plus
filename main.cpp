#include "sqlite/sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <stdio.h>
int ans = 0;

bool logs(std::string message, int type);
std::string username;
std::string password;

void SaveData_local(std::string message) {
	std::ofstream SaveData_local;
	SaveData_local.open("C://logs.txt");
	if (SaveData_local.is_open()) {
		logs("",2);
}

}

void CheckForPrivilege() {
	std::ofstream savedata;
	savedata.open("c://adminprivilegetest.txt");
	if (savedata.is_open()) {
		if (remove("c://adminprivilegetest.txt") == 0) {
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

void SendQuery(std::string Query) {
	char* err;
	sqlite3* db;
	const sqlite3_stmt* stmt;
	sqlite3_open("Kouluhomma.db", &db);
	int a1 = sqlite3_exec(db, Query.c_str(), NULL,NULL, &err);
	if (a1 != SQLITE_OK) {
		logs("Query Request Failed. Reason:", 3);
		std::cout << &err;
	
	}

}




int main() {
	char* err;
	sqlite3* db;
	const sqlite3_stmt* stmt;
	color(8);
	CheckForPrivilege();
	sqlite3_open("Kouluhomma.db", &db);
	int a1 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS UserDB (id integer PRIMARY KEY, name varchar(20), password varchar(20));", NULL, NULL, &err);
	int a2 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS ItemDB (id integer PRIMARY KEY, itemID varchar(20));", NULL, NULL, &err);
	int a3 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS InventoryDB (id integer PRIMARY KEY, itemID integer FOREIGN KEY, amount integer)", NULL, NULL, &err);
	int a4 = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS MarketplaceDB (id integer PRIMARY KEY, price integer)",NULL,NULL, &err);
	if (a1 || a2 || a3 || a4 != SQLITE_OK) {
		logs("Creating Database Failed. Reason:", 3);
		std::cout << &err;
	}

start:
	std::cout << "\n[0] View Database";
	std::cout << "\n[1] Register.\n";
	std::cout << "[2] Login.\n\n";
	std::cout << "[3] Exit.\n\n:";
	std::cin >> ans;
	if (ans == 0) {
		std::string b2a = "SELECT * FROM UserDB";
		int b2 = sqlite3_exec(db, b2a.c_str(), NULL, NULL, NULL);
		if (b2 != SQLITE_OK) {
			std::cout << &err;
			
		}
		else {
			system("cls");
			std::cout << "\rOUTPUT:" <<b2;
			system("pause");
		}
	}
	if (ans == 3) {
		system("exit");
	
	}
	if (ans == 1) {
		begin:
			system("cls");
			std::cout << "Enter Username:"; std::cin >> username;
			std::cout << "\nEnter Password:"; std::cin >> password;
			if (username.length() < 3) {
				std::cout << "Username's length is not long enough. Expected Length (>3)";
				goto begin;
			}
			if (password.length() < 3) {
				std::cout << "Password's length is not long enough. Expected Length (>3)";
				goto begin;
			}
			else {
				username = username;
				password = password;
				SendQuery("INSERT INTO UserDB(name, password) VALUES(" + username + ", " + password + "");
			}
	}
	else {
		logs("Invalid Input", 2);
		system("cls");
		goto start;
	}

	return 0;
}