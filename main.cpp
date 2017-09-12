#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
using namespace std;
const int INF = 1e9;
struct node {
	int data, y, max, min, sum, size, add,mpc;
	node * left;
	node * right;
	node(int val) {
		data = val;
		y = (rand()<<15)+rand();
		size = 1;
		left = nullptr;
		right = nullptr;
		max = val;
		min = val;
		sum = val;
		add = 0;
		mpc = 1;
	}
	~node() {
		if (left)
			delete left;
		if (right)
			delete right;
	}
};

int get_size(node * root) {
	if (root) {
		return root->size;
	}
	else {
		return 0;
	}
}

bool exists(node*root, int key) {
	if (root == nullptr)
		return false;
	if (key == root->data)
		return true;
	if (exists(root->left, key))
		return true;
	if (exists(root->right, key))
		return true;
	return false;
}

int kth(node * root, int k) {
	int m = 0;
	m= get_size(root->left);
	if (k <= get_size(root->left))
		return kth(root->left, k);
	if (k == get_size(root->left)+1)
		return root->data;
	return kth(root->right, k - get_size(root->left) - 1);
}

int get_max(node * root) {
	if (root)
		return root->add + root->max*root->mpc;
	else
		return -INF;
}

int get_min(node * root) {
	if (root)
		return root->add + root->min*root->mpc;
	else
		return INF;
}

int get_sum(node * root) {
	if (root)
		return root->sum* root->mpc-root->add * root->size;
	else
		return 0;
}

void update(node * root) {
	if (!root)
		return;
	root->size = 1 + get_size(root->left) + get_size(root->right);
	root->max = max(root->data, max(get_max(root->left), get_max(root->right)));
	root->min = min(root->data, min(get_min(root->left), get_min(root->right)));
	root->sum = get_sum(root->left) + get_sum(root->right) + root->data;
}

void push(node * root) {
	if (root->left) {
		root->left->add += root->add;
		root->left->mpc *= root->mpc;
	}
	if (root->right) {
		root->right->add += root->add;
		root->right->mpc *= root->mpc;
	}
	root->data = root->add + root->data*root->mpc;
	root->max = root->add + root->max*root->mpc;
	root->min = root->add + root->min*root->mpc;
	root->sum = root->sum* root->mpc - root->add * root->size;
	root->add = 0;
	root->mpc = 1;
}

pair <node *, node *> split_kth(node * root, int k) {
	if (root == nullptr)
		return{ nullptr,nullptr };
	if (get_size(root) <= k)
		return{ root,nullptr };
	if (k == 0)
		return{ nullptr,root };
	push(root);
	if (k <= get_size(root->left)) {
		auto res = split_kth(root->left, k);
		root->left = res.second;
		update(root);
		return{ res.first,root };
	}
	else {
		auto res = split_kth(root->right, k - 1 - get_size(root->left));
		root->right = res.first;
		update(root);
		return{ root,res.second };
	}
}

node * merge(node*root1, node*root2) {
	if (root1 == nullptr)
		return root2;
	if (root2 == nullptr)
		return root1;
	if (root1->y < root2->y) {
		push(root1);
		root1->right = merge(root1->right, root2);
		update(root1);
		return root1;
	}
	else {
		push(root2);
		root2->left = merge(root1, root2->left);
		update(root2);
		return root2;
	}
}

node * insert(node*root, int pos, int val) {
	auto res = split_kth(root, pos - 1);
	node * newnode = new node(val);
	return merge(merge(res.first, newnode), res.second);
}

node * erase(node * root, int pos) {
	auto res1 = split_kth(root, pos);
	auto res2 = split_kth(res1.first, pos - 1);
	delete res2.second;
	return merge(res2.first, res1.second);
}

void gr_op(char op, int val, node * root, int left, int right) {
	auto  res1 = split_kth(root, right);
	auto  res2 = split_kth(res1.first, left - 1);
	if (op == '+')
		res2.second->add += val;
	if (op == '-')
		res2.second->add -= val;
	if (op == '*') {
		res2.second->mpc *= val;
		res2.second->add *= val;
	}
	merge(merge(res2.first, res2.second), res1.second);
}

int gr_qu(char query, node * root, int left, int right) {
	auto  res1 = split_kth(root, right);
	auto  res2 = split_kth(res1.first, left - 1);
	if (query == 'n') {
		int ans= res2.second->min;
		merge(merge(res2.first, res2.second), res1.second);
		return ans;
	}
	if (query == 'x') {
		int ans = res2.second->max;
		merge(merge(res2.first, res2.second), res1.second);
		return ans;
	}
	if (query == 's') {
		int ans = res2.second->sum;
		merge(merge(res2.first, res2.second), res1.second);
		return ans;
	}
}

int depth(node * root) {
	if (root == nullptr)
		return 0;
	return 1 + max(depth(root->left), depth(root->right));
}

void print(node * root) {
	if (root == nullptr)
		return;
	push(root);
	print(root->left);
	cout << root->add + root->data*root->mpc << " ";
	print(root->right);
}

int main() {

	//srand(time(0));
	int k;
	node * root= new node(1);
	for (int i = 1; i <= 10; i++) {
		insert(root, i, i);
	}
	print(root);
	cout << endl;
//	cout << "hight = " << depth(root)<< endl;
//	cout << "size = " << root->size << endl;
//	cout << "input k" << endl;
//	cin >> k;
//	cout << "k-th = " << kth(root, k) << endl;
//	cout << "erase k-th" << endl;
//	erase(root, k);
//	print(root);
//	cout << endl;
//	gr_op('+', 1, root, 1, 11);
	gr_op('*', 2, root, 1, 11);
	gr_op('+', 1, root, 1, 11);
	print(root);
	cout << endl;
//	cout << gr_qu('x', root, 3, 7) << " ";
//	cout << gr_qu('n', root, 3, 7) << " ";
	cout << gr_qu('s', root, 3, 7) << " ";
}