#include <bits/stdc++.h>
#include "BTree.h"
using namespace std;

#define DEGREE 5

void takeInput(BTree *&clg, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error opening file" << endl;
        exit(EXIT_FAILURE);
    }

    // Skip the header line
    string line;
    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);
        Details student;

        string yearStr;
        string feeStr;
        string appStr;

        // Read data from the line
        getline(ss, student.name, ',');
        getline(ss, student.dep, ',');
        getline(ss, yearStr, ',');
        getline(ss, student.rollno, ',');

        // Convert year from string to enum class
        int yearInt = stoi(yearStr);
        student.year = static_cast<Year>(yearInt - 1);

        for (int i = 0; i < 5; ++i)
        {
            string value;
            getline(ss, value, ',');
            student.classesConducted[i] = stoi(value);
        }

        for (int i = 0; i < 5; ++i)
        {
            string value;
            getline(ss, value, ',');
            student.classesAttended[i] = stoi(value);
        }

        getline(ss, feeStr, ',');
        getline(ss, appStr, ',');

        // Convert fee and applicant from string to enum class
        student.fee = static_cast<FeeStatus>(stoi(feeStr));
        student.app = static_cast<Applicant>(stoi(appStr));

        clg->insert(student); // Insert into BTree
    }

    file.close();
}
int main()
{
    BTree *clg = new BTree(DEGREE);
    string filename = "sample.csv";
    takeInput(clg, filename);

    bool exit = false;
    while (!exit)
    {
        int choice;

        cout << "Menu:" << endl;
        cout << "1. Traverse entire B-tree" << endl;
        cout << "2. Search for a student by roll number" << endl;
        cout << "3. Range search" << endl;
        cout << "4. Delete a student" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            clg->traverse();
            break;
        }
        case 2:
        {

            cout << "Enter roll number of student you want to search: ";
            string rollno;
            cin >> rollno;
            Details student;
            student.rollno = rollno;
            BTreeNode *node = clg->search(student);
            if (node)
            {
                // Iterate through keys and print details of the found student
                for (int i = 0; i < node->keyCount; ++i)
                {
                    if (node->keys[i] == student)
                    {
                        node->keys[i].printDetails();
                        break;
                    }
                }
            }
            else
            {
                cout << "No such student exists" << endl;
            }
            break;
        }
        case 3:
        {
            // Example range search based on year
            cout << "Enter start and end year (1 to 4): ";
            int startYear, endYear;
            cin >> startYear >> endYear;
            Year start = static_cast<Year>(startYear - 1);
            Year end = static_cast<Year>(endYear - 1);

            // Perform the range search
            clg->rangeSearch(start, end); // You need to implement this method in BTree
        }
        break;
        case 4:
        {

            string rollno;
            cout << "Enter roll number of student to delete: ";
            cin >> rollno;
            Details student;
            student.rollno = rollno;
            clg->remove(student);
            break;
        }
        case 9:
        {

            exit = true;
            break;
        }
        default:
            cout << "Incorrect Input, Try again" << endl;
        }
    }
    clg->traverse();
    delete clg;
    return 0;
}