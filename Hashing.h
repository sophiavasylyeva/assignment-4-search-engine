#ifndef HASHING_H
#define HASHING_H

#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;
template <typename Key, typename Value>

class Hashing{
  //function decleration 
private:
    struct HashNode{
        Key key;
        map<Value, int> vals;
        HashNode* next;
    
        HashNode(Key k, Value v, int f){
            key = k;
            vals[v] = f;
            next = nullptr;
        }
        HashNode(Key k, map<Value, int> v ){
            key = k;
            vals = v;
            next = nullptr;
        }
        HashNode(const HashNode& n){
            key = n.key;
            vals =n.vals;
            next = nullptr;
        }
    };
    int capacity;
    int size;
    HashNode** Table;
    void rehash();
    int Hash(Key);

public:
    Hashing(int n = 100);
    Hashing(const Hashing&);  //rule of three
    ~Hashing();
    Hashing &operator=(const Hashing&);  
    map<Value, int> find(const Key k);
    void insert(Key, Value, int);  
    void insert(Key, map<Value, int>);  
    int getsize();
    int getcapacity();
    void clear();
    void createHash(int n = 100);
    void clone(const Hashing&);
    void saveTree(ostream&);
    void saveToFile(ostream&, const Key&, const map<Value, int> &);

};

template <typename Key, typename Value>
Hashing<Key,Value>::Hashing(int n) {
    createHash(n);
}

template <typename Key, typename Value>
void Hashing<Key,Value>::createHash(int n){ 
    capacity = n;    
    size = 0;
    Table = new HashNode*[capacity];
    for(int i =0; i < capacity; i++){
        Table[i] = nullptr;
    }
}


template <typename Key, typename Value>
Hashing<Key,Value>::Hashing(const Hashing& h)//copy constructor
 {
    clone(h.Table);
 }

 template <typename Key, typename Value>
 void Hashing<Key,Value>::clear(){
    for(int i = 0; i < capacity; i ++){
        HashNode* itr = Table[i];
        while(itr != nullptr){
            HashNode* prev = itr;
            itr = itr->next;
            delete prev;
            size--;
        }
    }
    delete[] Table;
 }

template <typename Key, typename Value>
 Hashing<Key,Value>::~Hashing(){
    clear();

    // createHash(100);
    //destructor call clear function and create table
 }


 template <typename Key, typename Value>
 void Hashing<Key,Value>::clone(const Hashing& copy){
    createHash(copy.capacity);
    size = copy.size;
    for(int i = 0; i < capacity;i ++){
        HashNode* itr1 = copy.Table[i];
        HashNode* itr2 = Table[i];
        while(itr1 != nullptr){
            itr2 = new HashNode(*itr1);
            itr1= itr1->next;
            itr2= itr2 ->next;
        }

    }
 }
 
 template <typename Key, typename Value>
 int Hashing<Key,Value>::Hash(Key k){
    int index = std::hash<Key>{}(k);
    return abs(index%capacity);
 }

 template <typename Key, typename Value>
 void Hashing<Key,Value>::insert(Key k, Value v, int f){  //double check function 
    int index = Hash(k);
    if(Table[index] == nullptr){
        Table[index] = new HashNode(k,v,f);
        size++;
    }
    else{
        HashNode* itr = Table[index];
        HashNode* prev;
        while(itr != nullptr){
            if(itr->key == k){
                //itr->value.push_back(v);
                if(itr->vals.find(v) == itr->vals.end()){
                    itr->vals[v] = f;
                }
                break;
            }
            prev = itr;
            itr=itr->next;
        }
        if(itr == nullptr)
        {
        prev->next= new HashNode(k,v,f);
        size++; 
        }
    }
    if(size == capacity){
        rehash();
    }
 }
template <typename Key, typename Value>  ///fix formatting
 map<Value, int> Hashing<Key,Value>::find(const Key k){
    int index = Hash(k);
    HashNode *itr = Table[index];
    while(itr != nullptr){
        if(itr->key == k){ //looking for value
          return itr->vals;
        }
        itr=itr->next; 
    }
   return map<Value, int>();

}


template <typename Key, typename Value>
void Hashing<Key,Value>::saveTree(ostream &out)//new find function using map instead of vector 
{                            
    for(int i = 0; i < capacity;i++){
        HashNode* itr = Table[i];
        while(itr != nullptr){
          saveToFile(out, itr->key, itr->vals);
        }
    }
}


template <typename Key, typename Value>  
int Hashing<Key,Value>::getsize(){
    return size; //returning length 
}

template <typename Key, typename Value>  
int Hashing<Key,Value>::getcapacity(){
    return capacity; //returning length 
}

template <typename Key, typename Value>
void Hashing<Key,Value>::rehash(){  //double check function and make sure I called right varaiables 
    int oldCap = capacity;  
    capacity *= 2;  //making capacity bigger 
    HashNode** oldTable = Table;   //creating new table 
    createHash(capacity);
    for(int i = 0; i < oldCap;i++){
        HashNode* itr = oldTable[i];
        while(itr != nullptr){
            insert(itr->key,itr->vals);    //Value
            HashNode* temp = itr;
            itr = itr->next;
            delete temp;
        }
        oldTable[i] = nullptr;
    }
    delete [] oldTable;
}
template <typename Key, typename Value>
void Hashing<Key,Value>::insert(Key k, map<Value, int> v){
    int index = Hash(k);
    if(Table[index] == nullptr){
        Table[index] = new HashNode(k,v);
        size++;
    }
    else{
        HashNode* itr = Table[index];
        HashNode* prev;
        while(itr != nullptr){
            if(itr->key == k){
                //itr->value.push_back(v);
                itr->vals = v;
                break;
            }
            prev = itr;
            itr=itr->next;
        }
        if(itr == nullptr)
        {
        prev->next= new HashNode(k,v);
        size++; 
        }
    }
    if(size == capacity){
        rehash();
    }
 }
 template <typename Key, typename Value>
 Hashing<Key,Value>& Hashing<Key,Value>::operator=(const Hashing& y){
    clear();
    clone(y);
    /// add copy constructor
    return *this;
    
 }

template <typename Key, typename Value>
void Hashing<Key,Value>::saveToFile(ostream& out, const Key& k, const map<Value, int> &v){
      out << k << " ";
        for (auto& a : v) {
            out << a.first << " " << a.second << " ";
        }
        out << "\n";
}


#endif