// #include <algorithm>
// #include <cassert>
// #include <iostream>
// #include <map>
// #include <set>
// #include <string>
// #include <vector>

#include <bits/stdc++.h>

using namespace std;

struct Node {
  Node *next;
  Node *prev;
  int value;
  int key;
  Node(Node *p, Node *n, int k, int val)
      : prev(p), next(n), key(k), value(val){};
  Node(int k, int val) : prev(NULL), next(NULL), key(k), value(val){};
};

class Cache {

protected:
  map<int, Node *> mp;            // map the key to the node in the linked list
  int cp;                         // capacity
  Node *tail;                     // double linked list tail pointer
  Node *head;                     // double linked list head pointer
  virtual void set(int, int) = 0; // set function
  virtual int get(int) = 0;       // get function
};

class LRUCache : Cache {
public:
  LRUCache(const int capacity) {
    cp = capacity;
    tail = head = NULL; // double linked list tail pointer
    sz = 0;
  } // LRUCache(.)

  virtual void set(int key, int value) {
    // cerr << "set(" << key << "," << value << ") before: "; ShowChache();
    auto node_it = mp.find(key);
    if (node_it != mp.end()) { // key exists in the cache (and map)
      // cerr << " " << key << " exists! ";
      Node *node_ptr = node_it->second;
      node_ptr->value = value;
      if (node_ptr->prev)
      {
        // move *node_ptr in front of the queue
        node_ptr->prev->next = node_ptr->next;
        node_ptr->next = head;
        head->prev = node_ptr;
        head = node_ptr;
      }
    } else { // its a new key
      // cerr << " " << key << " doesnt exist! ";

      // make sure size <= capacity - 1
      while (sz >= cp) {
        // cerr << " sz=" << sz;
        const auto pre_tail = tail->prev;
        // cerr << " tail=(" << tail->key << "," << tail->value << ") ";
        mp.erase(tail->key);
        delete (tail);
        tail = pre_tail;
        tail->next = NULL;
        --sz;
      } // while (sz >= cp)
      AddNewNode(key, value);
      mp[key] = head;
      ++sz;
    } // if mp.contains(key)
    // cerr << " after: "; ShowChache(); cerr << endl;
  } // set(..)

  virtual int get(int key) {
    // cerr << "get(" << key << "): ";
    // ShowChache();
    cerr << endl;
    int result = -1;
    auto node_it = mp.find(key);
    if (node_it != mp.end()) { // key exists in the cache (and map)
      result = node_it->second->value;
    } // if mp.contains(key)
    return result;
  } // get(..)

private:
  int sz{0}; // cache size <= capacity

  void ShowChache() {
    auto n = head;
    while (n) {
      cerr << " (" << n->key << "," << n->value << ")";
      n = n->next;
    }
  }

  void AddNewNode(int key = 0, int value = 0) {
    // cerr << " AddNewNode(" << key << "," << value << ") ";
    Node *newnode = (Node *)malloc(sizeof(Node));
    newnode->key = key;
    newnode->value = value;
    newnode->prev = NULL;
    newnode->next = head;
    if (head != NULL)
      head->prev = newnode;
    if (tail == NULL)
      tail = head;
    head = newnode;

    head = newnode;
  } // InsertNode(..)
}; // LRUCache

int main() {
  int n, capacity, i;
  cin >> n >> capacity;
  LRUCache l(capacity);
  for (i = 0; i < n; i++) {
    string command;
    cin >> command;
    if (command == "get") {
      int key;
      cin >> key;
      cout << l.get(key) << endl;
    } else if (command == "set") {
      int key, value;
      cin >> key >> value;
      l.set(key, value);
    }
  }
  return 0;
}
