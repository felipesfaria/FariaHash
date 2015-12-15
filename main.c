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
    if(ll->size==0){
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
            return false;
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

Node* ll_get(LinkedList* ll, int key){
    Node* n = ll->first;
    while(n!=NULL){
        if(n->key==key){
            return n;
        }
        n=n->nextNode;
    }
    return NULL;
}

void ll_destroy(LinkedList* ll){
    if(ll==NULL) return;
    Node* n = ll->first;
    Node* next;
    while(n!=NULL){
        next = n->nextNode;
        free(n);
        n=next;
    }
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

LinkedHashMap* lhm_init(int maxSize, float loadFactor)
{
    LinkedHashMap* f = malloc(sizeof(LinkedHashMap));
    f->Size=0;
    f->MapSize=(int)(maxSize/loadFactor);
    f->Hash = (LinkedList**)malloc(sizeof(LinkedList*)*f->MapSize);
    f->alpha = loadFactor;
    return f;
}

Node* lhm_get(LinkedHashMap *f, int key)
{
    int i = hash(f,key);
    LinkedList* ll = f->Hash[i];
    return ll_get(ll,key);
}

bool lhm_put(LinkedHashMap *lhm, int key, int value)
{
    int i = hash(lhm, key);
    if(lhm->Hash[i] == NULL)
        lhm->Hash[i] = (LinkedList*)malloc(sizeof(LinkedList));
    if(LL_Insert(lhm->Hash[i], key, value)){
        lhm->Size+=1;
        return true;
    }
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
void lhm_destroy(LinkedHashMap* lhm){
    int i;
    for(i=0;i<lhm->MapSize;++i){
        ll_destroy(lhm->Hash[i]);
        free(lhm->Hash[i]);
    }
}
int main(void)
{
    LinkedHashMap*lhm;
    lhm = lhm_init(8, 0.5);
    lhm_put(lhm, 4, 21);
    lhm_put(lhm, 20, 24);
    lhm_put(lhm, 36, 25);
    lhm_put(lhm, 5, 26);
    lhm_put(lhm, 4, 26);
    Node* n = lhm_get(lhm,4);
    if(lhm->Size!=4) printf("Wrong size for lhm: expexted %d\t found: %d\n",4,lhm->Size);
    if(n->value!=26) printf("Wrong value for index 4");
    lhm_destroy(lhm);
    hello();
    return 0;
}