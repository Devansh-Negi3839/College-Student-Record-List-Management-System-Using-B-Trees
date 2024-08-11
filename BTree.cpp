#include "BTree.h"

using namespace std;

BTree::BTree(int _degree)
{
    root = NULL;
    degree = _degree;
}

void BTree::traverse()
{
    if (root != NULL)
        root->traverse();
}

BTreeNode *BTree::search(Details k)
{
    return (root == NULL) ? NULL : root->search(k);
}

// Function to insert a new key in the B-tree
void BTree::insert(Details k)
{
    // If the tree is empty, create a new root
    if (root == NULL)
    {
        root = new BTreeNode(degree, true);
        root->keys[0] = k;  // Insert the first key
        root->keyCount = 1; // Update number of keys in root
    }
    else
    {
        // If the root node is full, then tree grows in height
        if (root->keyCount == 2 * degree - 1)
        {
            BTreeNode *s = new BTreeNode(degree, false);

            // Make the old root a child of the new root
            s->children[0] = root;

            // Split the old root and move a key to the new root
            s->splitChild(0, root);

            // Determine which of the two children will have the new key
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->children[i]->insertNonFull(k);

            // Change root
            root = s;
        }
        else
        {
            // If the root is not full, call insertNonFull for root
            root->insertNonFull(k);
        }
    }
}

// Function to remove a key from the B-tree
void BTree::remove(Details k)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }

    // Call the remove function on the root
    root->remove(k);

    // If the root node has 0 keys, make its first child the new root
    if (root->keyCount == 0)
    {
        BTreeNode *tmp = root;
        if (root->isLeaf)
            root = NULL;
        else
            root = root->children[0];

        // Free the old root
        delete tmp;
    }
}

// Constructor for BTreeNode class
BTreeNode::BTreeNode(int _degree, bool _isLeaf)
{
    degree = _degree;
    isLeaf = _isLeaf;

    // Allocate memory for the keys and child pointers
    keys = new Details[2 * degree - 1];
    children = new BTreeNode *[2 * degree];

    keyCount = 0; // Initialize the number of keys to 0
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{
    int i;
    // Traverse through the keys and corresponding subtrees
    for (i = 0; i < keyCount; i++)
    {
        if (isLeaf == false)
            children[i]->traverse(); // Traverse the child subtree
        keys[i].printDetails();      // Print the current key
    }

    // Print the subtree rooted with the last child
    if (isLeaf == false)
        children[i]->traverse();
}

// Function to search for a key in the subtree rooted with this node
BTreeNode *BTreeNode::search(Details k)
{
    int i = 0;
    // Find the first key greater than or equal to k
    while (i < keyCount && keys[i] < k)
        i++;

    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return this;

    // If the key is not found and this is a leaf node, return NULL
    if (isLeaf == true)
        return NULL;

    // Go to the appropriate child
    return children[i]->search(k);
}

// Function to insert a key in the subtree rooted with this node
// The node must be non-full when this function is called
void BTreeNode::insertNonFull(Details k)
{
    int i = keyCount - 1;

    // If this is a leaf node
    if (isLeaf == true)
    {
        // Find the location of the new key to be inserted
        // Move all greater keys one position ahead
        while (i >= 0 && k < keys[i])
        {
            keys[i + 1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i + 1] = k;
        keyCount = keyCount + 1;
    }
    else
    { // If this node is not a leaf
        // Find the child which is going to have the new key
        while (i >= 0 && k < keys[i])
            i--;

        // See if the found child is full
        if (children[i + 1]->keyCount == 2 * degree - 1)
        {
            // If the child is full, then split it
            splitChild(i + 1, children[i + 1]);

            // After splitting, the middle key of children[i+1] goes up and children[i+1] is split into two
            if (keys[i + 1] < k)
                i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

// Function to split the child y of this node at index i
void BTreeNode::splitChild(int i, BTreeNode *y)
{
    // Create a new node to store degree-1 keys of y
    BTreeNode *z = new BTreeNode(y->degree, y->isLeaf);
    z->keyCount = degree - 1;

    // Copy the last (degree-1) keys of y to z
    for (int j = 0; j < degree - 1; j++)
        z->keys[j] = y->keys[j + degree];

    // Copy the last degree children of y to z
    if (y->isLeaf == false)
    {
        for (int j = 0; j < degree; j++)
            z->children[j] = y->children[j + degree];
    }

    // Reduce the number of keys in y
    y->keyCount = degree - 1;

    // Create space for the new child
    for (int j = keyCount; j >= i + 1; j--)
        children[j + 1] = children[j];

    // Link the new child to this node
    children[i + 1] = z;

    // Move a key from y to this node
    for (int j = keyCount - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[degree - 1];

    // Increment count of keys in this node
    keyCount = keyCount + 1;
}

// Function to remove the key k from the subtree rooted with this node
void BTreeNode::remove(Details k)
{
    int idx = findKey(k);

    // The key to be removed is present in this node
    if (idx < keyCount && keys[idx] == k)
    {
        if (isLeaf)
            removeFromLeaf(idx); // Remove from leaf node
        else
            removeFromNonLeaf(idx); // Remove from non-leaf node
    }
    else
    {
        // If the key is not present in this node and this is a leaf node
        if (isLeaf)
        {
            cout << "The student with roll no. -" << k.rollno << " does not exist in the database\n";
            return;
        }

        // Flag to indicate if the key is present in the last child
        bool flag = ((idx == keyCount) ? true : false);

        // If the child where the key may exist has less than the minimum number of keys, fill that child
        if (children[idx]->keyCount < degree)
            fill(idx);

        // If the last child has been merged, the key must be present in the previous child
        if (flag && idx > keyCount)
            children[idx - 1]->remove(k);
        else
            children[idx]->remove(k);
    }
}

// Function to find the index of the first key that is greater or equal to k
int BTreeNode::findKey(Details k)
{
    int idx = 0;
    while (idx < keyCount && keys[idx] < k)
        ++idx;
    return idx;
}

// Function to remove the key present in idx-th position in this node which is a leaf
void BTreeNode::removeFromLeaf(int idx)
{
    // Shift all the keys after the idx-th position one step backward
    for (int i = idx + 1; i < keyCount; ++i)
        keys[i - 1] = keys[i];

    // Reduce the count of keys
    keyCount--;
}

// Function to remove the key present in idx-th position in this node which is a non-leaf node
void BTreeNode::removeFromNonLeaf(int idx)
{
    Details k = keys[idx];

    // If the child that precedes k (children[idx]) has at least degree keys
    if (children[idx]->keyCount >= degree)
    {
        Details pred = getPred(idx);     // Get the predecessor
        keys[idx] = pred;            // Replace k by predecessor
        children[idx]->remove(pred); // Recursively delete the predecessor
    }
    // If the child children[idx] has fewer than degree keys, examine children[idx+1]
    else if (children[idx + 1]->keyCount >= degree)
    {
        Details succ = getSucc(idx);         // Get the successor
        keys[idx] = succ;                // Replace k by successor
        children[idx + 1]->remove(succ); // Recursively delete the successor
    }
    // If both children[idx] and children[idx+1] have fewer than degree keys, merge k and children[idx+1] into children[idx]
    else
    {
        merge(idx);
        children[idx]->remove(k);
    }
}

// Function to get the predecessor of the key at index idx
Details BTreeNode::getPred(int idx)
{
    BTreeNode *cur = children[idx];
    while (!cur->isLeaf)
        cur = cur->children[cur->keyCount];
    return cur->keys[cur->keyCount - 1];
}

// Function to get the successor of the key at index idx
Details BTreeNode::getSucc(int idx)
{
    BTreeNode *cur = children[idx + 1];
    while (!cur->isLeaf)
        cur = cur->children[0];
    return cur->keys[0];
}

// Function to fill the child node at index idx if it has less than the minimum number of keys
void BTreeNode::fill(int idx)
{
    if (idx != 0 && children[idx - 1]->keyCount >= degree)
        borrowFromPrev(idx); // Borrow a key from the previous sibling
    else if (idx != keyCount && children[idx + 1]->keyCount >= degree)
        borrowFromNext(idx); // Borrow a key from the next sibling
    else
    {
        if (idx != keyCount)
            merge(idx); // Merge the child with its next sibling
        else
            merge(idx - 1); // Merge with its previous sibling
    }
}

// Function to borrow a key from children[idx-1] and insert it into children[idx]
void BTreeNode::borrowFromPrev(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx - 1];

    // The last key from children[idx-1] goes up to the parent and key[idx-1] from parent is inserted as the first key in children[idx]
    for (int i = child->keyCount - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    // Moving sibling's child as child's first child
    if (!child->isLeaf)
    {
        for (int i = child->keyCount; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = keys[idx - 1];

    // Move sibling's last child to child's first position
    if (!child->isLeaf)
        child->children[0] = sibling->children[sibling->keyCount];

    keys[idx - 1] = sibling->keys[sibling->keyCount - 1];

    child->keyCount += 1;
    sibling->keyCount -= 1;
}

// Function to borrow a key from children[idx+1] and place it in children[idx]
void BTreeNode::borrowFromNext(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];

    // keys[idx] is inserted as the last key in children[idx]
    child->keys[(child->keyCount)] = keys[idx];

    // Sibling's first child is inserted as the last child of children[idx]
    if (!(child->isLeaf))
        child->children[(child->keyCount) + 1] = sibling->children[0];

    keys[idx] = sibling->keys[0];

    // Shift all keys in sibling one step left
    for (int i = 1; i < sibling->keyCount; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    // Shift the child pointers one step left
    if (!sibling->isLeaf)
    {
        for (int i = 1; i <= sibling->keyCount; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->keyCount += 1;
    sibling->keyCount -= 1;
}

// Function to merge children[idx] and children[idx+1] with key[idx] in the middle
void BTreeNode::merge(int idx)
{
    BTreeNode *child = children[idx];
    BTreeNode *sibling = children[idx + 1];

    // keys[idx] is inserted as the last key in children[idx]
    child->keys[degree - 1] = keys[idx];

    // keys of sibling is inserted after children[idx]
    for (int i = 0; i < sibling->keyCount; ++i)
        child->keys[i + degree] = sibling->keys[i];

    // Child pointers of sibling are also inserted
    if (!child->isLeaf)
    {
        for (int i = 0; i <= sibling->keyCount; ++i)
            child->children[i + degree] = sibling->children[i];
    }

    // Moving all keys after idx in the parent one step before to fill the gap created by moving keys[idx] to children[idx]
    for (int i = idx + 1; i < keyCount; ++i)
        keys[i - 1] = keys[i];

    // Moving the child pointers after (idx+1) in the parent one step before
    for (int i = idx + 2; i <= keyCount; ++i)
        children[i - 1] = children[i];

    // Updating the key count of child and parent
    child->keyCount += sibling->keyCount + 1;
    keyCount--;

    // Free the memory occupied by sibling
    delete sibling;
}

// Helper function to recursively search within the range
void BTreeNode::rangeSearch(Year start, Year end)
{
    int i = 0;
    while (i < keyCount && keys[i].year < start) 
        ++i; // Skip keys before start year
    
    // Traverse all children
    for (; i < keyCount && keys[i].year <= end; ++i)
    {
        // Print details of current key
        keys[i].printDetails();

        // If not a leaf, recurse into the children
        if (!isLeaf)
            children[i]->rangeSearch(start, end);
    }

    // Finally, check the last child if this node is not a leaf
    if (!isLeaf)
        children[i]->rangeSearch(start, end);
}

// Implement the rangeSearch method in BTree
void BTree::rangeSearch(Year start, Year end)
{
    if (root != nullptr)
        root->rangeSearch(start, end);
}