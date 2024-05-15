
      //STUDENT RECORD MANAGEMENT SYSTEM USING FILE I/O..

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

//DEFINING CLASS AND INCORPORATING LINKED LIST...
class Student {
public:
    int roll_no, phone_no;
    string name, fname, course, quali;
    Student* next;

    Student(int roll_no, string name, string fname, string course, string quali, int phone_no) {
        this->roll_no = roll_no;
        this->name = name;
        this->fname = fname;
        this->course = course;
        this->quali = quali;
        this->phone_no = phone_no;
        this->next = nullptr;
    }

//FUNCTION TO DISPLAY STUDENT RECORD..
    void display() {
        cout << "Student ID: " << roll_no << endl;
        cout << "Name: " << name << endl;
        cout << "Father's Name: " << fname << endl;
        cout << "Course: " << course << endl;
        cout << "Qualification: " << quali << endl;
        cout << "Phone Number: " << phone_no << endl;
    }
};

//SECOND CLASS FOR DEFINING ALL FUNCTIONS FOR OPERATIONS..
class StudentRecord {
public:
    Student* head;

    StudentRecord() {
        head = nullptr;
        readFromFile();
    }

    ~StudentRecord() {
        writeToFile();
        clearList();
    }

//FUNCTION TO ADD STUDENT RECORD..
    void addStudent() {
        int roll_no, phone_no;
        string name, fname, course, quali;

        cout << "Enter Student ID: ";
        cin >> roll_no;
        cout << "Enter Student Name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        cout << "Enter Father's Name: ";
        getline(cin, fname);
        cout << "Enter Course: ";
        getline(cin, course);
        cout << "Enter Qualification: ";
        getline(cin, quali);
        cout << "Enter Phone Number: ";
        cin >> phone_no;

        Student* new_student = new Student(roll_no, name, fname, course, quali, phone_no);

        if (head == nullptr) {
            head = new_student;
        } else {
            Student* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_student;
        }

        cout << "\nStudent record added successfully.\n";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input stream
    }

//FUNCTION TO SEARCH THE GIVEN STUDENT RECORD..
    void searchStudent() {
        if (head == nullptr) {
            cout << "\nStudent record is empty.\n";
            return;
        }

        int roll_no;
        cout << "Enter Student ID to search: ";
        cin >> roll_no;

        Student* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->roll_no == roll_no) {
                cout << "\nStudent Found:\n";
                temp->display();
                found = true;
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "\nStudent with ID " << roll_no << " not found.\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input stream
    }

//COUNTING THE NUMBER OF STUDENTS IN THE RECORD..
    void countStudents() {
        int count = 0;
        Student* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        cout << "\nTotal number of students: " << count << endl;
    }

//UPDATING THE RECORD OF THE GIVEN STUDENT..
    void updateStudent() {
        if (head == nullptr) {
            cout << "\nStudent record is empty.\n";
            return;
        }

        int roll_no;
        cout << "Enter Student ID to update: ";
        cin >> roll_no;

        Student* temp = head;
        bool found = false;
        while (temp != nullptr) {
            if (temp->roll_no == roll_no) {
                cout << "\nEnter new details for the student:\n";
                cout << "Enter Student Name: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, temp->name);
                cout << "Enter Father's Name: ";
                getline(cin, temp->fname);
                cout << "Enter Course: ";
                getline(cin, temp->course);
                cout << "Enter Qualification: ";
                getline(cin, temp->quali);
                cout << "Enter Phone Number: ";
                cin >> temp->phone_no;
                cout << "\nStudent record updated successfully.\n";
                found = true;
                break;
            }
            temp = temp->next;
        }

        if (!found) {
            cout << "\nStudent with ID " << roll_no << " not found.\n";
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input stream
    }

//DELETING THE RECORD OF GIVEN STUDENT..
    void deleteStudent() {
        if (head == nullptr) {
            cout << "\nStudent record is empty.\n";
            return;
        }

        int roll_no;
        cout << "Enter Student ID to delete: ";
        cin >> roll_no;

        if (head->roll_no == roll_no) {
            Student* temp = head;
            head = head->next;
            delete temp;
            cout << "\nStudent record deleted successfully.\n";
            return;
        }

        Student* prev = head;
        Student* temp = head->next;
        while (temp != nullptr) {
            if (temp->roll_no == roll_no) {
                prev->next = temp->next;
                delete temp;
                cout << "\nStudent record deleted successfully.\n";
                return;
            }
            prev = temp;
            temp = temp->next;
        }

        cout << "\nStudent with ID " << roll_no << " not found.\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input stream
    }

    void clearList() {
        Student* temp;
        while (head != nullptr) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

//WRITING IN THE FILE..
    void writeToFile() {
        ofstream file("student_records.txt");
        if (!file.is_open()) {
            cout << "Error opening file for writing.\n";
            return;
        }

        Student* temp = head;
        while (temp != nullptr) {
            file << temp->roll_no << endl;
            file << temp->name << endl;
            file << temp->fname << endl;
            file << temp->course << endl;
            file << temp->quali << endl;
            file << temp->phone_no << endl;
            temp = temp->next;
        }

        file.close();
        cout << "\nStudent records written to file successfully.\n";
    }

//READING FROM THE FILE..
    void readFromFile() {
        ifstream file("student_records.txt");
        if (!file.is_open()) {
            cout << "Error opening file for reading.\n";
            return;
        }

        clearList(); // Clear existing list before reading from file

        int roll_no, phone_no;
        string name, fname, course, quali;
        while (file >> roll_no >> ws && getline(file, name) && getline(file, fname) &&
               getline(file, course) && getline(file, quali) && file >> phone_no) {
            Student* new_student = new Student(roll_no, name, fname, course, quali, phone_no);
            if (head == nullptr) {
                head = new_student;
            } else {
                Student* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = new_student;
            }
        }

        file.close();
        cout << "\nStudent records read from file successfully.\n";
    }
};

int main() {
    StudentRecord record;
    int choice;
    //LOOP TO ENTER THE CHOICE FOR PERFORMING THE SPECIFIC TASK..
    while (true) {
        cout << "\nStudent Record Management System\n";
        cout << "Choose 1 to Add student record\n";
        cout << "Choose 2 to Search student record\n";
        cout << "Choose 3 to Count student records\n";
        cout << "Choose 4 to Update student record\n";
        cout << "Choose 5 to Delete student record\n";
        cout << "Choose 6 to Exit\n";
        cout << "\nEnter your choice: \t";
        cin >> choice;

        switch (choice) {
            case 1:
                record.addStudent();
                break;
            case 2:
                record.searchStudent();
                break;
            case 3:
                record.countStudents();
                break;
            case 4:
                record.updateStudent();
                break;
            case 5:
                record.deleteStudent();
                break;
            case 6:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}
