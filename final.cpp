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

    clg->traverse();

    return 0;
}