#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstdio>
using namespace std;

struct AVL {
	int key, height;
	AVL *left, *right;
};

struct Stack {
	AVL *tree;
	Stack *next;
};

struct IntStack {
	int data;
	IntStack *next;
};

struct Queue {
	AVL *tree;
	Queue *next;
};

struct DefineQueue {
	Queue *front = nullptr, *rear = nullptr;
};

int max(int a, int b) {

	return a > b ? a : b;

}

AVL* NewNode(int key) {
	AVL *temp = new AVL;
	temp->key = key;
	temp->height = 1;
	temp->left = temp->right = nullptr;
	return temp;
}

void Push(Stack *&stack, AVL *tree) {
	
	Stack *temp = new Stack;
	temp->tree = tree;
	temp->next = stack;
	stack = temp;

}

AVL* Pop(Stack *&stack) {

	Stack *temp = stack;
	AVL *tree = stack->tree;
	stack = stack->next;
	delete temp;
	return tree;

}

AVL* Peek(Stack *&stack) {

	return stack->tree;

}

bool Empty(Stack *&stack) {

	return stack == nullptr;

}

void IntPush(IntStack *&intStack, int data) {

	IntStack *temp = new IntStack;
	temp->data = data;
	temp->next = intStack;
	intStack = temp;

}

int IntPop(IntStack *&intStack) {

	IntStack *temp = intStack;
	int data = intStack->data;
	intStack = intStack->next;
	delete temp;
	return data;

}

int IntPeek(IntStack *&intStack) {

	return intStack->data;

}

bool IntEmpty(IntStack *&intStack) {

	return intStack == nullptr;

}

void Insert(DefineQueue &queue, AVL *tree) {

	Queue *temp = new Queue;
	temp->tree = tree;
	temp->next = nullptr;
	if (!queue.rear) queue.front = temp;
	else queue.rear->next = temp;
	queue.rear = temp;

}

AVL* Delete(DefineQueue &queue) {

	Queue *temp = queue.front;
	AVL *tree = queue.front->tree;
	queue.front = queue.front->next;
	if (!queue.front) queue.rear = nullptr;
	delete temp;
	return tree;

}

AVL* Front(DefineQueue &queue) {

	return queue.front->tree;

}

bool EmptyQ(DefineQueue &queue) {

	return queue.front == nullptr;

}

int GetHeight(AVL *node) {

	if (!node) return 0;
	else return node->height;

}

int GetBalance(AVL *node) {

	if (!node) return 0;
	else return GetHeight(node->left) - GetHeight(node->right);

}

AVL* RightRotate(AVL *x) {

	AVL *y = x->left;
	x->left = y->right;
	y->right = x;

	x->height = max(GetHeight(x->left), GetHeight(x->right)) + 1;
	y->height = max(GetHeight(y->left), GetHeight(y->right)) + 1;

	return y;

}

AVL* LeftRotate(AVL *x) {

	AVL *y = x->right;
	x->right = y->left;
	y->left = x;

	x->height = max(GetHeight(x->left), GetHeight(x->right)) + 1;
	y->height = max(GetHeight(y->left), GetHeight(y->right)) + 1;

	return y;

}

void AVLMenu() {}

int AVLFindKey(AVL *&root, int key, bool output = 1) {

	AVL *node = root;
	int numSteps = 0;
	DefineQueue queue;
	bool found = false;

	numSteps++;
	while (node) {
		Insert(queue, node);

		if (node->key == key) break;
		else if (node->key > key) node = node->left;
		else node = node->right;

		numSteps++;
	}
	
	if (!node) {
		if (output) cout << "Ne postoji cvor sa kljucem zadate vrednosti.\n";
		while (!EmptyQ(queue)) Delete(queue);
	}
	else {
		if (output) {
			cout << "Pronadjen je cvor sa kljucem zadate vrednosti.\nPut do njega je: ";
			while (!EmptyQ(queue)) {
				node = Delete(queue);
				cout << node->key;
				if (EmptyQ(queue)) cout << "\n";
				else cout << " -> ";
			}
		}
		else while (!EmptyQ(queue)) Delete(queue);
	}

	return numSteps;
}

void AVLInsert(AVL *&root, int key) {

	if (!root) {
		root = NewNode(key);
		return;
	}

	AVL *node = root, *prev = nullptr;
	Stack *stack = nullptr;
	while (node) {
		Push(stack, node);
		if (key <= node->key) node = node->left;
		else node = node->right;
	}
	node = NewNode(key);
	prev = node;

	int balance;
	while (!Empty(stack)) {
		node = Pop(stack);

		if (node->key >= prev->key) node->left = prev;
		else node->right = prev;

		node->height = max(GetHeight(node->left), GetHeight(node->right)) + 1;
		balance = GetBalance(node);

		if (balance > 1 && node->left->key >= key) node = RightRotate(node);
		else if (balance > 1 && node->left->key < key) {
			node->left = LeftRotate(node->left);
			node = RightRotate(node);
		}
		else if (balance < -1 && node->right->key >= key) {
			node->right = RightRotate(node->right);
			node = LeftRotate(node);
		}
		else if (balance < -1 && node->right->key < key) node = LeftRotate(node);

		prev = node;
	}

	root = prev;

}

void AVLDelete(AVL *&root) {

	AVL *node = root;
	DefineQueue queue;
	Insert(queue, node);
	while (!EmptyQ(queue)) {
		node = Delete(queue);
		if (node->left) Insert(queue, node->left);
		if (node->right) Insert(queue, node->right);
		delete node;
	}

}

void KeyPrint(int key, int num) {

	for (int i = 0;i < 7 * num;i++) cout << "-";
	cout << " " << key << "\n";

}

void AVLPrint(AVL *&root) {

	AVL *node = nullptr;
	int num;
	Stack *stack = nullptr;
	IntStack *intStack = nullptr;

	Push(stack, root);
	IntPush(intStack, root->height);
	while (!Empty(stack)) {
		node = Pop(stack);
		num = IntPop(intStack);
		while (node) {
			KeyPrint(node->key, num);
			num--;
			if (node->right) {
				Push(stack, node->right);
				IntPush(intStack, num);
			}
			node = node->left;
		}
	}

}

int main() {

	AVL *root = nullptr;
	AVLInsert(root, 12);
	AVLInsert(root, 10);
	AVLInsert(root, 11);
	AVLInsert(root, 10);
	AVLInsert(root, 20);
	AVLInsert(root, 24);
	AVLInsert(root, 15);
	AVLInsert(root, 14);
	cout << AVLFindKey(root, 12) << endl;
	cout << AVLFindKey(root, 10) << endl;
	cout << AVLFindKey(root, 16) << endl;
	cout << AVLFindKey(root, 24) << endl;
	AVLPrint(root);
	return 0;

}