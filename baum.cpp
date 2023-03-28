#include <iostream>
#include <fstream>
#include <cmath>
#include <list>

using namespace std;

struct Node // lets create the tree structure, we need Nodes left and right
{
    int key;
    Node *left;
    Node *right;
};
void deleteTree(Node *node);
void insert(Node *&root, int key);
int height(Node *node);
int is_AVL(Node *root);
void check_AVL(Node *root);
int min_key(Node *root);
int max_key(Node *root);
int sum_keys(Node *root);
double avg_key(Node *root, int num_nodes);
bool isSubtree(Node *t1, Node *t2);

// argc is the number of arguments sent to the programm in execution
//*argv[] -> here we have the argument sent to the program
int main(int argc, char *argv[])
{
    // if the number of arguments is not 2, then:
    if (argc < 2 || argc > 3)
    {
        // display error with explanation on how to use the program
        cerr << "Usage: " << endl << argv[0] << " <list_path> to check if the list builds an AVL-Tree"<< endl << "or " << argv[0] << " <child_path> <parent_path> to check if child is in the father tree" << endl;
        return 1;
    }
    // we start our tree
    Node *root = NULL;
    // list<int> tree;
    int num_nodes = 0;
    // we start in argv[1] to send our parameter to ifstream and open the file
    // in argv[0] we have first argument, in our case baum.exe
    ifstream input_file(argv[1]);
    if (!input_file.is_open())
    {
        cerr << "Could not open input file." << endl; // capture error if file could not be opened
        return 1;
    }

    int key;                  // we store the values that we read
    while (input_file >> key) // we continue while until we can't read anymore from our file
    {
        insert(root, key); // we insert the our generated keys into the tree
        num_nodes++;       // we keep count of the number of nodes
    }
    input_file.close();

    if (root == NULL) // if root until here is NULL, then it is empty
    {
        cout << "The binary search tree is empty." << endl;
        return 0;
    }
    if (argc == 2)
    {
        check_AVL(root); // now we check the positions

        int min = min_key(root);
        int max = max_key(root);
        double avg = avg_key(root, num_nodes);

        cout << "min: " << min << ", max: " << max << ", avg: " << avg;

        deleteTree(root); // lets delete the tree

        return 0;
    }
    if (argc == 3)
    {
        // we start our tree
        Node *root2 = NULL;
        // list<int> tree;
        int num_nodes_2 = 0;
        // we start in argv[1] to send our parameter to ifstream and open the file
        // in argv[0] we have first argument, in our case baum.exe
        ifstream input_file(argv[2]);
        if (!input_file.is_open())
        {
            cerr << "Could not open input file." << endl; // capture error if file could not be opened
            return 1;
        }

        int key;                  // we store the values that we read
        while (input_file >> key) // we continue while until we can't read anymore from our file
        {
            insert(root2, key); // we insert the our generated keys into the tree
            num_nodes_2++;     // we keep count of the number of nodes
        }
        input_file.close();

        if (isSubtree(root, root2))
        {
            cout << "The second tree is a subtree of the first one";
        }
        else
        {
            cout << "The second tree is not a subtree of the first one";
        }

        return 0;
    }
}

void deleteTree(Node *node)
{
    if (node == NULL)
    {
        return;
    } // recursive function to delete the tree, at the end (AKA when node is NULL) we just return.

    deleteTree(node->left);
    deleteTree(node->right);

    delete node; // we delete current node
    node = NULL; // set at the end to NULL, apparently is good practice
}

// Function to insert a key into the binary search tree
void insert(Node *&root, int key)
{
    if (root == NULL) // create first Node
    {
        root = new Node;
        root->key = key;
        root->left = NULL;
        root->right = NULL;
    }
    else
    {
        if (key < root->key) // when key is lower, we insert left
        {
            insert(root->left, key);
        }
        else if (key > root->key)
        {
            insert(root->right, key); // when key is higher, we insert right
        }
    }
}

// Function to calculate the height of a node in the AVL tree
int height(Node *node)
{
    if (node == NULL)
    {
        return -1; // tree is empty
    }
    else
    {
        // we call this function recursive left and right and calculate the height of the tree from the node that was given
        int left_height = height(node->left);
        int right_height = height(node->right);
        return 1 + max(left_height, right_height);
    }
}
//still needs to comment it!
bool isSubtree(Node* t1, Node* t2) {
    // If t1 is empty, it is a subtree of t2
    if (t1 == NULL) {
        return true;
    }

    // If t2 is empty, t1 is not a subtree of t2
    if (t2 == NULL) {
        return false;
    }

    // If the keys of the nodes match, check if the left and right subtrees of t1 are present in t2
    if (t1->key == t2->key) {
        return isSubtree(t1->left, t2->left) && isSubtree(t1->right, t2->right);
    }

    // Check if t1 is a subtree of t2's left or right subtree
    return isSubtree(t1, t2->left) || isSubtree(t1, t2->right);
}


int is_AVL(Node *root)
{
    if (root == NULL)
    {
        return 0;
    }
    // calculate height left and right
    int left_height = height(root->left);
    int right_height = height(root->right);
    // calculate the balance factor to check if the AVL condition is met
    int bf = right_height - left_height;

    // With this we can print the height of nodes, not needed but helps with overview and understanding

    /* if (root->left != NULL){
        cout << "The height of " << root->left->key << " is " << left_height <<endl;
    }
    if (root->right != NULL){
        cout << "The height of " << root->right->key << " is " << right_height <<endl;
    } */

    // print the information about the node
    cout << "bal(" << root->key << ") = " << bf << " ";

    if (bf > 1 || bf < -1)
    { // this part depending on the balance factor
        cout << "(AVL violation!)" << endl;
    }
    else
    {
        cout << endl;
    }

    // then we call recursive this function again with left and right
    int is_left_AVL = is_AVL(root->left);
    int is_right_AVL = is_AVL(root->right);
    // if any violation of the AVL tree is detected we return -1 to check_AVL to print the right output
    if (is_left_AVL == -1 || is_right_AVL == -1 || abs(left_height - right_height) > 1)
    {
        return -1;
    }
    // if we arrived here, then we will just return 1 and print that it is indeed an AVL tree
    return 1;
}

// here we start everything... at the end we print if it is AVL or not
void check_AVL(Node *root)
{
    int is_avl = is_AVL(root); // all other information printing we do here depending on the nodes analyzed

    if (is_avl == -1)
    {
        cout << "AVL: no" << endl;
    }
    else if (is_avl == 1)
    {
        cout << "AVL: yes" << endl;
    }
}

// using the logic of the tree, we just go all the way to the left to find the minimum key
int min_key(Node *root)
{
    if (root == NULL)
    {
        return -1;
    }
    else if (root->left == NULL) // if nothing else left, then we return the value
    {
        return root->key;
    }
    else
    {
        return min_key(root->left); // else call this function again to go further left
    }
}
// same exact logic as in minimum key, this time all the way to the right

int max_key(Node *root)
{
    if (root == NULL)
    {
        return -1;
    }
    else if (root->right == NULL)
    {
        return root->key;
    }
    else
    {
        return max_key(root->right);
    }
}

// Sume operation of all the nodes
int sum_keys(Node *root)
{
    if (root == NULL)
    {
        return 0; // if we reach the end, then just sum 0
    }
    else
    {
        // recursive call and summ all results until we reach the end
        return root->key + sum_keys(root->left) + sum_keys(root->right);
    }
}

// Function to calculate the average key in the binary search tree
double avg_key(Node *root, int num_nodes)
{
    int sum = sum_keys(root);
    // use of static_cast to change sum from int to double, so we can deliver an exact result
    return static_cast<double>(sum) / num_nodes;
}
