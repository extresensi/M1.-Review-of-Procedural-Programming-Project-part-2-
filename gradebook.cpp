#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// Global constants
const int NUM_TESTS = 5;         // Number of test scores per student (global constant)
const int MAX_STUDENTS = 100;    // Maximum capacity for partially filled arrays

// Function prototypes
bool readStudentData(const string &filename,
                     string names[],
                     int scores[][NUM_TESTS],
                     int &studentCount);

void calculateAverages(const int scores[][NUM_TESTS],
                       double averages[],
                       int studentCount);

char getLetterGrade(double average);

void printReport(const string names[],
                 const double averages[],
                 const char letterGrades[],
                 int studentCount);

// =============================
// Main
// =============================
int main()
{
    string filename = "StudentGrades.txt"; // input file (should be in same directory)
    string names[MAX_STUDENTS];
    int scores[MAX_STUDENTS][NUM_TESTS];
    double averages[MAX_STUDENTS];
    char letterGrades[MAX_STUDENTS];
    int studentCount = 0;

    cout << "Grade Book Program\n";
    cout << "Reading data from \"" << filename << "\"...\n\n";

    if (!readStudentData(filename, names, scores, studentCount)) {
        cerr << "Failed to read student data. Exiting.\n";
        return 1;
    }

    // Compute averages
    calculateAverages(scores, averages, studentCount);

    // Compute letter grades
    for (int i = 0; i < studentCount; ++i) {
        letterGrades[i] = getLetterGrade(averages[i]);
    }

    // Print formatted report
    printReport(names, averages, letterGrades, studentCount);

    return 0;
}

// =============================
// Function Definitions
// =============================

// Reads student data from filename into parallel arrays.
// Returns true on success. studentCount is set to the number of students read.
// Expected file format (per line):
// Name score1 score2 score3 score4 score5
bool readStudentData(const string &filename,
                     string names[],
                     int scores[][NUM_TESTS],
                     int &studentCount)
{
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Could not open file \"" << filename << "\" for reading.\n";
        return false;
    }

    studentCount = 0;

    while (studentCount < MAX_STUDENTS && infile >> names[studentCount]) {
        // Read NUM_TESTS scores for this student
        bool ok = true;
        for (int t = 0; t < NUM_TESTS; ++t) {
            if (!(infile >> scores[studentCount][t])) {
                ok = false;
                break;
            }
        }

        if (!ok) {
            cerr << "Warning: Incomplete data for student \"" << names[studentCount]
                 << "\". Stopping read.\n";
            break;
        }

        ++studentCount;
    }

    infile.close();

    if (studentCount == 0) {
        cerr << "Warning: No student records found in file.\n";
    }

    return true;
}

// Calculates average score for each student and stores in averages[].
// Assumes studentCount entries exist in scores[].
void calculateAverages(const int scores[][NUM_TESTS],
                       double averages[],
                       int studentCount)
{
    for (int i = 0; i < studentCount; ++i) {
        int sum = 0;
        for (int t = 0; t < NUM_TESTS; ++t) {
            sum += scores[i][t];
        }
        averages[i] = static_cast<double>(sum) / NUM_TESTS;
    }
}

// Converts numeric average to a letter grade using the specified scale.
char getLetterGrade(double average)
{
    if (average >= 90.0) return 'A';
    if (average >= 80.0) return 'B';
    if (average >= 70.0) return 'C';
    if (average >= 60.0) return 'D';
    return 'F';
}

// Prints a neatly formatted report with aligned columns:
// Student Name | Average (2 decimal places) | Letter Grade
void printReport(const string names[],
                 const double averages[],
                 const char letterGrades[],
                 int studentCount)
{
    const int NAME_WIDTH = 20;
    const int AVG_WIDTH = 10;
    const int GRADE_WIDTH = 8;

    cout << left << setw(NAME_WIDTH) << "Student"
         << setw(AVG_WIDTH) << "Average"
         << setw(GRADE_WIDTH) << "Grade" << '\n';

    cout << string(NAME_WIDTH + AVG_WIDTH + GRADE_WIDTH, '-') << '\n';

    cout << fixed << setprecision(2);
    for (int i = 0; i < studentCount; ++i) {
        cout << left << setw(NAME_WIDTH) << names[i]
             << setw(AVG_WIDTH) << averages[i]
             << setw(GRADE_WIDTH) << letterGrades[i]
             << '\n';
    }
    cout << '\n';
}
