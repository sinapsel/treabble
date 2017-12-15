#ifndef TREAP_H
#define TREAP_H
#include <iostream>
using namespace std;

namespace service {
typedef long big;
const big INF = 1e9;
template <class T1, class T2>
struct pair {
  T1 first;
  T2 second;
  pair(const T1 &f, const T2 &s) {
    this->first = f;
    this->second = s;
  }
  pair() : first(0), second(0) {}
  template <class U, class V>
  pair(const pair<U, V> &pr) {
    (*this) = *pr;
  }
};

template <typename T>
T max(T a, T b) {
  return (a > b) ? a : b;
}
template <typename T>
T min(T a, T b) {
  return (a < b) ? a : b;
}
}

#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

template <class T>
class treap {
  //#define cnt(t) ((t)?(t)->c:0)
  //    #define update(t) if(t)(t)->c=1+cnt(t->l)+cnt(t->r)
  struct node {
    T val;
    size_t key, c;

    T max, min, sum, size, add, mpc;
    bool rev;

    node *left, *right;
    static size_t cnt(node *t) { return ((t) ? (t)->c : 0); }
    node() {
      left = right = nullptr;
      c = 0;
      rev = false;
      key = rand() << 15 | rand();
      add = 0;
      mpc = 1;
    }
    node(T V, node *L, node *R) : val(V) {
      key = rand() << 15 | rand();
      left = L;
      right = R;
      c = 1 + cnt(left) + cnt(right);
      rev = false;
      add = 0;
      mpc = 1;
    }
  };

  node *root;
  int n;

  void push(node *&root) {
    if (root && root->rev) {
      root->rev = false;
      node *q = root->left;
      root->left = root->right;
      root->right = q;
      if (root->left) {
        root->left->rev ^= true;
        root->left->add += root->add;
        root->left->mpc *= root->mpc;
      }
      if (root->right) {
        root->right->rev ^= true;
        root->right->add += root->add;
        root->right->mpc *= root->mpc;
      }
      root->val = root->add + root->val * root->mpc;
      root->max = root->add + root->max * root->mpc;
      root->min = root->add + root->min * root->mpc;
      root->sum = root->sum * root->mpc - root->add * root->size;
      root->add = 0;
      root->mpc = 1;
    }
  }

  int size(node *root) {
    if (root) {
      return root->size;
    } else {
      return 0;
    }
  }

  void update(node *root) {
    if (!root) return;
    root->c = 1 + node::cnt(root->left) + node::cnt(root->right);
    root->size = 1 + size(root->left) + size(root->right);
    root->max =
        service::max(root->val, max(get_max(root->left), get_max(root->right)));
    root->min =
        service::min(root->val, min(get_min(root->left), get_min(root->right)));
    root->sum = get_sum(root->left) + get_sum(root->right) + root->val;
  }

  void merge(node *&t, node *left, node *right) {
    push(left);
    push(right);
    if (!left)
      t = right;
    else if (!right)
      t = left;
    else if (left->key > right->key) {
      merge(left->right, left->right, right);
      t = left;
    } else {
      merge(right->left, left, right->left);
      t = right;
    }
    update(t);
  }

  void split(node *t, int sk, node *&left, node *&right, int less = 0) {
    if (!t)
      left = right = nullptr;
    else {
      push(t);
      int key = node::cnt(t->left) + less;
      if (sk <= key) {
        split(t->left, sk, left, t->left, less);
        right = t;
      } else {
        split(t->right, sk, t->right, right, less + node::cnt(t->left) + 1);
        left = t;
      }
      update(t);
    }
  }

  void print(node *&t) {
    if (!t) return;
    push(t);
    print(t->left);
    cout << t->val << ' ';
    print(t->right);
  }

  bool isexist(node *root, T key) {
    if (!root) return false;
    if (key == root->val) return true;
    if (isexist(root->left, key)) return true;
    if (isexist(root->right, key)) return true;
    return false;
  }

  T *find(node *root, T elem) {
    if (root == nullptr) return nullptr;
    if (elem == root->val)
      return (&root->val);
    else {
      T *l = find(root->left, elem);
      if (l == nullptr) {
        l = find(root->right, elem);
        if (l == nullptr)
          return nullptr;
        else
          return l;
      } else
        return l;
    }
  }

  T get_max(node *root) {
    if (root)
      return root->add + root->max * root->mpc;
    else
      return -service::INF;
  }

  T get_min(node *root) {
    if (root)
      return root->add + root->min * root->mpc;
    else
      return service::INF;
  }

  T get_sum(node *root) {
    if (root)
      return root->sum * root->mpc - root->add * root->size;
    else
      return 0;
  }

  int depth(node *root) {
    if (root == nullptr) return 0;
    return 1 + service::max(depth(root->left), depth(root->right));
  }

  void gr_op(char op, int val, node *root, int left, int right) {
    auto res1 = split_kth(root, right);
    auto res2 = split_kth(res1.first, left - 1);
    if (op == '+') res2.second->add += val;
    if (op == '-') res2.second->add -= val;
    if (op == '*') {
      res2.second->mpc *= val;
      res2.second->add *= val;
    }
    merge(merge(res2.first, res2.second), res1.second);
  }

  int gr_qu(char query, node *root, int left, int right) {
    auto res1 = split_kth(root, right);
    auto res2 = split_kth(res1.first, left - 1);
    int ans = 0;
    if (query == 'n') {
      int ans = res2.second->min;
      merge(merge(res2.first, res2.second), res1.second);
    }
    if (query == 'x') {
      int ans = res2.second->max;
      merge(merge(res2.first, res2.second), res1.second);
    }
    if (query == 's') {
      int ans = res2.second->sum;
      merge(merge(res2.first, res2.second), res1.second);
    }
    return ans;
  }

 public:
  treap() {
    root = nullptr;
    n = 0;
  }
  treap(node *n) { root = n; }
  treap(T val) {
    root = nullptr;
    n = 0;
    this.val = val;
  }
  treap(vector<T> &a) {
    root = nullptr;
    n = a.size();
    for (auto it = a.begin(); it != a.end(); it++) this.insert(*it);
  }

  // T gLeftData(){return root->left->val;}
  // T gRightData(){return root->right->val;}
  // T gValue(){return root->val;}

  void insert(int pos, T val) {
    if (pos < 0 || pos > n) return;
    node *q = new node(val, nullptr, nullptr);
    node *tl, *tr;
    split(root, pos, tl, tr);
    merge(tl, tl, q);
    merge(root, tl, tr);
    ++n;
  }

  T &at(int pos) {
    node *t = root;
    int less = 0;
    while (t) {
      int key = cnt(t->left) + less;
      if (pos == key) break;
      if (pos < key)
        t = t->left;
      else {
        less += cnt(t->left) + 1;
        t = t->r;
      }
    }
    return t->val;
  }

  T &operator[](int pos) {
    node *t = root;
    int less = 0;
    while (t) {
      int key = node::cnt(t->left) + less;
      if (pos == key) break;
      if (pos < key)
        t = t->left;
      else {
        less += node::cnt(t->left) + 1;
        t = t->right;
      }
    }
    return t->val;
  }

  void reverse(int left, int right) {
    left = service::max(left, 0);
    right = service::min(right, n - 1);
    if (left > right) return;

    node *tl, *tm, *tr;
    split(root, left, tl, tr);
    split(tr, right - left + 1, tm, tr);
    tm->rev ^= true;
    merge(tl, tl, tm);
    merge(root, tl, tr);
  }

  void move(int left, int right, int w) {
    if (w == 0 || left > right || left + w < 0 || right + w >= n) return;

    node *tl, *tm, *tr;
    split(root, left, tl, tr);
    split(tr, right - left + 1, tm, tr);
    merge(tl, tl, tr);
    split(tl, left + w, tl, tr);
    merge(tl, tl, tm);
    merge(root, tl, tr);
  }

  void erase(int left, int right) {
    left = service::max(left, 0);
    right = service::min(right, n - 1);
    if (left > right) return;

    node *tl, *tm, *tr;
    split(root, left, tl, tr);
    split(tr, right - left + 1, tm, tr);
    merge(root, tl, tr);
    n -= (right - left + 1);
  }

  T *find(T elem) { return find(root, elem); }

  T get_max() { return get_max(root); }

  T get_min() { return get_min(root); }

  T get_sum() { return get_sum(root); }

  bool isexist(T key) { return isexist(root, key); }

  int depth() { return depth(root); }

  size_t size() { return n; }

  void print() {
    print(root);
    cout << endl;
  }
};

#endif  // TREAP_H
