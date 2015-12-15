/* Hello World program */

#include<stdio.h>
#include <malloc.h>
#include <stdbool.h>

#define alfa 0.5

typedef struct _node {
    int key;
    int value;
    struct _node* lhm_next;
    struct _node* lhm_prev;
    struct _node* ll_next;
    struct _node* ll_prev;
} Node;

Node* n_init(int key, int value){
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

Node* ll_insert(LinkedList *ll, int key, int value){
    int i;
    if(ll->size==0){
        Node* n = n_init(key, value);
        ll->first = n;
        ll->last = n;
        ll->size = 1;
        return n;
    }
    Node* n = ll->first;
    do{
        if(n->key==key){
            n->value = value;
            return n;
        }
        n = n->ll_next;
    } while(n!=NULL);
    n = n_init(key, value);
    Node* last = ll->last;
    last->ll_next = n;
    n->ll_prev = last;
    ll->last = n;
    ll->size=ll->size+1;
    return n;
}

Node* ll_get(LinkedList* ll, int key){
    Node* n = ll->first;
    while(n!=NULL){
        if(n->key==key){
            return n;
        }
        n=n->ll_next;
    }
    return NULL;
}

bool ll_remove(LinkedList* ll, int key){
    Node* n = ll_get(ll,key);
    if(n==NULL) return false;
    Node* prev = n->ll_prev;
    Node* next = n->ll_next;
    if(ll->first==n)
        ll->first==next;
    if(ll->last==n)
        ll->last==prev;
    if(prev!=NULL)
        prev->ll_next = next;
    if(next!=NULL)
        next->ll_prev = prev;
    ll->size--;
    free(n);
    return true;
}

void ll_destroy(LinkedList* ll){
    if(ll==NULL) return;
    Node* n = ll->first;
    Node* next;
    while(n!=NULL){
        next = n->ll_next;
        free(n);
        n=next;
    }
}

typedef struct _linkedHashMap {
    LinkedList** Hash;
    int Size;
    int MapSize;
    float alpha;
    int MaxSize;
    Node* mru;
    Node* lru;
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
    f->MaxSize = maxSize;
    f->Hash = (LinkedList**)malloc(sizeof(LinkedList*)*f->MapSize);
    f->alpha = loadFactor;
    return f;
}

Node* lhm_get(LinkedHashMap *lhm, int key)
{
    int i = hash(lhm, key);
    LinkedList* ll = lhm->Hash[i];
    return ll_get(ll,key);
}

bool lhm_put(LinkedHashMap *lhm, int key, int value)
{
    int i = hash(lhm, key);
    if(lhm->Hash[i] == NULL)
        lhm->Hash[i] = (LinkedList*)malloc(sizeof(LinkedList));
    if(ll_insert(lhm->Hash[i], key, value)){
        lhm->Size+=1;
        return true;
    }
    return true;
}

bool lhm_remove(LinkedHashMap *lhm, int key)
{
    int i = hash(lhm, key);
    if(ll_remove(lhm->Hash[i], key)){
        lhm->Size--;
        return true;
    }
    return false;
}

void lhm_destroy(LinkedHashMap* lhm){
    int i;
    for(i=0;i<lhm->MapSize;++i){
        ll_destroy(lhm->Hash[i]);
        free(lhm->Hash[i]);
    }
}

int hash(LinkedHashMap *f, int key){
    return key*37 % f->MapSize;
}

int main(void)
{
    Node* n;
    LinkedHashMap* lhm;
    lhm = lhm_init(8, 0.5);
    lhm_put(lhm, 4, 21);
    lhm_put(lhm, 20, 24);
    lhm_put(lhm, 36, 25);
    lhm_put(lhm, 5, 26);
    lhm_put(lhm, 4, 26);
    n = lhm_get(lhm,4);
    if(lhm->Size!=4) printf("Wrong size for lhm: expexted %d\t found: %d\n",4,lhm->Size);
    if(lhm->Hash[4]->size!=3) printf("Wrong size for ll as Hash[4]: expexted 3\t found: %d\n",lhm->Hash[4]->size);
    if(n->value!=26) printf("Wrong value for index 4");
    bool success = lhm_remove(lhm,20);
    if(!success) printf("Failed to remove element.");
    n = lhm_get(lhm,20);
    if(n!=NULL) printf("Removed element was found.");
    lhm_destroy(lhm);
    hello();
    return 0;
}