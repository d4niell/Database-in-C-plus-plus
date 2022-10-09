#include "sqlite/sqlite3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <ctime>
#define max 8 // define the max string  
std::string strings[max];
int ans = 0;
int isAdmin = 0;
bool isLoginned = false;
bool findUserInventory(int _id);
int isChecked = false;
bool database_created = false;
const char* dir = "C:\\Database.db";
static bool isBeta = true;
static int fetch_inventory(const char* s, std::string sql);
bool advanced_mode = false;
static std::string version = "1.1.3";
bool logs(std::string message, int type);
std::string username;
std::string password;
static int createTable(const char* s);
static int fetch_items(const char* s, std::string sql);
void Messages();
void SettingsTab();
std::vector<std::string> lines;
void fetchcashAmount();
void Checkforsettings();
void delete_marketplace_item();
void ATM();
void Marketplace();
void view_marketplace();
void userPanel();
std::string line;
void userInput(int cls, int type, std::string o1, std::string o2, std::string o3, std::string o4);
int main(const char* s);
static int createDB(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int insertData(const char* s, std::string sql);
static int fetch_messages(const char* s, std::string sql);
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
	int mainAns = 0;
	int userpanelAns = 0;
	int ATMAns = 0;
	int cash = 0;
	int marketplaceAns = 0;
	std::string item = "";
	int view_marketplace = 0;
	int add_marketplace_item = 0;
	int view_marketplace_edit = 0;
	int Settings_tab = 0;
	int edit_marketplace_item = 0;
	std::string data;
	std::string data1;
	int Messages = 0;
	std::string data2;
	std::string data3;
	int inventory_input = 0;
	std::string uid;
	int money;
	bool Save_Credentials = false;
	std::string user_un;
	std::string user_pw;
	std::string col_name;


}user;
struct {
public:
	std::string item_name;
	int item_price;
}market;
struct {
public:
	std::string purchase_name;
	int		    purchase_price;

}atm;
struct {
public:
	char** callback;
}db;

BOOL CheckforPrivilege() { //checks for admin privieleges
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

void color(int color) { //with this fucntion we can change the console color (normally the whole console changes it's color, this function makes it so you can change even 1 word to different color
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
	Sleep(100);
	return 0;
}
bool Register() {
	system("cls");
	color(14);
	std::cout << "[*] Registration Panel";
	color(8);
	std::cout << "\n		Username:"; color(14); std::cin >> username; color(8); std::cout << "\n\n		Password:"; color(14); std::cin >> password; color(8);
	if (username.length() < 3) {
		logs("username length is too short", 2);
		if (password.length() < 3) {
			logs("password length is too short", 2);
			return 1;
		}

	}
	else {
		std::string save = "INSERT INTO User (username, password) VALUES ('" + username + "','" + password + "');";
		insertData(dir, save);
		logs("registration succesfull!", 1);
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
		return 0;
}
void highlighter(std::string message, std::string highlightedMessage) { //useless for now
	std::cout << message; color(14); std::cout << highlightedMessage << "\n\n"; color(8);
}
void Addmoney(std::string username, std::string amount) {
	std::string::size_type sz;
	std::string sql = "UPDATE User SET cash =" + amount + " WHERE username = '" + username + "';";
	insertData(dir, sql);
	int i_amount = std::stoi(amount, &sz); //converts amount to int so that we can count it
	logs("Money was added successfully!", 1);
	userPanel();
}
void view_ATM_purchase_history() {
	std::fstream myfile;
	myfile.open("c://" + username + "_dbincpp_atm.txt");
	if (!myfile.is_open()) {
		logs("Error Occured", 3);
		perror("\n");
	}
	else {
		logs("Purchase History", 1);
		std::string line;
		while (getline(myfile, line)) { //getline loop 

			std::cout << "------------------->"; color(14); std::cout << line << std::endl; color(8);
			std::cout << "\n\n";
		}
	}
	system("pause");
	ATM();
	myfile.close();
}
void add_ATM_purchase_history(std::string purchase_item, int purchase_price) {
	time_t now = time(0);
	char* dt = ctime(&now);
	std::ofstream myfile;
	myfile.open("c://" + username + "_dbincpp_atm.txt", std::ios::app);
	if (!myfile.is_open()) {
		logs("Error Occured", 3);
		perror("\n");
	}
	else {
		myfile << "[purchased] date:" << dt << "type: [purchase] name: " << purchase_item << " price: " << purchase_price << "\n";
	}
	myfile.close();
}
void sendMoney() {
	std::ostringstream str1;
	std::string u_username;
	int u_user_amount;
	int amount;
	color(8);
	std::cout << "username\n> "; color(14); std::cin >> u_username; color(8); std::cout << "\namount\n> "; color(14); std::cin >> amount;
	if (user.cash >= amount) {
		std::string query_1 = "SELECT cash FROM User WHERE username = '" + u_username + "'";
		selectData(dir, query_1);
		u_user_amount = stoi(user.data1);
		int new_user_amount = u_user_amount + amount;
		int local_user_amount = user.cash - amount;
		std::ostringstream str2;
		str2 << local_user_amount;// storing local_user_amount to str2 so that we can use it on querys
		str1 << new_user_amount; // storgin new_user_amount to str1 so that we can use it on querys
		std::string i_pay_amount = str1.str(); // this function converts new_user_amount (which we stored into str1) into string so we can use it on querys
		std::string local_amount = str2.str(); // /--/
		std::cout << i_pay_amount << " " << u_username;
		query_1 = "UPDATE User SET cash = " + local_amount + " WHERE username = '" + username + "';";
		insertData(dir, query_1);
		query_1 = "UPDATE User SET cash = " + i_pay_amount + " WHERE username = '" + u_username + "';";
		insertData(dir, query_1);
		logs("You're a nice person", 1);
		system("pause");
		userPanel();
	}
	else {
		logs("You have insufficient funds", 2);
		ATM();
	}
}
void ATM() {
	//	fetchcashAmount();
		//highlighter("Welcome back, ", username);
	color(8); std::cout << "\nYou Currently have: $"; color(14); std::cout << user.cash << "\n"; color(8);
	userInput(0, 3, "View History", "Send Money", "Manipulate money", "Back");
	std::string amount;
	switch (user.ATMAns) {
	case 1:
		view_ATM_purchase_history();
		break;
	case 2:
		sendMoney();
		break;
	case 3:
		//std::string amount;
		color(8);
		std::cout << "\n\nEnter the amount you want to change your cash to:"; color(14); std::cin >> amount;	color(8);
		Addmoney(username, amount);
		break;
	case 4:
		userPanel();
		break;
	}
}
void additem_marketplace() {
	std::string item_name;
	std::string item_price;
	userInput(1, 6, "Select Item", "View Marketplace", "ATM", "Back");
	switch (user.add_marketplace_item) {
	case 1:
		color(8);
		std::cout << "Select a name:"; color(14); std::cin >> item_name;
		if (item_name.length() >= 1) {
			color(8); std::cout << "\nSelect a price for:"; color(14);
			std::cout << item_name << "\n> ";
			std::cin >> item_price; color(8);
			if (item_price.length() > 0) {
				logs("adding item.", 1);
				std::string query = "INSERT INTO Marketplace (userID,itemName, price) VALUES (" + user.uid + ",'" + item_name + "', " + item_price + "); ";
				insertData(dir, query);
				//std::cout << query;
				Marketplace();
			}
		}
		break;
	case 2:
		view_marketplace();
		break;
	case 3:
		ATM();
		break;
	case 4:

		break;
	}

}
void confirm_market_purchase() {
	int desired_price;
	int user_funds;
	//color(14); std::cout << user.cash; color(8); std::cout << "<<"; color(14);std::cout << market.item_price;
	if (user.cash < market.item_price) //compares the market place item's desired price if the user has enough money
	{
		logs("you have insufficient funds", 2);
		Marketplace();
	}
	else
	{
		std::string add_inv = "INSERT INTO Inventory (userID, item, amount) VALUES (" + user.uid + ", '" + market.item_name + "', 1)";
		insertData(dir, add_inv);
		std::ostringstream str1;
		int pay_amount = user.cash - market.item_price;
		str1 << pay_amount; //stores pay_amount into str1
		std::string i_pay_amount = str1.str(); //converts pay_amount which is stored in str1 into i_pay_amount (string)
		std::string query = "UPDATE User SET cash = " + i_pay_amount + " WHERE id =" + user.uid + ";";
		insertData(dir, query);
		std::cout << "\n";
		logs("Thank you for purchasing", 1);
		std::cout << "You have $"; color(14); std::cout << pay_amount; color(8); std::cout << " left.";
		add_ATM_purchase_history(market.item_name, market.item_price);
		std::string delete_item = "DELETE FROM Marketplace WHERE itemName = '" + market.item_name + "';";

		selectData(dir, delete_item);
		Sleep(500);
		userPanel();
	}

}
void buy_marketplace_item() {
	int price;
	std::string name;
	color(8); std::cout << "name\n> "; color(14); std::cin >> name; color(8);
	if (name.length() >= 1) {
		std::string query = "SELECT itemName, price from Marketplace WHERE itemName ='" + name + "';";
		selectData(dir, query);
		//	system("pause");
		market.item_name = user.data;
		market.item_price = stoi(user.data1);
		color(8); std::cout << "\nSelected item is: "; color(14); std::cout << market.item_name; color(8); std::cout << "\ncost: $"; color(14); std::cout << market.item_price << "\n";
		color(8); std::cout << "Do you confirm this purchase? type: \""; color(14); std::cout << "confirm"; color(8); std::cout << "\". To go back, type: \""; color(14); std::cout << "back"; color(8); std::cout << "\"\n\n:";
		std::string ans;
		color(14);
		std::cin >> ans;
		if (ans == "confirm") {
			color(8);
			confirm_market_purchase();
		}
		if (ans == "back") {
			color(8);
			Marketplace();
		}

	}

}
void edit_marketplace_price() {
back:
	std::ostringstream str1;
	std::string item_name;
	int item_price;
	std::string query = "SELECT itemName, price FROM Marketplace WHERE userID = " + user.uid + ";";
	color(8); std::cout << "Your item(s) you have listed:\n"; color(14);
	fetch_items(dir, query);
	color(8); std::cout << "\n Select item to edit\nname:"; color(14); std::cin >> item_name;
	color(8); std::cout << "\nnew price\n> "; color(14); std::cin >> item_price; color(8);
	if (item_price <= 0) {
		logs("you can't list free items", 2);
		goto back;
	}
	else {
		str1 << item_price;
		std::string new_price = str1.str();
		query = "UPDATE Marketplace SET price = " + new_price + " WHERE userID = " + user.uid + " AND itemName ='" + item_name + "';";
		insertData(dir, query);
		logs("price updated succesfully", 1);
		Marketplace();
	}
}
void edit_marketplace() {
	userInput(1, 9, "Edit price", "Delete Item", "View Marketplace", "Back");
	switch (user.edit_marketplace_item) {
	case 1:
		edit_marketplace_price();
		break;
	case 2:
		delete_marketplace_item();
		break;
	case 3:
		view_marketplace();
		break;
	case 4:
		Marketplace();
		break;
	}
}
void delete_marketplace_item() {
	std::string item_name;
	std::string query = "SELECT itemName, price FROM Marketplace WHERE userID = " + user.uid + ";";
	color(8); std::cout << "Your item(s) you have listed:\n"; color(14);
	fetch_items(dir, query);
	color(8); std::cout << "\n Select item to delete\nname:"; color(14); std::cin >> item_name;
	if (item_name.length() >= 1) {
		color(8); std::cout << "Selected item is: "; color(14); std::cout << market.item_name; color(8); std::cout << "\ncost: $"; color(14); std::cout << market.item_price << "\n";
		color(8); std::cout << "Do you confirm this deletion? type: \""; color(14); std::cout << "confirm"; color(8); std::cout << "\". To go back, type: \""; color(14); std::cout << "back"; color(8); std::cout << "\"\n\n:";
		std::string ans;
		color(14);
		std::cin >> ans;
		if (ans == "confirm") {
			color(8);
			query = "DELETE FROM Marketplace WHERE userID =" + user.uid + " AND itemName = '" + item_name + "';";
			selectData(dir, query);
			logs("Item has been deleted.", 2);
			view_marketplace();
		}
		if (ans == "back") {
			color(8);
			view_marketplace();
		}
	}
}

void view_marketplace() {
	sqlite3* db;
	system("cls");
	color(8);
	std::string query = "SELECT itemName, price FROM Marketplace";
	color(14);
	fetch_items(dir, query);
	color(8);
	std::cout << "\nyou currently have: $"; color(14); std::cout << user.cash; color(8);
	userInput(0, 7, "Buy Item", "Edit Item", "Get Money", "Back");
	switch (user.view_marketplace_edit) {
	case 1:
		buy_marketplace_item();
		break;
	case 2:
		edit_marketplace();
		break;
	case 3:
		user.ATMAns = 3;
		ATM();
		break;
	case 4:
		Marketplace();
		break;
	}
}
void Marketplace() {
	userInput(0, 4, "View Marketplace", "Add Item", "Back", "Exit");
	switch (user.marketplaceAns) {
	case 1:
		view_marketplace();
		break;
	case 2:
		additem_marketplace();
		break;
	case 3:
		userPanel();
		break;
	case 4:
		userPanel();
		break;
	}
}
void Inventory() {
	//std::cout << "item name:";
	std::string query = "SELECT item, amount FROM Inventory WHERE userID = " + user.uid + ";";
	color(14);
	fetch_inventory(dir, query);
	color(8);
	userInput(0, 10, "Buy Item(s)", "List Item(s)", "View Purchase History", "Back");
	switch (user.inventory_input) {
	case 1:
		view_marketplace();
		break;
	case 2:
		additem_marketplace();
		break;
	case 3:
		view_ATM_purchase_history();
		break;
	case 4:
		userPanel();
		break;
	}
}
void fetchUID() { //gets the user UID for info panel and saves the variable for future functions
	std::string query = "SELECT id FROM User WHERE username = '" + username + "';";
	selectData(dir, query);
	user.uid = user.data1;
}
void fetchCASH() { // same thing but does it for cash
	std::string query = "SELECT cash FROM User WHERE username = '" + username + "';";
	selectData(dir, query);
	user.cash = stoi(user.data1);
}
void send_message() {
	std::string s_username;
	std::string s_message;
	std::string s_user_uid;
	std::cout << "\nusername\n> "; color(14); std::cin >> s_username; color(8); std::cout << "\nmessage\n> "; color(14); std::cin >> s_message;
	color(8);
	if (s_message.length() <= 3) {
		logs("message is too short, please avoid spamming", 2);
	}
	else {
		std::string find_user_uid = "SELECT id FROM User WHERE username = '" + s_username + "';";
		selectData(dir, find_user_uid); // We first need to know the users uid before we can proceed (messages table only has userID which can be integrated to id from User)
		s_user_uid = user.data1; //user UID
		std::string send_message = "INSERT INTO Messages (senderID, sender_name, receiverID, message) VALUES (" + user.uid + ",'" + username + "'," + s_user_uid + ",'" + s_message + "');";
		insertData(dir, send_message);
		logs("Message sent succesfully", 1);
		Messages();
	}
}
void view_messages() {
	std::string u_sender;
	std::string query = "SELECT sender_name, message FROM Messages WHERE receiverID = " + user.uid + ";";  //we're fetching messages via uid and after that's done we convert the uid to username for it to be readable :D
	color(14);
	fetch_messages(dir, query);
	color(8);
	Messages();

}
void Messages() {
	userInput(0, 11, "View Messages", "Send Message", "Back", "");
	switch (user.Messages) {
	case 1:
		view_messages();
		break;
	case 2:
		send_message();
		break;
	case 3:
		userPanel();
		break;


	}

}
void userPanel() {
	time_t now = time(0); //for time
	char* dt = ctime(&now); //for time
	fetchUID(); //calling these functions instantly once in userpanel
	fetchCASH(); //-//
	//fetchMessages();
	system("cls");
	color(8); std::cout << "> "; color(11); std::cout << dt; //prints out the time from ctime
	color(8); std::cout << "> {"; color(7); std::cout << "USERNAME: "; color(14); std::cout << username; color(7); std::cout << " | UID : "; color(14); std::cout << user.uid; color(7); std::cout << " | CASH : "; color(14); std::cout << user.cash; color(8); std::cout << "}";
	userInput(0, 2, "ATM", "Marketplace", "Inventory", "Messages");
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
		Inventory();
		break;
	case 4:
		Messages();
		break;
	}

}
int len(std::string str) //useless for now
{
	int length = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		length++;

	}
	return length;
}
void split(std::string str, char seperator) //useless for now
{
	int currIndex = 0, i = 0;
	int startIndex = 0, endIndex = 0;
	while (i <= len(str))
	{
		if (str[i] == seperator || i == len(str))
		{
			endIndex = i;
			std::string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);
			strings[currIndex] = subStr;
			currIndex += 1;
			startIndex = endIndex + 1;
		}
		i++;
	}
}
bool Login() {
	int attempts = 0;
start:
	std::string m_username;
	std::string m_password;
	system("cls");
	color(14);
	std::cout << "[*] Login Panel";
	color(8);
	if (attempts == 3) {
		std::cout << "\n";
		logs("too many failed login attempts", 2);
		Sleep(1000);
		main(dir);
	}
	std::cout << "\nUsername\n> "; color(14); std::cin >> username; color(8); std::cout << "\n\nPassword\n> "; color(14); std::cin >> password;
	std::string sql = "SELECT username, password FROM User WHERE username = '" + username + "' AND password = '" + password + "';";
	selectData(dir, sql);
	if (username.length() > 3 && password.length() > 3) {
		if (!user.data.find(username)) {
			if (!user.data1.find(password)) {
				if (user.Save_Credentials == true) { //for "save credentials" feature //todo
					std::ofstream save_data;
					save_data.open("c://credentials.txt");
					if (save_data.is_open()) {
						save_data << username << " " << password;
						save_data.close();
					}
				}
				userPanel();
			}
		}
		else
		{
			logs("Invalid Username/password", 2);
			Sleep(1000);
			attempts++;
			goto start;
		}


	}
	else {
		logs("username/password length is invalid", 2);
		Sleep(1000);
		attempts++;
		goto start;
		Login();
	}

	//system("pause");

	return 0;
}

void userInput(int cls, int type, std::string o1, std::string o2, std::string o3, std::string o4) { //this is the legendary function which replaces all if clauses and other reappearing things such as questions and console inputs

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
	color(9);
	std::cout << "\n\n> "; color(7); std::cout << "dbincpp"; color(7); std::cout << " v"; color(8); std::cout << "["; color(5); std::cout << version; color(8); std::cout << "] \n";
	color(8);

	std::cout << "\n|	["; color(5); std::cout << "1"; color(8); std::cout << "] "; color(7); std::cout << o1; color(8);
	std::cout << "\n|	["; color(5); std::cout << "2"; color(8); std::cout << "] "; color(7); std::cout << o2; color(8);
	std::cout << "\n|	["; color(5); std::cout << "3"; color(8); std::cout << "] "; color(7); std::cout << o3; color(8);
	std::cout << "\n|	["; color(5); std::cout << "4"; color(8); std::cout << "] "; color(7); std::cout << o4; color(8);
	color(14); std::cout << "\n		> "; std::cin >> ans; color(8);
	std::cout << "------------------------------------------------\n";
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
	switch (type) { //this switch clause indexes the ans from this function to the other functions so that we can use the user input value on different functions
	case 1:
		user.mainAns = ans; //for main function
		break;
	case 2:
		user.userpanelAns = ans; //for user panel function
		break;
	case 3:
		user.ATMAns = ans; //for atm function
		break;
	case 4:
		user.marketplaceAns = ans; //for market main function
		break;
	case 5:
		user.view_marketplace = ans; //view marketplace function
		break;
	case 6:
		user.add_marketplace_item = ans; //add item fucntion
		break;
	case 7:
		user.view_marketplace_edit = ans; //view marketplace function
		break;
	case 8:
		user.Settings_tab = ans; //settings tab function
		break;
	case 9:
		user.edit_marketplace_item = ans;//for edit marketplace function
		break;
	case 10:
		user.inventory_input = ans;
		break;
	case 11:
		user.Messages = ans;
	}

}
int main(const char* s) {
	//view_marketplace();

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
	system("title Database In C++");

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
		logs("Thank you for testing me out, have fun!", 1);
		system("exit");
		break;

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
	system("title initializing databases");
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
		//	logs("error on user table", 2);
		sqlite3_free(Error);
	}
	else {
		//logs("User table Created Successfully", 1);	
		query = "CREATE TABLE IF NOT EXISTS Marketplace ("
			"userID INTEGER,"
			"itemName TEXT,"
			"price INTEGER);";

		exit = sqlite3_open(s, &db);
		exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
		if (exit != SQLITE_OK) {
			std::cout << "\nError While Creating Table:" << Error;
			//	logs("error on Marketplace table", 2);
			sqlite3_free(Error);
		}
		else {
			//logs("Marketplace Database table Created Successfully", 1);
			query = "CREATE TABLE IF NOT EXISTS Inventory ("
				"userID INTEGER,"
				"item TEXT,"
				"amount INTEGER);";

			exit = sqlite3_open(s, &db);
			exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);
			if (exit != SQLITE_OK) {
				std::cout << "\nError While Creating Table:" << Error;
				//	logs("error on  table", 2);
				sqlite3_free(Error);
			}
			else {
				//	logs("Inventory table was created successfully!", 1);
				query = "CREATE TABLE IF NOT EXISTS Messages ("
					"MessageID	INTEGER UNIQUE,"
					"senderID INTEGER,"
					"sender_name TEXT,"
					"receiverID INTEGER,"
					"message TEXT,"
					"PRIMARY KEY(MessageID AUTOINCREMENT));";

				exit = sqlite3_open(s, &db);
				exit = sqlite3_exec(db, query.c_str(), NULL, 0, &Error);

				if (exit != SQLITE_OK) {
					std::cout << "\nError While Creating Messages Table:" << Error;
					//	logs("error on  table", 2);
					sqlite3_free(Error);
					system("pause");
				}
				else {
					Sleep(200);
					system("title databases initialized!");
					Sleep(200);
				}

			}
		}
	}

	sqlite3_close(db);

	return 0;
}
static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
	int i;
	for (i = 0; i < argc; i++) {
		// printf(" %s\n", argv[i] ? argv[i] : "NULL");
		user.data = argv[0];
		user.data1 = argv[i];
	}
	printf("\n");
	return 0;
}
static int callback_marketplace_items(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		if (i == 0)
		{
			printf("name: %s price: %s", argv[0], argv[1]);
		}
	}
	printf("\n");
	return 0;
}
static int fetch_items(const char* s, std::string sql)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback_marketplace_items, 0, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		return(0);
}
static int callback_messages(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		if (i == 0)
		{
			printf("FROM: %s ", argv[0]);
		}
		if (i == 1)
		{
			printf("MESSAGE: %s", argv[1]);
		}

	}
	printf("\n");
	return 0;
}
static int fetch_messages(const char* s, std::string sql)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback_messages, 0, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		return(0);
}
static int callback_inventory(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		if (i == 0)
		{
			printf("item: %s ", argv[0]);
		}
		if (i == 1)
		{
			printf("quantity: %s", argv[1]);
		}

	}
	printf("\n");
	return 0;
}
static int fetch_inventory(const char* s, std::string sql)
{
	sqlite3* DB;
	char* messageError;
	int exit = sqlite3_open(s, &DB);
	/* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
	exit = sqlite3_exec(DB, sql.c_str(), callback_inventory, 0, &messageError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error in selectData function." << std::endl;
		sqlite3_free(messageError);
	}
	else
		return(0);
}

void SettingsTab() {
	userInput(1, 8, "Advanced Mode (W.I.P)", "Clear Logs", "Save Credentials", "Back");
	switch (user.Settings_tab) {
	case 1:
		system("pause");
		break;
	case 2:
		remove("dbincpp_logs.txt");
		logs("Logs cleared succesfully", 1);
		Sleep(500);
		main(dir);
		break;
	case 3:
		user.Save_Credentials = true;
		logs("credentials will be saved after you've loginned.", 1);
		Sleep(1000);
		Login();
		break;
	case 4:
		main(dir);
		break;
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