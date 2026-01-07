#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

struct Node{
    int key; //int val;
    Node* prev;
    Node* next;
    Node(int k){
        key = k;
        prev = nullptr;
        next = nullptr;
    }
};

struct T{
    unordered_map<int, Node*> mp;
    Node *head = nullptr, *tail = nullptr;
    int size = 0;
};

class ARCCache{
    int capacity;
    int p; // adaptive parameter

    T T1, T2; // main lists
    unordered_set<int> B1, B2; // ghost lists

    void addToFront(T& t, Node* n){
        n->prev = nullptr;
        n->next = t.head;
        if(t.head) t.head->prev = n;
        t.head = n;
        if(!t.tail) t.tail = n;
        t.size++;
        t.mp[n->key] = n;
    }

    void removeNode(T& t, Node* n){
        if(n->prev) n->prev->next = n->next;
        else t.head = n->next;

        if(n->next) n->next->prev = n->prev;
        else t.tail = n->prev;

        t.mp.erase(n->key);
        t.size--;
    }

    void moveToFront(T& t, Node* n){
        if(t.head == n) return;
        removeNode(t, n);
        addToFront(t, n);
    }

    void moveT1toT2(int key){
        Node* n = T1.mp[key];
        removeNode(T1, n);
        addToFront(T2, n);
    }

    void trimGhosts(){
        while((int)(B1.size() + B2.size()) > capacity){
            if(B1.size() > B2.size())
                B1.erase(B1.begin());
            else
                B2.erase(B2.begin());
        }
    }

    void replace(){ // evict
        if(T1.size > 0 && (T1.size > p || T2.size == 0)){
            Node* victim = T1.tail;
            removeNode(T1, victim);
            B1.insert(victim->key);
            delete victim;
        }else{
            Node* victim = T2.tail;
            removeNode(T2, victim);
            B2.insert(victim->key);
            delete victim;
        }
        trimGhosts();
    }

public:
    ARCCache(int capacity){
        this->capacity = capacity;
        this->p = 0;
    }

    bool access(int key) {
        // Hit in T1
        if(T1.mp.count(key)){
            moveT1toT2(key);
            return true;
        }

        // Hit in T2
        if(T2.mp.count(key)){
            moveToFront(T2, T2.mp[key]);
            return true;
        }

        // Ghost hit in B1
        if(B1.count(key)){
            p = min(capacity, p + max(1, (int)B2.size() / max(1, (int)B1.size())));
            replace();
            B1.erase(key);
            addToFront(T2, new Node(key));
            return false;
        }

        // Ghost hit in B2
        if(B2.count(key)){
            p = max(0, p - max(1, (int)B1.size() / max(1, (int)B2.size())));
            replace();
            B2.erase(key);
            addToFront(T2, new Node(key));
            return false;
        }

        // Cache miss
        if(T1.size + T2.size == capacity)
            replace();

        addToFront(T1, new Node(key));
        return false;
    }
};

int main(){
    int keys[] = {1,2,3,2,1,4,5,1,2,3,4,5};
    int n = sizeof(keys)/sizeof(keys[0]);

    ARCCache cache(4);

    for (int i = 0; i < n; i++) {
        cout << cache.access(keys[i]) << " ";
    }
    cout << endl;
    return 0;
}
