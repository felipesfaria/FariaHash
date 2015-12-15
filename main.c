/* Hello World program */

#include<stdio.h>
#include <malloc.h>
#include <stdbool.h>

#define alfa 0.5

typedef struct _node {
    int key;
    int value;
    int next;
    int previous;
    struct _node* nextNode;
    struct _node* prevNode;
} Node;

Node* initNode(int key, int value){
    Node* n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->value = value;
    return n;
}

typedef struct _linkedList {
    Node* first;
    Node* last;
    int size;
} LinkedList;

bool LL_Insert(LinkedList* ll, int key, int value){
    int i;
    if(ll==NULL)
        ll = (LinkedList*)malloc(sizeof(LinkedList));
    if(ll->size=0){
        Node* n = initNode(key,value);
        ll->first = n;
        ll->last = n;
        ll->size = 1;
        return true;
    }
    Node* n = ll->first;
    do{
        if(n->key==key){
            n->value = value;
            return true;
        }
        n = n->nextNode;
    } while(n!=NULL);
    n = initNode(key,value);
    Node* last = ll->last;
    last->nextNode = n;
    n->prevNode = last;
    ll->last = n;
    ll->size=ll->size+1;
    return true;
}

typedef struct _linkedHashMap {
    LinkedList** Hash;
    int Size;
    int MapSize;
    float alpha;
} LinkedHashMap;

void hello()
{
    printf("Hello World!");
}

LinkedHashMap* initLinkedHashMap(int maxSize, float loadFactor)
{
    LinkedHashMap* f = malloc(sizeof(LinkedHashMap));
    f->Size=0;
    f->MapSize=(int)(maxSize/loadFactor);
    f->Hash = (LinkedList**)malloc(sizeof(LinkedList*)*f->MapSize);
    f->alpha = loadFactor;
    return f;
}

int get(LinkedHashMap *f, int key)
{
//    int i;
//    int k = hash(f, key);
//    for(i =0 ; i<f->MapSize;i++){
//        int index = (k+i)%f->MapSize;
//        if(f->Ocupied[index]==false)
//            return -1;
//        if(f->Hash[index].key==key)
//            return f->Hash[index].value;
//    }
    return -1;
}

bool put(LinkedHashMap *f, int key, int value)
{
    int hashIndex = hash(f,key);
    LinkedList* ll = f->Hash[hashIndex];
    if(ll==NULL)
        ll=(LinkedList*)malloc(sizeof(LinkedList));
    if(ll->size==0){
        Node n;
        n.key = key;
        n.value = value;
        n.nextNode = NULL;
        n.prevNode = NULL;
        ll->first = &n;
        ll->last = &n;
        ll->size=1;
        f->Size=f->Size+1;
        return true;
    }
    Node* i = ll->first;
    while(i!=NULL){
        if(i->key==key) break;
        i=i->nextNode;
    }
    if(i==NULL){
        Node n;
        n.key = key;
        n.value = value;
        n.nextNode = NULL;
        Node* last = ll->last;
        last->nextNode = &n;
        n.prevNode = last;
        ll->last = &n;
        ++ll->size;
        return true;
    }
    if(i->key==key){
        i->value = value;
        return true;
    }
    return false;

//    bool inserted = false;
//    int i =0;
//    int k = hash(f, key);
//    for(i =0 ; i<f->MapSize;i++){
//        int index = (k+i)%f->MapSize;
//        if(f->Ocupied[index]==false){
//            Node n;
//            n.key = key;
//            n.value = value;
//            f->Hash[index] = n;
//            f->Ocupied[index] = true;
//            f->Size++;
//            inserted = true;
//            break;
//        }
//    }
//    if(!inserted) return false;
    return true;
}
bool rem(LinkedHashMap *f, int key)
{
//    int i;
//    int k = hash(f, key);
//    for(i =0 ; i<f->MapSize;i++){
//        int index = (k+i)%f->MapSize;
//        if(f->Ocupied[index]==false)
//            return -1;
//        if(f->Hash[index].key==key)
//        {
//            f->Size--;
//            f->Ocupied[index] = false;
//        }
//    }
    return -1;
}
int hash(LinkedHashMap *f, int key){
    return key % f->MapSize;
}
int main(void)
{
    LinkedHashMap* hash;
    hash = initLinkedHashMap(8,0.5);
    put(hash, 4, 21);
    put(hash, 20, 24);
    put(hash, 36, 25);
    put(hash, 5, 26);
    int t = get(&hash, 13);
    hello();
    return 0;
}