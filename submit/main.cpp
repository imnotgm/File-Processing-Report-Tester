#include <iostream>
#include <stdlib.h>
#include <stack>

// Node structure definition
typedef struct node
{
    int key;
    int height;
    int size;
    struct node *left;
    struct node *right;
}Node;

// Function to allocate memory to a new node and returns pointer of it
Node *getNode()
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->height = 1;
    newNode->size = 1;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// Function to free allocated memory of a node
void freeNode(Node *node)
{
    // exception check
    if(node != NULL)
        free(node);
}

// Function to return height of the tree T
int height(Node *T)
{
    if(T == NULL)
        return 0;
    return T->height;
}

int size(Node *T)
{
    if(T == NULL)
        return 0;
    return T->size;
}

// Function to print key of nodes in the tree T, traversing in order
void inorder(Node *T)
{
    if(T == NULL)
        return;
    printf("<");
    inorder(T->left);
    printf(" %d ", T->key);
    inorder(T->right);
    printf(">");
}

// Function to clear nodes of the tree T
void clear(Node **T)
{
    if((*T) == NULL)
        return;
    clear(&((*T)->left));
    clear(&((*T)->right));
    free(*T);
}

bool insertBST(Node **T, int newKey) //T = root node
{
    Node *p = *T, *q = NULL;
    std::stack<Node *> s;

    // find position to insert newKey while storing parent node on stack
    while(p != NULL)
    {
        // New key already exists in the tree
        if(newKey == p->key)
            return false;
        // Node q is parent of p
        q = p;
        // Push parent 'q' onto stack so that its height can be updated
        s.push(q);
        if(newKey < p->key)
            p = p->left;
        else
            p = p->right;
    }
    // create new node
    Node *newNode = getNode();
    newNode->key = newKey;

    // New node is a root of the tree
    if(*T == NULL)
        *T = newNode;

    // Attatch newNode as a child of node 'q'
    else if(newKey < q->key)
        q->left = newNode;
    else
        q->right = newNode;

    // Update height and size of nodes from (deleted node's) parent to root
    while(!s.empty())
    {
        q = s.top();
        s.pop();
        int lh = height(q->left);
        int rh = height(q->right);
        q->height = 1 + (lh > rh ? lh : rh);

        int ls = size(q->left);
        int rs = size(q->right);
        q->size = 1 + ls + rs;
    }
    return true;
}

bool deleteBST(Node **T, int deleteKey) //T = root node
{
    Node *p = *T, *q = NULL;
    std::stack<Node *> s;
    
    // find position of deleteKey while storing parent node on stack
    while(p != NULL && deleteKey != p->key)
    {
        q = p;
        s.push(q);

        if(deleteKey < p->key)
            p = p->left;
        else
            p = p->right;
    }
    // deleteKey not found
    if(p == NULL)
        return false;

    // case where degree of p is 2
    if(p->left != NULL && p->right != NULL)
    {
        Node *tmp = p;
        s.push(p);

        // Find min key from nodes in the right subtree
        if(p->left->height < p->right->height || (p->left->height == p->right->height && p->left->size < p->right->size))
        {
            p = p->right;
            while(p->left != NULL)
            {
                s.push(p);
                p = p->left;
            }
        }
        // Find max key from nodes in the left subtree
        else
        {
            p = p->left;
            while(p->right != NULL)
            {
                s.push(p);
                p = p->right;
            }
        }
        tmp->key = p->key;
        q = s.top();
        s.pop();
    }

    // case where degree of p is 0
    if(p->left == NULL && (p->right == NULL))
    {
        // delete root
        if(q == NULL)
            *T = NULL;
        // Node 'p' has no subtree => no nodes to attatch to 'q'
        else if(q->left == p)
            q->left = NULL;
        else
            q->right = NULL;
    }
    // case where degree of p is 1
    else
    {
        // The node to be deleted has only a left-subtree
        if(p->left != NULL)
        {
            // The node is root
            if(q == NULL)
                *T = (*T)->left;
            // The node is a left child
            else if(q->left == p)
                q->left = p->left;
            // Th node is a right child
            else
                q->right = p->left;
        }
        // The node to be deleted has only a right-subtree
        else
        {
            // Node is root
            if(q == NULL)
                *T = (*T)->right;
            // Node is a left child
            else if(q->left == p)
                q->left = p->right;
            // Node is a right child
            else
                q->right = p->right;
        }
    }
    freeNode(p);

    // Update height and size of nodes from (deleted node's) parent to root
    while(!s.empty())
    {
        q = s.top();
        s.pop();
        int lh = height(q->left);
        int rh = height(q->right);
        q->height = 1 + (lh > rh ? lh : rh);

        int ls = size(q->left);
        int rs = size(q->right);
        q->size = 1 + ls + rs;
    }
    return true;
}

int main()
{
    Node *root = NULL;

    char command;
    int key;

	while (std::cin >> command >> key) {
		switch ((int)command) {
			case (int)'i':
				if (!insertBST(&root, key)) {
					std::cerr << "i " << key << ": The key already exists" << std::endl;
					continue;
				}
				break;
			case (int)'d':
				if (!deleteBST(&root, key)) {
					std::cerr << "d " << key << ": The key does not exist" << std::endl;
					continue;
				}
				break;
			default:
				std::cerr << "Invalid command: " << command << std::endl;
				return (1);
				break;
		}
        inorder(root);
        std::cout << std::endl;
	}

	// 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
	clear(&root);
}