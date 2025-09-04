#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

using namespace std;


struct Course {
    string courseName;
    string courseCode;
    int creditHours;
    double marks;
    string grade;
    double gradePoints;
};


struct Student {
    string name;
    string rollNumber;
    int semester;
    vector<Course> courses;
    double gpa;
    double cgpa;
    int totalCreditHours;
    double totalGradePoints;
};

class CGPACalculator {
private:
    vector<Student> students;
    string filename = "student_records.txt";

   
    void calculateGradeAndPoints(Course& course) {
        if (course.marks >= 85) {
            course.grade = "A+";
            course.gradePoints = 4.0;
        } else if (course.marks >= 80) {
            course.grade = "A";
            course.gradePoints = 3.7;
        } else if (course.marks >= 75) {
            course.grade = "A-";
            course.gradePoints = 3.3;
        } else if (course.marks >= 70) {
            course.grade = "B+";
            course.gradePoints = 3.0;
        } else if (course.marks >= 65) {
            course.grade = "B";
            course.gradePoints = 2.7;
        } else if (course.marks >= 60) {
            course.grade = "B-";
            course.gradePoints = 2.3;
        } else if (course.marks >= 55) {
            course.grade = "C+";
            course.gradePoints = 2.0;
        } else if (course.marks >= 50) {
            course.grade = "C";
            course.gradePoints = 1.7;
        } else if (course.marks >= 45) {
            course.grade = "D";
            course.gradePoints = 1.0;
        } else {
            course.grade = "F";
            course.gradePoints = 0.0;
        }
    }

   
    void calculateGPA(Student& student) {
        double totalWeightedPoints = 0.0;
        int totalCredits = 0;

        for (const auto& course : student.courses) {
            totalWeightedPoints += course.gradePoints * course.creditHours;
            totalCredits += course.creditHours;
        }

        student.totalCreditHours = totalCredits;
        student.totalGradePoints = totalWeightedPoints;
        student.gpa = (totalCredits > 0) ? (totalWeightedPoints / totalCredits) : 0.0;
    }

 
    void calculateCGPA(Student& student) {
       
        student.cgpa = student.gpa;
    }

public:
   
    void displayMenu() {
        cout << "\n========================================" << endl;
        cout << "      CGPA CALCULATOR SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << "1. Add New Student Record" << endl;
        cout << "2. Display Student Records" << endl;
        cout << "3. Search Student by Roll Number" << endl;
        cout << "4. Calculate GPA/CGPA" << endl;
        cout << "5. Display All Students' CGPA" << endl;
        cout << "6. Save Records to File" << endl;
        cout << "7. Load Records from File" << endl;
        cout << "8. Delete Student Record" << endl;
        cout << "9. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
    }

   
    void addStudent() {
        Student newStudent;
        int numCourses;

        cout << "\n--- Add New Student ---" << endl;
        cout << "Enter Student Name: ";
        cin.ignore();
        getline(cin, newStudent.name);

        cout << "Enter Roll Number: ";
        getline(cin, newStudent.rollNumber);

        cout << "Enter Semester: ";
        cin >> newStudent.semester;

        cout << "Enter Number of Courses: ";
        cin >> numCourses;

        for (int i = 0; i < numCourses; i++) {
            Course course;
            cout << "\n--- Course " << (i + 1) << " ---" << endl;

            cout << "Enter Course Name: ";
            cin.ignore();
            getline(cin, course.courseName);

            cout << "Enter Course Code: ";
            getline(cin, course.courseCode);

            cout << "Enter Credit Hours: ";
            cin >> course.creditHours;

            cout << "Enter Marks (0-100): ";
            cin >> course.marks;

           
            while (course.marks < 0 || course.marks > 100) {
                cout << "Invalid marks! Please enter marks between 0-100: ";
                cin >> course.marks;
            }

            calculateGradeAndPoints(course);
            newStudent.courses.push_back(course);
        }

        calculateGPA(newStudent);
        calculateCGPA(newStudent);
        students.push_back(newStudent);

        cout << "\nStudent record added successfully!" << endl;
        cout << "GPA: " << fixed << setprecision(2) << newStudent.gpa << endl;
        cout << "CGPA: " << fixed << setprecision(2) << newStudent.cgpa << endl;
    }

   
    void displayAllStudents() {
        if (students.empty()) {
            cout << "\nNo student records found!" << endl;
            return;
        }

        cout << "\n========================================" << endl;
        cout << "         STUDENT RECORDS" << endl;
        cout << "========================================" << endl;

        for (size_t i = 0; i < students.size(); i++) {
            displayStudentDetails(students[i]);
            cout << "----------------------------------------" << endl;
        }
    }

   
    void displayStudentDetails(const Student& student) {
        cout << "\nName: " << student.name << endl;
        cout << "Roll Number: " << student.rollNumber << endl;
        cout << "Semester: " << student.semester << endl;
        cout << "\nCourse Details:" << endl;
        cout << left << setw(15) << "Course Code" 
             << setw(25) << "Course Name" 
             << setw(8) << "Credits" 
             << setw(8) << "Marks" 
             << setw(8) << "Grade" 
             << setw(12) << "Grade Points" << endl;
        cout << string(80, '-') << endl;

        for (const auto& course : student.courses) {
            cout << left << setw(15) << course.courseCode
                 << setw(25) << course.courseName
                 << setw(8) << course.creditHours
                 << setw(8) << fixed << setprecision(1) << course.marks
                 << setw(8) << course.grade
                 << setw(12) << fixed << setprecision(2) << course.gradePoints << endl;
        }

        cout << "\nTotal Credit Hours: " << student.totalCreditHours << endl;
        cout << "Total Grade Points: " << fixed << setprecision(2) << student.totalGradePoints << endl;
        cout << "GPA: " << fixed << setprecision(2) << student.gpa << endl;
        cout << "CGPA: " << fixed << setprecision(2) << student.cgpa << endl;
    }

    
    void searchStudent() {
        if (students.empty()) {
            cout << "\nNo student records found!" << endl;
            return;
        }

        string rollNo;
        cout << "\nEnter Roll Number to search: ";
        cin.ignore();
        getline(cin, rollNo);

        bool found = false;
        for (const auto& student : students) {
            if (student.rollNumber == rollNo) {
                cout << "\n--- Student Found ---" << endl;
                displayStudentDetails(student);
                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\nStudent with Roll Number '" << rollNo << "' not found!" << endl;
        }
    }

   
    void calculateStudentGPA() {
        if (students.empty()) {
            cout << "\nNo student records found!" << endl;
            return;
        }

        string rollNo;
        cout << "\nEnter Roll Number: ";
        cin.ignore();
        getline(cin, rollNo);

        for (auto& student : students) {
            if (student.rollNumber == rollNo) {
                calculateGPA(student);
                calculateCGPA(student);

                cout << "\n--- GPA/CGPA Calculation Results ---" << endl;
                cout << "Student: " << student.name << endl;
                cout << "Roll Number: " << student.rollNumber << endl;
                cout << "Total Courses: " << student.courses.size() << endl;
                cout << "Total Credit Hours: " << student.totalCreditHours << endl;
                cout << "Total Grade Points: " << fixed << setprecision(2) << student.totalGradePoints << endl;
                cout << "GPA: " << fixed << setprecision(2) << student.gpa << endl;
                cout << "CGPA: " << fixed << setprecision(2) << student.cgpa << endl;

                
                if (student.cgpa >= 3.7) {
                    cout << "Classification: Excellent (A)" << endl;
                } else if (student.cgpa >= 3.0) {
                    cout << "Classification: Good (B)" << endl;
                } else if (student.cgpa >= 2.0) {
                    cout << "Classification: Average (C)" << endl;
                } else if (student.cgpa >= 1.0) {
                    cout << "Classification: Below Average (D)" << endl;
                } else {
                    cout << "Classification: Fail (F)" << endl;
                }
                return;
            }
        }

        cout << "\nStudent with Roll Number '" << rollNo << "' not found!" << endl;
    }

    
    void displayAllCGPA() {
        if (students.empty()) {
            cout << "\nNo student records found!" << endl;
            return;
        }

       
        vector<Student> sortedStudents = students;
        sort(sortedStudents.begin(), sortedStudents.end(), 
             [](const Student& a, const Student& b) {
                 return a.cgpa > b.cgpa;
             });

        cout << "\n========================================" << endl;
        cout << "      CGPA RANKING LIST" << endl;
        cout << "========================================" << endl;
        cout << left << setw(5) << "Rank" 
             << setw(20) << "Name" 
             << setw(15) << "Roll Number" 
             << setw(10) << "CGPA" 
             << setw(15) << "Grade" << endl;
        cout << string(65, '-') << endl;

        for (size_t i = 0; i < sortedStudents.size(); i++) {
            string grade;
            if (sortedStudents[i].cgpa >= 3.7) grade = "A";
            else if (sortedStudents[i].cgpa >= 3.0) grade = "B";
            else if (sortedStudents[i].cgpa >= 2.0) grade = "C";
            else if (sortedStudents[i].cgpa >= 1.0) grade = "D";
            else grade = "F";

            cout << left << setw(5) << (i + 1)
                 << setw(20) << sortedStudents[i].name
                 << setw(15) << sortedStudents[i].rollNumber
                 << setw(10) << fixed << setprecision(2) << sortedStudents[i].cgpa
                 << setw(15) << grade << endl;
        }
    }

    
    void saveToFile() {
        ofstream file(filename);
        if (!file) {
            cout << "\nError: Unable to create file!" << endl;
            return;
        }

        file << students.size() << endl;
        for (const auto& student : students) {
            file << student.name << "|" << student.rollNumber << "|" 
                 << student.semester << "|" << student.courses.size() << endl;

            for (const auto& course : student.courses) {
                file << course.courseName << "|" << course.courseCode << "|"
                     << course.creditHours << "|" << course.marks << "|"
                     << course.grade << "|" << course.gradePoints << endl;
            }

            file << student.gpa << "|" << student.cgpa << "|"
                 << student.totalCreditHours << "|" << student.totalGradePoints << endl;
        }

        file.close();
        cout << "\nRecords saved to file successfully!" << endl;
    }

    
    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            cout << "\nNo existing file found. Starting with empty records." << endl;
            return;
        }

        students.clear();
        int numStudents;
        file >> numStudents;
        file.ignore();

        for (int i = 0; i < numStudents; i++) {
            Student student;
            string line;
            getline(file, line);

            // Parse student basic info
            size_t pos = 0;
            pos = line.find('|');
            student.name = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            student.rollNumber = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find('|');
            student.semester = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            int numCourses = stoi(line);

           
            for (int j = 0; j < numCourses; j++) {
                Course course;
                getline(file, line);

                pos = line.find('|');
                course.courseName = line.substr(0, pos);
                line.erase(0, pos + 1);

                pos = line.find('|');
                course.courseCode = line.substr(0, pos);
                line.erase(0, pos + 1);

                pos = line.find('|');
                course.creditHours = stoi(line.substr(0, pos));
                line.erase(0, pos + 1);

                pos = line.find('|');
                course.marks = stod(line.substr(0, pos));
                line.erase(0, pos + 1);

                pos = line.find('|');
                course.grade = line.substr(0, pos);
                line.erase(0, pos + 1);

                course.gradePoints = stod(line);
                student.courses.push_back(course);
            }

           
            getline(file, line);
            pos = line.find('|');
            student.gpa = stod(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find('|');
            student.cgpa = stod(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find('|');
            student.totalCreditHours = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            student.totalGradePoints = stod(line);
            students.push_back(student);
        }

        file.close();
        cout << "\nRecords loaded from file successfully!" << endl;
        cout << "Total records loaded: " << students.size() << endl;
    }

    
    void deleteStudent() {
        if (students.empty()) {
            cout << "\nNo student records found!" << endl;
            return;
        }

        string rollNo;
        cout << "\nEnter Roll Number to delete: ";
        cin.ignore();
        getline(cin, rollNo);

        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->rollNumber == rollNo) {
                cout << "\nStudent '" << it->name << "' (Roll: " << rollNo << ") deleted successfully!" << endl;
                students.erase(it);
                return;
            }
        }

        cout << "\nStudent with Roll Number '" << rollNo << "' not found!" << endl;
    }

   
    void run() {
        int choice;

  p
        loadFromFile();

        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
                case 1:
                    addStudent();
                    break;
                case 2:
                    displayAllStudents();
                    break;
                case 3:
                    searchStudent();
                    break;
                case 4:
                    calculateStudentGPA();
                    break;
                case 5:
                    displayAllCGPA();
                    break;
                case 6:
                    saveToFile();
                    break;
                case 7:
                    loadFromFile();
                    break;
                case 8:
                    deleteStudent();
                    break;
                case 9:
                    cout << "\nSaving records and exiting..." << endl;
                    saveToFile();
                    cout << "Thank you for using CGPA Calculator!" << endl;
                    break;
                default:
                    cout << "\nInvalid choice! Please try again." << endl;
                    break;
            }

            if (choice != 9) {
                cout << "\nPress Enter to continue...";
                cin.ignore();
                cin.get();
                system("clear"); 
            }

        } while (choice != 9);
    }
};


int main() {
    CGPACalculator calculator;
    calculator.run();
    return 0;
}
