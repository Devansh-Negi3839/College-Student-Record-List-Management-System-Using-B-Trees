#ifndef BTREE_H
#define BTREE_H

#include <bits/stdc++.h>
using namespace std;

#define NAME_SIZE 20
#define ROLL_SIZE 15
#define SUBJECT_SIZE 5

enum class Year
{
    First,
    Second,
    Third,
    Fourth
};
enum class FeeStatus
{
    Pending,
    Clear
};
enum class Applicant
{
    NotApplied,
    Applied
};

class Details
{
public:
    string name;
    string dep;
    Year year;
    string rollno;
    vector<int> classesConducted;
    vector<int> classesAttended;
    FeeStatus fee;
    Applicant app;

    Details() : classesConducted(5, 0), classesAttended(5, 0) {}
    // operator overloading to compare 2 Details objects
    bool operator<(const Details &other) const
    {
        // Compare year in decreasing order
        if (this->year != other.year)
            return year > other.year;

        // Compare department in increasing order
        if (this->dep != other.dep)
            return dep < other.dep;

        // Compare roll number in increasing order
        return this->rollno < other.rollno;
    }
    bool operator==(const Details &other) const
    {
        return rollno == other.rollno;
    }

    void printDetails() const
    {
        // Helper function to convert enum to string
        auto yearToString = [](Year y)
        {
            switch (y)
            {
            case Year::First:
                return "First";
            case Year::Second:
                return "Second";
            case Year::Third:
                return "Third";
            case Year::Fourth:
                return "Fourth";
            default:
                return "Unknown";
            }
        };
        auto feeToString = [](FeeStatus f)
        {
            return f == FeeStatus::Pending ? "Pending" : "Clear";
        };
        auto applicantToString = [](Applicant a)
        {
            return a == Applicant::NotApplied ? "Not Applied" : "Applied";
        };

        cout << name << " | " << dep << " | " << yearToString(year) << " | " << rollno << " | "
             << feeToString(fee) << " | " << applicantToString(app) << " | " << "Classes Conducted: ";
        for (auto i = 0; i < classesConducted.size(); ++i)
            cout << classesConducted[i] << (i < classesConducted.size() - 1 ? ", " : "");
        cout << " | Classes Attended: ";
        for (auto i = 0; i < classesAttended.size(); ++i)
            cout << classesAttended[i] << (i < classesAttended.size() - 1 ? ", " : "");
        cout << endl;
    }
};

// Node of a B-tree
class BTreeNode
{
    Details *keys;        // Array to store keys
    int degree;           // Minimum degree (defines the range for number of keys)
    BTreeNode **children; // Array of child pointers
    int keyCount;         // Current number of keys
    bool isLeaf;          // True if the node is a leaf node

public:
    BTreeNode(int _degree, bool _isLeaf); // Constructor
    void insertNonFull(Details k);        // Inserts a new key when the node is not full
    void splitChild(int i, BTreeNode *y); // Splits the child of a node at index i
    void traverse();                      // Traverses all nodes in a subtree rooted with this node
    BTreeNode *search(Details k);         // Searches for a key in the subtree rooted with this node
    int findKey(Details k);               // Finds the index of the first key that is greater or equal to k
    void remove(Details k);               // Removes the key from the subtree rooted with this node
    void removeFromLeaf(int idx);         // Removes the key at index idx in a leaf node
    void removeFromNonLeaf(int idx);      // Removes the key at index idx in a non-leaf node
    Details getPred(int idx);             // Gets the predecessor of the key at index idx
    Details getSucc(int idx);             // Gets the successor of the key at index idx
    void fill(int idx);                   // Fills the child node at index idx if it has less than the minimum number of keys
    void borrowFromPrev(int idx);         // Borrows a key from the previous sibling
    void borrowFromNext(int idx);         // Borrows a key from the next sibling
    void merge(int idx);                  // Merges the child node at index idx with its sibling
    friend class BTree;                   // Granting access to private members for BTree class
};

// B-tree class
class BTree
{
    BTreeNode *root; // Pointer to the root node
    int degree;      // Minimum degree

public:
    BTree(int _degree);
    void traverse();
    BTreeNode *search(Details k);
    void insert(Details k); // Inserts a new key in the B-tree
    void remove(Details k); // Removes a key from the B-tree
};

#endif