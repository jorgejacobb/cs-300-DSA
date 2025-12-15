#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <limits>

using namespace std;

// =======================
// DSA: Analysis and Design - Project 2
// Student: Jorge Fernando Moreno Jacob
// December 8, 2025
// =======================

struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& c) : course(c), left(nullptr), right(nullptr) {}
};

class CourseBST {
private:
    Node* root;

    void insertNode(Node*& node, const Course& course) {
        // If tree/subtree is empty, this course becomes the root of that subtree
        if (node == nullptr) {
            node = new Node(course);
            return;
        }

        // Compare by course number (e.g., "CSCI100")
        if (course.number < node->course.number) {
            insertNode(node->left, course);
        } else {
            insertNode(node->right, course);
        }
    }

    void inOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);
        cout << node->course.number << ", " << node->course.title << endl;
        inOrder(node->right);
    }

    Node* searchNode(Node* node, const string& courseNumber) const {
        if (node == nullptr) {
            return nullptr;
        }

        if (courseNumber == node->course.number) {
            return node;
        } else if (courseNumber < node->course.number) {
            return searchNode(node->left, courseNumber);
        } else {
            return searchNode(node->right, courseNumber);
        }
    }

    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    CourseBST() : root(nullptr) {}

    ~CourseBST() {
        destroy(root);
    }

    void insert(const Course& course) {
        insertNode(root, course);
    }

    void printInOrder() const {
        inOrder(root);
    }

    Course* search(const string& courseNumber) const {
        Node* found = searchNode(root, courseNumber);
        if (found == nullptr) {
            return nullptr;
        }
        return &found->course;
    }
};

// =======================
// Helper functions
// =======================

string toUpperStr(const string& input) {
    string result = input;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return static_cast<char>(toupper(c)); });
    return result;
}

void loadCourses(const string& filename, CourseBST& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string token;

        Course course;

        // course number
        if (!getline(ss, token, ',')) {
            continue; // malformed line
        }
        course.number = toUpperStr(token);

        // course title
        if (!getline(ss, token, ',')) {
            continue; // malformed line
        }
        course.title = token;

        // remaining tokens are prerequisites (if any)
        while (getline(ss, token, ',')) {
            if (!token.empty()) {
                course.prerequisites.push_back(toUpperStr(token));
            }
        }

        bst.insert(course);
    }

    file.close();
    cout << "Data loaded from " << filename << endl;
}

// Print details for a single course
void printCourseInfo(const Course& course) {
    cout << course.number << ", " << course.title << endl;

    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
    } else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// =======================
// Main program
// =======================

int main() {
    CourseBST bst;
    bool dataLoaded = false;

    // Default filename (same name as the file you have)
    string filename = "CS 300 ABCU_Advising_Program_Input.csv";

    cout << "Welcome to the course planner." << endl << endl;

    int choice = 0;
    while (choice != 9) {
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        if (!(cin >> choice)) {
            // Handle non-numeric input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline
        cout << endl;

        switch (choice) {
        case 1: {
            cout << "Enter the file name (or press Enter for default: " << filename << "): ";
            {
                string input;
                getline(cin, input);
                if (!input.empty()) {
                    filename = input;
                }
            }

            loadCourses(filename, bst);
            dataLoaded = true;
            cout << endl;
            break;
        }

        case 2:
            if (!dataLoaded) {
                cout << "Please load the data structure first (option 1)." << endl << endl;
            } else {
                cout << "Here is a sample schedule:" << endl << endl;
                bst.printInOrder();
                cout << endl;
            }
            break;

        case 3:
            if (!dataLoaded) {
                cout << "Please load the data structure first (option 1)." << endl << endl;
            } else {
                cout << "What course do you want to know about? ";
                {
                    string courseNumber;
                    getline(cin, courseNumber);
                    courseNumber = toUpperStr(courseNumber);

                    cout << endl;

                    Course* course = bst.search(courseNumber);
                    if (course == nullptr) {
                        cout << courseNumber << " not found." << endl << endl;
                    } else {
                        printCourseInfo(*course);
                        cout << endl;
                    }
                }
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl << endl;
            break;
        }
    }

    return 0;
}
