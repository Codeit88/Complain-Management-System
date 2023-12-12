
//Complaint Management System by 21L-1868(SAMI KHOKHER) AND 21L-5483(MOMIN AMIR)

#include<iostream>
#include<vector>
#include<string>
#include<chrono>
#include<ctime>
#include<fstream>
#include<sstream>

using namespace std;


// Define ANSI escape codes for text colors
namespace Color {
	const std::string Reset = "\033[0m";
	const std::string Red = "\033[31m";
	const std::string Green = "\033[32m";
	const std::string Yellow = "\033[33m";
	const std::string Blue = "\033[34m";
	const std::string Magenta = "\033[35m";
	const std::string Cyan = "\033[36m";
	const std::string White = "\033[37m";
}

namespace TextStyle {
	const std::string Bold = "\033[1m";
	const std::string Italic = "\033[3m";
	const std::string Underline = "\033[4m";
	const std::string Strikethrough = "\033[9m";
	const std::string Reset = "\033[0m";
}

string s1;          //it will declare the current user
int primaryFunc, emp1,*ar=NULL;    //it will take user,s primary query
int secondaryFunc;  //it will take user,s secondary query
string complain,name;       //it  will be the teahcers complain
char enter = 'a';               //to go back
bool exitSys = false;
int role;
string date1,date2,roles[5] = { "Admin","Teacher","Manager","Employee","Director" };
string primaryUsers[3] = { "Teacher","Manager","Employee" };


/// <declaring classes name as used for forword declaration>
class Admin;
class Teacher;
class Department;
class Complain;
class Employee;
class Manager;
class Director;
class Job;

vector<Complain*>complains;
vector<Job*>jobs;
class Date {
	int day=0, month=0, year=0;
public:
	Date() {
		// Get the current time point
		auto now = std::chrono::system_clock::now();

		// Convert it to a time_t object
		std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

		// Use localtime_s instead of localtime
		std::tm currentDate;
		localtime_s(&currentDate, &currentTime);

		// Extract and print date components
		this->year = currentDate.tm_year + 1900;
		this->month = currentDate.tm_mon + 1; // Month is 0-indexed
		this->day = currentDate.tm_mday;

	}


	friend bool operator<(const Date& lhs, const Date& rhs) {
		if (lhs.year != rhs.year) {
			return lhs.year < rhs.year;
		}
		if (lhs.month != rhs.month) {
			return lhs.month < rhs.month;
		}
		return lhs.day < rhs.day;
	}

	friend bool operator > (const Date& lhs, const Date& rhs) {
		return rhs < lhs;
	}

	friend bool operator <= (const Date& lhs, const Date& rhs) {
		return !(lhs > rhs);
	}

	friend bool operator >= (const Date& lhs, const Date& rhs) {
		return !(lhs < rhs);
	}

	friend bool operator==(const Date& lhs, const Date& rhs) {
		return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
	}

	friend bool operator!=(const Date& lhs, const Date& rhs) {
		return !(lhs == rhs);
	}


	int operator-(const Date& other) const {
		// Convert both dates to days since epoch for calculation
		std::tm date1 = { 0, 0, 0, day, month - 1, year - 1900 };
		std::tm date2 = { 0, 0, 0, other.day, other.month - 1, other.year - 1900 };

		std::time_t time1 = std::mktime(&date1);
		std::time_t time2 = std::mktime(&date2);

		const int seconds_per_day = 60 * 60 * 24;
		return std::difftime(time1, time2) / seconds_per_day;
	}

	Date(string& formattedDate){
		sscanf_s(formattedDate.c_str(), "%d/%d/%d", &day, &month, &year);
	}

	string getFormattedDate() const {
		return to_string(day) + "/" + to_string(month) + "/" + to_string(year);
	}

	void print() {
		cout << this->day << "/" << this->month << "/" << this->year << "\n";
	}
};
class Department {
	int id;
	string name;
	vector < Complain* > complains;
	vector < Employee* > employees;
	Manager* manager;
public:
	Department(int& id, string name) {
		this->id = id;
		this->name = name;
	}

	void assignManager(Manager* manager) {
		this->manager = manager;
	}

	Manager* getManager() {
		return this->manager;
	}

	int& getId() {
		return this->id;
	}

	int* getComps();

	void addEmployee(Employee* employee) {
		employees.push_back(employee);
	}

	void addComplain(Complain* complain) {
		complains.push_back(complain);
		cout << " Your complain has been successfully added !" << endl;
	}
	int totalEmployees() {
		return this->employees.size();
	}
	int totalComplains() {
		return this->complains.size();
	}

	vector <Complain*> getComplains();

	vector <Employee*> getEmployees();

	string& getName() {
		return this->name;
	}

};
vector<Department*>departments;
class Complain {
	int id;
	string name;
	string status;
	Department* department;
	Teacher* teacher;
	Date* date;
	//bool notification;
public:
	Complain(int& id, string& name, Department* department, Teacher* teacher ,string&status ,Date*date) {
		this->status = status;
		this->name = name;
		this->date = date;
		this->id = id;
		this->teacher = teacher;
		this->department = department;
		this->department->addComplain(this);
	}
	int& getId() {
		return this->id;
	}
	string& getName() {
		return this->name;

	}
	void notifyTeacher();

	Date* getDate() {
		return this->date;
	}

	void notifyManager();
	string& getStatus() {
		return this->status;
	}
	Teacher* getTeacher() {
		return this->teacher;
	}
	Department* getDepartment() {
		return this->department;
	}
	void changeFileStatus(int& id, string newStatus);
	void print();
};

///  declaring Parental class Person
class Person {
protected:
	string name, role;
	bool LoggedIn = false;
public:
	virtual void print() = 0;
	virtual void login() = 0;
};
class Employee :public Person {
	string status;
	int id;
	Department* department;
	Complain* complain;
	Job* job;
	Manager* manager;
public:
	Employee(int id, string name, Department* department, Manager* manager) {
		this->id = id;
		this->status = "Available";
		this->name = name;
		this->department = department;
		this->department->addEmployee(this);
		this->manager = manager;
	}
	int& getId() {
		return this->id;
	}
	int& getDepId() {
		return this->department->getId();
	}
	void manuallyAssigned(Complain*& complain){
		this->complain = complain;
		//this->complain->getStatus() = "Assigned";
		this->status = "Assigned";
	}
	void getAssigned(Complain*& complain) {
		this->complain = complain;
		this->complain->getStatus() = "Assigned";
		this->complain->changeFileStatus(this->complain->getId(), "Assigned");
		this->status = "Assigned";
		makeJob();
	}
	void login() {
		this->LoggedIn = true;
		while (LoggedIn) {
			enter = 'a';
			system("CLS");
			cout << " You are logged in as : " << this->name << endl;
			cout << " 1 ) Update Job Status! " << endl;
			cout << " 2 ) Logout! " << endl;

			cin >> primaryFunc; cin.ignore(); // Add this line to clear the input buffer

			switch (primaryFunc) {
			case 1:
				system("CLS");
				if (this->complain != NULL) {
					cout << "Press 'N' to change your ' " << this->complain->getName() << " ' status " << endl;
					cin >> enter;
					if (enter == 'N' || enter == 'n') {
						this->complain->changeFileStatus(this->complain->getId(), "Resolved");
						this->complain->notifyManager();
					}
				}
				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;
			case 2:
				this->LoggedIn = false;
				break;

			default:
				std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
				break;

			}
		}
	}
	string& getStatus() {
		return this->status;
	}
	string& getName() {
		return this->name;
	}
	void makeJob();
	void print();
	Complain* getcomp() {
		return this->complain;
	}
};
vector<Employee*>employees;
class Teacher :public Person {
	int id;
	bool notify;
	string status;
	Admin* admin;
	vector<Complain*>complains2;
public:
	Teacher(int& id, string& name, Admin& admin) {
		this->id = id;
		this->name = name;
		this->admin = &admin;
		this->role = "Teacher";
	}
	int getId() {
		return this->id;
	}
	void seedComps(Complain*comp){
		this->complains2.push_back(comp);
	}


	void fileComplain(string complain, Department* department) {
		string status = "Active";
		Date* date = new Date;
		int size1 = complains.size();
		size1++;		
		//this->complains2.push_back(new Complain(size1, complain, department, this, status));
		complains.push_back(new Complain(size1, complain, department, this,status,date));
		ofstream outputFile("Complains.txt", std::ios::app);  // Use std::ios::app to append

		if (outputFile.is_open()) {
			// Append the new complaint to the file
			outputFile << complains.back()->getId() << "(" << complains.back()->getName() << ")" << " "
				<< complains.back()->getDepartment()->getId() << " "
				<< complains.back()->getTeacher()->getId() << " " << complains.back()->getStatus() << " "
				<< date->getFormattedDate()<<      endl;

			outputFile.close();
		}
		else {
			cerr << "Error: Unable to open Complains file for writing" << endl;
		}

	}
	bool& getNotify() {
		return this->notify;
	}
	void  login() {
		this->LoggedIn = true;
		while (LoggedIn) {

			this->notify = false;
			for (int i = 0; i < this->complains2.size(); i++)
			{
				if (complains2[i]->getStatus() == "Done") {
					this->notify = true;
					break;
				}
			}
			enter = 'a';
			system("CLS");
			cout << TextStyle::Bold << Color::Green << "\n\n \t\t You are logged in as : " << this->name << TextStyle::Reset <<
				Color::Reset << endl << endl;
			cout << " 1 ) Make a complain! " << endl;
			cout << " 2 ) See your complains! " << endl;
			cout << " 3 ) Logout! " << endl;
			if (this->notify) {
				cout <<Color::Yellow<<"\n Press 4 to see notifications and give your feedback ! "<<Color::Reset;
			}

			cin >> primaryFunc; cin.ignore(); // Add this line to clear the input buffer

			switch (primaryFunc) {
			case 1:
				system("CLS");

				cout << "\n Select your department ! " << endl;

				for (int i = 0; i < departments.size(); i++)
				{
					cout << i + 1 << " : " << departments[i]->getName() << endl;
				}

				cout << 4 << " : " << "Logout" << endl;
				cout << 5 << " : " << "Exit the system!" << endl;


				cin >> secondaryFunc; cin.ignore(); // Add this line to clear the input buffer


				switch (secondaryFunc) {
				case 1:
					cout << "\n Enter your complain : ";
					getline(cin, complain);

					fileComplain(complain, departments[0]);
					this->complains2.push_back(complains[complains.size()-1]);

					cout << Color::Green << " Complain Filed ! " << Color::Reset << endl;

					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;

				case 2:
					cout << "\n Enter your complain : ";
					getline(cin, complain);

					fileComplain(complain, departments[1]);
					this->complains2.push_back(complains[complains.size() - 1]);


					cout << Color::Green << " Complain Filed ! " << Color::Reset << endl;

					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;

				case 3:
					cout << "\n Enter your complain : ";
					getline(cin, complain);

					fileComplain(complain, departments[2]);
					this->complains2.push_back(complains[complains.size() - 1]);


					cout << Color::Green << " Complain Filed ! " << Color::Reset << endl;

					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;

				case 4:
					this->LoggedIn = false;
					break;

				case 5:
					this->LoggedIn = false;
					exitSys = true;
					break;
				default:
					std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
					break;
				}
				break;

			case 2:
				getComplains();
				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;

			case 3:
				LoggedIn = false;
				break;

			case 4:
				if (this->notify) {
					system("CLS");
					for (int i = 0; i < complains2.size(); i++)
					{
						if (complains2[i]->getStatus() == "Done") {
							cout <<complains2[i]->getId() <<" : "<<complains2[i]->getName() << " has status " << complains2[i]->getStatus() << "\n";
							//	cout<<" "
						}
					}
					cout << " Enter your complain id to mark its status ";
					cin >> primaryFunc;
					if (primaryFunc<0 || primaryFunc>complains.size()) {
						cout << " Enter valid number !";
					}

					cout << " 1 : Satisfied \n";
					cout << " 2 : Not Satisfied \n";

					cin >> secondaryFunc;
					Complain* comp=NULL;
					for (int i = 0; i < complains.size(); i++)
					{
						if (complains[i]->getId() == primaryFunc)
							comp = complains[i];
					}

					if (secondaryFunc == 1) {
						comp->changeFileStatus(comp->getId(), "Closed");
					}
					else if (secondaryFunc == 2) {

						comp->changeFileStatus(comp->getId(), "Active");
					}
					else {
						cout << " Enter valid num ";
					}

				}
				else {
					cout << " You have no notifications yet  \n";
				}
				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;
			default:
				std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
				break;
			}
		}
	}

	string& getStatus() {
		return this->status;
	}
	void getComplains() {

		if (complains2.size() == 0) {
			cout << " You have no complains !\n";
			return;
		}
		for (int i = 0; i < complains2.size(); i++)
		{
			cout << complains2[i]->getName() << " for department : " <<Color::Green <<complains2[i]->getDepartment()->getName()
				<<Color::Reset<< " | " <<Color::Red<< complains2[i]->getStatus() <<Color::Reset << " | "; complains2[i]->getDate()->print();
			cout << endl;
		}
	}
	string getname() {
		return this->name;
	}
	void print();
};
vector<Teacher*>teachers;
class Manager :public Person {
	bool notify;
	int id;
	Department* department;
	Admin* admin;

public:
	Manager(int& id, string name, Admin& admin, Department& department) {
		this->id = id;
		this->name = name;
		this->admin = &admin;
		this->department = &department;
		this->department->assignManager(this);
		this->role = "Manager";
	}
	Department* returnDepartment() {
		return this->department;
	}
	bool& getNotify() {
		return this->notify;
	}
	int& getDepID() {
		return this->department->getId();
	}
	int& getID() {
		return this->id;
	}
	void login() {

		this->LoggedIn = true;

		while (LoggedIn) {
			this->notify = false;
			for (int i = 0; i < this->department->totalComplains(); i++)
			{
				if(this->department->getComplains()[i]->getStatus()=="Resolved"){
					this->notify = true;
					break;
				}
			}
			enter = 'a';
			system("CLS");
			cout << TextStyle::Bold << Color::Green << "\n\n \t\t You are logged in as : " << this->name << TextStyle::Reset <<
				Color::Reset << endl << endl;
			cout << " 1 ) See your complain,s status! " << endl;
			cout << " 2 ) Assign Employees! " << endl;
			cout << " 3 ) Logout! " << endl;
			if (this->notify) {
				cout <<Color::Yellow<<"\n Press 4 to see notifications and give your feedback ! "<<Color::Reset;
			}
			cin >> primaryFunc; cin.ignore(); // Add this line to clear the input buffer

			switch (primaryFunc) {
			case 1:
				system("CLS");

				cout << " Your Complains are ! " << endl;
				for (int i = 0; i < returnDepartment()->getComplains().size(); i++)
				{
					cout << i + 1 << " : " << returnDepartment()->getComplains()[i]->getName() << " | " <<
						returnDepartment()->getComplains()[i]->getStatus() << endl;
				}
				while (enter != 'Q') {
					cout << "Press 'Q' to go back : ";
					cin >> enter;
				}
				break;

			case 2:

				system("CLS");
				cout << " Select your Complain and then select your employee: \n";
				for (int i = 0; i < returnDepartment()->getComplains().size(); i++)
				{
					cout << i + 1 << " : " << returnDepartment()->getComplains()[i]->getName() << " | " <<
						returnDepartment()->getComplains()[i]->getStatus() << endl;
				}

				cin >> secondaryFunc;
				/*
				if (secondaryFunc <= (returnDepartment()->totalComplains())) {
					returnDepartment()->getComplains()[secondaryFunc - 1]->getStatus() = "Assigned";
				}
				else cout << " Invalid Input";
				*/
				cout << " Select your Employee : \n";
				for (int i = 0; i < returnDepartment()->totalEmployees(); i++)
				{
					cout << i+1 <<" : "<< returnDepartment()->getEmployees()[i]->getName()
						<< "|" << returnDepartment()->getEmployees()[i]->getStatus() << endl;
				}
				cin >> emp1;
				if (emp1 <= (returnDepartment()->totalEmployees())) {
					returnDepartment()->getEmployees()[emp1 - 1]->getAssigned
					(returnDepartment()->getComplains()[secondaryFunc - 1]);
				}
				else cout << " Invalid Input";

				/*
				for (int i = 0; i < returnDepartment()->totalComplains(); i++)
				{

					if (secondaryFunc == i + 1) {
						returnDepartment()->getComplains()[i]->getStatus() = "Assigned";
					}
				}*/


				while (enter != 'Q') {
					cout << "Press 'Q' to go back : ";
					cin >> enter;
				}
				break;

			case 3:
				LoggedIn = false;
				break;
			case 4:
				if (this->notify) {
					for (int i = 0; i < this->department->totalComplains(); i++)
					{
						if ((this->department->getComplains()[i]->getStatus()) == "Resolved")
							cout << this->department->getComplains()[i]->getId() << " " <<
							this->department->getComplains()[i]->getName() << " " <<
							this->department->getComplains()[i]->getStatus() << endl;
					
					}
					cout << " Select your complain id: ";
					cin >> secondaryFunc;

					for (int i = 0; i < complains.size(); i++)
					{
						if (complains[i]->getId() == secondaryFunc) {

							complains[i]->changeFileStatus(complains[i]->getId(), "Done");
							complains[i]->notifyTeacher();
						}
					}


					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;
				}
			default:
				std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
				LoggedIn = false;
				break;
			}
		}
	}

	string& getName() {
		return this->name;
	}
	void print() {
		cout << this->id << " " << this->name << " has dep " << this->department->getName() << endl;
	}
};
vector<Manager*>managers;
class Admin :public Person {
	//vector<Teacher*>teacher;
	//vector<Manager*>manager;
	//vector<Employee*>employee;
public:
	Admin(string name) {
		this->name = name;
		this->role = "Admin";
	}
	void deleteManager(int& id) {

		if (id >= 0 && id < managers.size()) {
			for (int i = 0; i < managers.size(); i++)
			{
				if (managers[i]->getID() == id) {
					delete managers[i]; // Free memory before erasing the pointer
					managers.erase(managers.begin() + i);
					break;
				}
			}
			std::cout << "\Manager at index " << id << " erased." << std::endl;
		}
		else {
			std::cout << "\nIndex out of range." << std::endl;
		}

		std::fstream file("Managers.txt", std::ios::in | std::ios::out);

		if (!file.is_open()) {
			std::cerr << "Error: Unable to open file" << std::endl;
			return;
		}

		std::string line;
		std::vector<std::string> modifiedContent;

		while (std::getline(file, line)) {
			istringstream iss(line);
			int did;
			iss >> did;

			if (did != id) {
				modifiedContent.push_back(line);
			}
		}

		file.close();

		file.open("Managers.txt", std::ios::out | std::ios::trunc);

		for (const auto& modifiedLine : modifiedContent) {
			file << modifiedLine << '\n';
		}

		file.close();
	}
	void deleteEmployee(int& id) {

		if (id >= 0 && id < employees.size()) {
			for (int i = 0; i < employees.size(); i++)
			{
				if (employees[i]->getId() == id) {
					delete employees[i]; // Free memory before erasing the pointer
					employees.erase(employees.begin() + i);
					break;
				}
			}
			std::cout << "\nEmployee at index " << id << " erased." << std::endl;
		}
		else {
			std::cout << "\nIndex out of range." << std::endl;
		}

		std::fstream file("Employees.txt", std::ios::in | std::ios::out);

		if (!file.is_open()) {
			std::cerr << "Error: Unable to open file" << std::endl;
			return;
		}

		std::string line;
		std::vector<std::string> modifiedContent;

		while (std::getline(file, line)) {
			istringstream iss(line);
			int did;
			iss >> did;

			if (did != id) {
				modifiedContent.push_back(line);
			}
		}

		file.close();

		file.open("Employees.txt", std::ios::out | std::ios::trunc);

		for (const auto& modifiedLine : modifiedContent) {
			file << modifiedLine << '\n';
		}

		file.close();
	}
	void deleteTeacher(int& id) {

		if (id >= 0 && id < teachers.size()) {
			for (int i = 0; i < teachers.size(); i++)
			{
				if (teachers[i]->getId() == id) {
					delete teachers[i]; // Free memory before erasing the pointer
					teachers.erase(teachers.begin() + i);
					break;
				}
			}
			std::cout << "\nteachers at index " << id << " erased." << std::endl;
		}
		else {
			std::cout << "\nIndex out of range." << std::endl;
		}

		std::fstream file("Teachers.txt", std::ios::in | std::ios::out);

		if (!file.is_open()) {
			std::cerr << "Error: Unable to open file" << std::endl;
			return;
		}

		std::string line;
		std::vector<std::string> modifiedContent;

		while (std::getline(file, line)) {
			istringstream iss(line);
			int did;
			iss >> did;

			if (did != id) {
				modifiedContent.push_back(line);
			}
		}

		file.close();

		file.open("Teachers.txt", std::ios::out | std::ios::trunc);

		for (const auto& modifiedLine : modifiedContent) {
			file << modifiedLine << '\n';
		}

		file.close();
	}
	void updateTeacher(string name) {
		int size = 0;
		if (teachers.size() == 0) {
			size = 0;
		}
		else {
			size = teachers[teachers.size() - 1]->getId();
			size++;
		}
		this->addTeacher(size, name);
		ofstream outputFile("Teachers.txt");
		if (outputFile.is_open()) {
			for (Teacher* curr : teachers) {
				outputFile << curr->getId() << " " << curr->getname() << endl;
			}
			outputFile.close();
		}
		else {
			cerr << "Error: Unable to open Teachers file for writing" << endl;
		}
	}
	void updateEmployee(string name, Department* department) {
		int size=0;
		if (employees.size() == 0) {
			size = 0;
		}
		else {
			size = employees[employees.size() - 1]->getId();
			size++;
		}
		this->addEmployee(size, name, department);
		ofstream outputFile("Employees.txt");
		if (outputFile.is_open()) {
			for (Employee* curr : employees) {
				outputFile << curr->getId() << " " << curr->getName() << " " << curr->getDepId() << endl;
			}
			outputFile.close();
		}
		else {
			cerr << "Error: Unable to open Employees file for writing" << endl;
		}
	}
	void updateManager(string name, Department* department) {
		int size = 0;
		if (managers.size() == 0) {
			size = 0;
		}
		else {
			size = managers[managers.size() - 1]->getID();
			size++;
		}
		this->addManager(size, name, department);
		ofstream outputFile("Managers.txt");
		if (outputFile.is_open()) {
			for (Manager* curr : managers) {
				outputFile << curr->getID() << " " << curr->getName() << " " << curr->getDepID() << endl;
			}
			outputFile.close();
		}
		else {
			cerr << "Error: Unable to open Employees file for writing" << endl;
		}
	}
	void addManager(int& id, string name, Department* department) {
		managers.push_back(new Manager(id, name, *this, *department));
	}
	void addTeacher(int& id, string name) {
		teachers.push_back(new Teacher(id, name, *this));
	}
	void addEmployee(int& id, string name, Department* deparmtent) {
		employees.push_back(new Employee(id, name, deparmtent, deparmtent->getManager()));
	}
	void login() {
		this->LoggedIn = true;
		while (LoggedIn) {
			enter = 'a';
			system("CLS");
			cout << TextStyle::Bold << Color::Green << "\n\n \t\t You are logged in as : " << this->name << TextStyle::Reset <<
				Color::Reset << endl<<endl;
			cout << " 1 ) Add a User! " << endl;
			cout << " 2 ) Remove a user! " << endl;
			cout << " 3 ) Log out! " << endl;

			cin >> primaryFunc; cin.ignore(); // Add this line to clear the input buffer

			switch (primaryFunc) {
			case 1:
				system("CLS");
				for (int i = 0; i < 3; i++)
				{
					cout <<"\n"<< i + 1 << " : " << " Add a " << primaryUsers[i];
				}
				cout << "\n Enter your option : ";
				cin >> secondaryFunc; cin.ignore();
				switch (secondaryFunc) {

				case 1:
					system("CLS");
					cout << "\n Enter name : ";
					getline(cin, name);
					this->updateTeacher(name);
					cout <<Color::Blue<<" Teacher Added Successfully ! " << Color::Reset<<endl;
					while (enter != 'Q') {
						cout << "\n Press Q to go back : ";
						cin >> enter;
					}
					break;
				case 2:
					system("CLS");
					cout << " Enter name : ";
					getline(cin, name);
					cout << " Select Department : ";
					for (int i = 0; i < departments.size(); i++)
					{
						cout <<"\n"<< i + 1 <<" : "<< departments[i]->getName() << endl;
					}
					cin >> secondaryFunc; cin.ignore();

					this->updateManager(name,departments[secondaryFunc-1]);
					cout << Color::Blue << " Manager Added Successfully ! " << Color::Reset << endl;
					while (enter != 'Q') {
						cout << "\n Press Q to go back : ";
						cin >> enter;
					}
					break;
				case 3:
					system("CLS");
					cout << " Enter name : ";
					getline(cin, name);
					for (int i = 0; i < departments.size(); i++)
					{
						cout <<"\n"<< i + 1 << " : " << departments[i]->getName() << endl;
					}

					cout << " Select Department : ";
					cin >> secondaryFunc; cin.ignore();

					this->updateEmployee(name, departments[secondaryFunc - 1]);
					cout << Color::Blue << "\n Employee Added Successfully ! " << Color::Reset << endl;
					while (enter != 'Q') {
						cout << "\n Press Q to go back : ";
						cin >> enter;
					}
					break;
				default:
					std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
					break;

				}

				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;
			case 2:
				system("CLS");
				for (int i = 0; i < 3; i++)
				{
					cout << i + 1 << " : " << " Remove a " << primaryUsers[i] << endl;
				}
				cout << " Enter your option : ";
				cin >> secondaryFunc; cin.ignore();
				switch (secondaryFunc) {

				case 1:
					system("CLS");
					for (int i = 0; i < teachers.size(); i++)
					{
						cout << teachers[i]->getId() << " . " << teachers[i]->getname() << endl;
					}
					cout << " Select your option : ";
					cin >> secondaryFunc;
					this->deleteTeacher(secondaryFunc);
					cout << Color::Red << " Teacher Deleted " << Color::Reset << endl;
					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;
				case 2:
					system("CLS");
					for (int i = 0; i < managers.size(); i++)
					{
						cout << managers[i]->getID() << " . " << managers[i]->getName() << endl;
					}
					cout << "Select your option : ";
					cin >> secondaryFunc;
					this->deleteManager(secondaryFunc);
					cout << Color::Red << " Manager Deleted " << Color::Reset << endl;
					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;
				case 3:

					system("CLS");
					for (int i = 0; i < employees.size(); i++)
					{
						cout << employees[i]->getId() << " . " << employees[i]->getName() << endl;
					}
					cout << "Select your option : ";
					cin >> secondaryFunc;
				//	secondaryFunc--;
					this->deleteEmployee(secondaryFunc);
					cout << Color::Red << " Employee Deleted " << Color::Reset << endl;
					while (enter != 'Q') {
						cout << "Press Q to go back : ";
						cin >> enter;
					}
					break;
				default:
					std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
					break;

				}

				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;
			case 3:
				this->LoggedIn = false;
				break;

			default:
				std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
				break;

			}
		}
	}

	/*vector<Manager*> getManagers() {
		return manager;
	}
	vector<Teacher*> getTeachers() {
		return teacher;
	}
	vector<Employee*> getEmployees() {
		return employee;
	}*/
	Teacher* returnTeacher(string& name) {
		for (int i = 0; i < teachers.size(); i++)
		{
			if (teachers[i]->getname() == name) {
				return teachers[i];
			}
		}
		cout << " Teacher not found " << endl;
		return 0;
	}
	Manager* returnManager(string& name) {
		for (int i = 0; i < managers.size(); i++)
		{
			if (managers[i]->getName() == name) {
				return managers[i];
			}
		}
		cout << " Manager not found " << endl;
		return 0;
	}
	Employee* returnEmployee(string& name) {
		for (int i = 0; i < employees.size(); i++)
		{
			if (employees[i]->getName() == name) {
				return employees[i];
			}
		}
		cout << " Employee not found " << endl;
		return 0;
	}
	string& getname() {
		return this->name;
	}

	void print() {
		cout << this->name << " is an " << role << endl;
	}
};
class Director :public Person {
	string name;
public:
	Director(string &name) {
		this->name = name;
	}

	void getComplainsByDate() {

	}

	void login() {
		this->LoggedIn = true;
		while (LoggedIn) {
			enter = 'a';
			system("CLS");
			cout << " You are logged in as : " << this->name << endl;
			cout << " 1 ) Get Complains by Date " << endl;
			cout << " 2 ) Watch Summary of Complains ! " << endl;
			cout << " 3 ) Discuss the problem ! " << endl;
			cout << " 4 ) Logout! " << endl;

			cin >> primaryFunc; cin.ignore(); // Add this line to clear the input buffer


			Date* startDate = NULL;
			Date* endDate = NULL;
			switch (primaryFunc) {
			case 1:
				system("CLS");
				cout << " Enter two dates in sequence day/month/year : \n";
				cin >> date1 >> date2;

				startDate = new Date(date1);
				endDate = new Date(date2);
				// Display complaints with dates within the given range
				for (Complain* complains : complains) {
					bool cmp1 = *complains->getDate() > *startDate,cmp2= *complains->getDate() < *endDate;
					if (cmp1&&cmp2) {
						complains->print();
						cout << endl;
					}
				}
            

				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;

			case 2:
				
				system("CLS");
				ar=new int[4];
				for (int i = 0; i < departments.size(); i++)
				{
					ar = departments[i]->getComps();
					cout << departments[i]->getName() << " has " << ar[0] << " Active Complains \n";
					cout << departments[i]->getName()<< " has " << ar[1] << " Assigned Complains \n";
					cout << departments[i]->getName()<< " has " << ar[2] << " Resolved Complains \n";
					cout << departments[i]->getName()<< " has " << ar[3] << " Closed Complains \n";
					cout << "\n\n\n";
				}
				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;
				
			case 3:
				system("CLS");


				for (int i = 0; i < complains.size(); i++)
				{
					complains[i]->print();
				}
				cout << " Enter complain id you want to know about : ";
				cin >> secondaryFunc;


				for (int i = 0; i < complains.size(); i++)
				{
					if (complains[i]->getId() == secondaryFunc) {

						cout << complains[i]->getId() << " : " << complains[i]->getName() << " | " << complains[i]->getDepartment()->getName() << " | " <<
							complains[i]->getDepartment()->getManager()->getName() <<" | ";
						for (int i = 0; i < employees.size(); i++)
						{
							if (employees[i]->getcomp()) {
								if (employees[i]->getcomp()->getId() == secondaryFunc) {
									cout << 
										" | "<<employees[i]->getName();
								}
							}
						}
						cout << " | " << complains[i]->getStatus() << " ";
						complains[i]->getDate()->print();

						cout << endl;
					}
				}


				while (enter != 'Q') {
					cout << "Press Q to go back : ";
					cin >> enter;
				}
				break;
			case 4:
				this->LoggedIn = false;
				break;

			default:
				std::cout << "Invalid choice. Please enter 1 or 2." << std::endl;
				break;

			}
		}
	}
	void print(){}
};
class Job {
	int id;
	vector<Employee*>employee;
	Complain* complain;
	Manager* manager;
	Date* date;
	string status;
public:
	Job(Complain* complain, Employee* employee, Manager* manager) {
		this->status = "Active";
		this->employee.push_back(employee);
		this->manager = manager;
		this->complain = complain;
		this->date = new Date();
	}

	int& getId() {
		return this->id;
	}

	void storeJob(){
		jobs.push_back(this);
		ofstream outputFile("Jobs.txt", std::ios::app);  // Use std::ios::app to append	
		if (outputFile.is_open()) {
			// Append the new complaint to the file
			for (int i = 0; i < this->employee.size(); i++)
			{

				outputFile << jobs.size()+1 << " " << this->complain->getId() << " " << this->manager->getID() <<
					" " << this->employee[i]->getId() <<  endl;
			}

			outputFile.close();
		}
		else {
			cerr << "Error: Unable to open Complains file for writing" << endl;
		}
	}

	void print() {
		for (int i = 0; i < employee.size(); i++)
		{

			cout << this->complain->getName() << "|" << this->employee[i]->getName() << "|" <<
				this->manager->getName() << "|";
			this->date->print();
			cout << "\n";
		}
	}
};


vector <Complain*> Department::getComplains() {
	return this->complains;
}
vector <Employee*> Department::getEmployees() {
	return this->employees;
}
void Teacher::print() {
	cout << this->name << " is added by " << admin->getname() << endl;
};
void Complain::notifyTeacher() {
	this->teacher->getNotify() = true;
}
void Complain::notifyManager() {
	this->department->getManager()->getNotify();
}
void Complain::print() {
	cout << this->id << " " << this->name << " " << this->department->getName() << " " << this->teacher->getname()
		<< this->status << endl;
}
void Employee::makeJob() {
	this->job = new Job(this->complain, this, this->manager);
	this->job->storeJob();
}
void Employee::print(){
		cout << this->id << " " << this->name << " belongs to " << this->department->getName() << "has manager" <<
		this->manager->getName() << endl;
}
void Complain::changeFileStatus(int& id, string newStatus) {

	std::fstream file("Complains.txt", std::ios::in | std::ios::out);
	Date* date = NULL;
	for (Complain* complain : complains) {
		if (complain->getId() == id) {
			complain->getStatus() = newStatus;
			//date = complain->date;
			break;
		}
	}

	std::ofstream ifile("Complains.txt");
	if (ifile.is_open()) {
		for (Complain* complain : complains) {
			ifile << complain->id << "(" << complain->name << ")" <<" "<<
				complain->department->getId() << " " << complain->getTeacher()->getId() << " " << 
				complain->getStatus() << " "
				<< complain->date->getFormattedDate() <<endl;
		}


		file.close();
		ifile.close();

	}
}
int* Department::getComps() {
	int* ar = new int[4];
	for (int i = 0; i < 4; i++)
	{
		ar[i] = 0;
	}
	for (int i = 0; i < totalComplains(); i++)
	{
		if (this->complains[i]->getStatus() == "Active")
			ar[0]+=1;
		else if (this->complains[i]->getStatus() == "Assigned")
			ar[1]++;
		else if (this->complains[i]->getStatus() == "Resolved")
			ar[2]++;
		else if (this->complains[i]->getStatus() == "Closed")
			ar[3]+=1;
	}
	return ar;
}

vector <Department*> Departments() {

	ifstream ifile;
	ifile.open("Departments.txt");
	vector<Department*>departments;
	int departmentId;
	string departmentName;
	// Check if the file is open
	if (ifile.is_open()) {
		while (ifile >> departmentId) {
			// Read the ID
			ifile.ignore(); // Ignore the dot and the space
			getline(ifile, departmentName); // Read the name

			// Create a new Department instance and set its ID and name
			Department* newDepartment = new Department(departmentId, departmentName);

			// Push the new Department instance into the vector
			departments.push_back(newDepartment);
		}
		return departments;
	}
	else {
		// Display an error message if the file cannot be opened
		cerr << "Error: Unable to open Department file " << endl;
	}
}
Admin* fetchAdmin() {
	string AdminName;
	ifstream ifile;
	ifile.open("Admin.txt");

	// Check if the file is open
	if (ifile.is_open()) {
		getline(ifile, AdminName);
		ifile.close();
		Admin* admin = new Admin(AdminName);
		return admin;
	}
	else {
		// Display an error message if the file cannot be opened
		cerr << "Error: Unable to open Admin file " << endl;
	}
}
void initSystem(Admin* admin, vector <Department*>departments) {


	while (!exitSys) {

	//	cout << Color::Red << "This is red text" << Color::Reset << endl;
		system("CLS");
		cout << TextStyle::Bold << 
			 Color::Red << "\n\n\n\n                                     \t \tInvalid choice wont be accepted!\n\n\n"<< Color::Reset << TextStyle::Reset << endl;;
		for (int i = 0; i < roles->size(); i++)
		{
			cout <<
				Color::Green <<" \t\t\t\t\t\t"<< i + 1 << " : Sign in as a " << roles[i] << Color::Reset << endl;
		}
		cout <<
			Color::Red << " \t\t\t\t\t\t" << roles->size() + 1 << " : Exit the system ! " << Color::Reset << endl;
		cout <<
			Color::Blue << "\n\n \t\t\t\t\t\t Enter your choice : "<< Color::Reset;
		cin >> role;
		if (role == (roles->size() + 1)) {
			exitSys = true;
		}
		else if ((role > roles->size() + 1) || role < 1) {
			continue;
		}
		system("CLS");
		/*cout << "Enter your name";
		cin >> s1;
		*/
		s1 = "Anderson";
		Teacher* t1 = admin->returnTeacher(s1);

		s1 = "Shahzaib";
		Manager* m1 = admin->returnManager(s1);

		s1 = "Suraina";
		Employee* e1 = admin->returnEmployee(s1);

		s1 = "Qadir";
		Director* d1 = new Director(s1);

		Person* p1[5] = { admin,t1,m1,e1,d1 };
		for (int i = 0; i < roles->size(); i++)
		{
			if (i == (role - 1)) {
				p1[i]->login();
			}

		}
	}
}

vector<Employee*> seedEmployees(Admin* admin) {
	ifstream inputFile("Employees.txt");

	vector<Employee*> employees1;

	if (inputFile.is_open()) {
		int id, departmentId;
		string name;

		while (inputFile >> id >> name >> departmentId) {
			admin->addEmployee(id, name, departments[departmentId - 1]);
		}
		employees1=employees;
		inputFile.close();
	}
	else {
		cerr << "Error: Unable to Employee file " << endl;
	}
	return employees1;
}
vector<Manager*> seedManagers(Admin* admin) {
	ifstream inputFile("Managers.txt");

	vector<Manager*> manager;

	if (inputFile.is_open()) {
		int id, departmentId;
		string name;

		while (inputFile >> id >> name >> departmentId) {
			admin->addManager(id, name, departments[departmentId - 1]);
		}

		manager = managers;
		inputFile.close();
	}
	else {
		cerr << "Error: Unable to Manager file " << endl;
	}
	return manager;
}
vector<Teacher*> seedTeachers(Admin* admin) {
	ifstream inputFile("Teachers.txt");

	vector<Teacher*> teacher;

	if (inputFile.is_open()) {
		int id;
		string name;

		while (inputFile >> id >> name) {
			admin->addTeacher(id, name);
		}
		teacher = teachers;
		inputFile.close();
		return teacher;
	}
	else {
		cerr << "Error: Unable to Teacher file " << endl;
	}
}
vector<Complain*> seedComplains(Admin* admin) {
	ifstream inputFile("Complains.txt");

	if (!inputFile.is_open()) {
		cerr << "Error: Unable to open Complains file." << endl;
		return {};  // Return an empty vector on error
	}

	vector<Complain*> complains;
	int id, teach, dept;
	string name,status,date1;

	while (inputFile >> id) {
		// Read the space-separated string until the newline character
		 // Read the space-separated string enclosed in single quotes
		if (inputFile.peek() == '(') {
			// Consume the opening single quote
			inputFile.ignore();
			// Read the string until the closing single quote
			getline(inputFile, name, ')');
		}
		else {
			// If not enclosed in single quotes, read the whole word as the name
			inputFile >> name ;
		}

		// Read the remaining fields
		inputFile >> dept >> teach >> status >>date1;
		Date* date = new Date(date1);
		// Find the corresponding Department and Teacher
		Department* _dept = NULL;
		for (Department* department : departments) {
			if (department->getId() == dept) {
				_dept = department;
				break;
			}
		}

		Teacher* _teach = NULL;
		for (Teacher* teacher : teachers) {
			if (teacher->getId() == teach) {
				_teach = teacher;
				break;
			}
		}

		// Check if both Department and Teacher are found
		if (_dept && _teach) {
			// Assuming your Complain constructor takes a space-separated name
			 complains.push_back(new Complain(id, name, _dept, _teach,status,date));
			_teach->seedComps(complains[complains.size()-1]);
		}
		else {
			cerr << "Error: Department or Teacher not found for ID " << id << endl;
		}
	}

	inputFile.close();
	return complains;
}
vector<Job*> seedJobs(Admin* admin) {
	ifstream inputFile("Jobs.txt");

	if (!inputFile.is_open()) {
		cerr << "Error: Unable to open Jobs file." << endl;
		return {};  // Return an empty vector on error
	}

	vector<Job*> job;
	int id, complain, employee, manager;

	while (inputFile >> id >> complain >> manager >> employee ) {
		
		// Find the corresponding Department and Teacher
		Complain*comp = NULL;
		for (Complain* complains : complains) {
			if (complains->getId() == complain) {
				comp = complains;
				break;
			}
		}

		Employee* _emp = NULL;
		for (Employee* employee1 : employees) {
			if (employee1->getId() == employee) {
				_emp = employee1;
				break;
			}
		}

		Manager* _man = NULL;
		for (Manager* man1 : managers) {
			if (man1->getID() == manager) {
				_man = man1;
				break;
			}
		}

		// Check if both Department and Teacher are found
		if (_emp && _man && comp) {
			// Assuming your Complain constructor takes a space-separated name
			job.push_back(new Job(comp, _emp, _man));
			_emp->manuallyAssigned(comp);
		}
		else {
			cerr << "Error: Manager,Employee or Complain error " << id << endl;
		}
	}

	inputFile.close();
	return job;
}
void seedUsers(Admin* admin, vector<Complain*>& complains, vector<Job*>& jobs,
	vector<Teacher*>& teachers, vector<Manager*>& managers, vector<Employee*>& employees, vector<Department*>& departments) {
	departments = Departments();
	managers = seedManagers(admin);
	employees = seedEmployees(admin);
	teachers = seedTeachers(admin);
	complains = seedComplains(admin);
	jobs = seedJobs(admin);
}

int main() {

	Admin* admin = fetchAdmin();
	seedUsers(admin, complains,jobs, teachers, managers, employees, departments);
	initSystem(admin, departments);

	return 0;
}