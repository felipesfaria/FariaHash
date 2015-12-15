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

Node* ll_insert(LinkedList *ll, int key, int value){
    int i;
    Node* n = ll_get(ll,key);
    if(n!=NULL){
        n->value = value;
        return n;
    }
    n = n_init(key,value);
    if(ll->size==0) {
        ll->first = n;
        ll->last = n;
    } else{
        Node* last = ll->last;
        last->ll_next = n;
        n->ll_prev = last;
        ll->last = n;
    }
    ll->size=ll->size+1;
    return n;
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
    int size;
    int mapSize;
    float alpha;
    int maxSize;
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
    f->size =0;
    f->mapSize =(int)(maxSize / loadFactor);
    f->maxSize = maxSize;
    f->Hash = (LinkedList**)malloc(sizeof(LinkedList*)*f->mapSize);
    f->alpha = loadFactor;
    return f;
}

Node* lhm_get(LinkedHashMap *lhm, int key)
{
    int i = hash(lhm, key);
    LinkedList* ll = lhm->Hash[i];
    return ll_get(ll,key);
}

Node* lhm_put(LinkedHashMap *lhm, int key, int value)
{
    int i = hash(lhm, key);
    if(lhm->Hash[i] == NULL)
        lhm->Hash[i] = (LinkedList*)malloc(sizeof(LinkedList));
    Node* n = ll_insert(lhm->Hash[i], key, value);
    if(lhm->size == 0){
        lhm->mru = n;
        lhm->lru = n;
        lhm->size = 1;
        return n;
    }else {
        if(lhm->size==1 || (n->lhm_next == NULL && n->lhm_prev == NULL)) {
            lhm->size++;
        }else if(lhm->mru==n){
            return n;
        }else if(lhm->lru==n){
            lhm->lru = n->lhm_next;
            lhm->lru->lhm_prev == NULL;
        } else {
            n->lhm_prev->lhm_next = n->lhm_next;
        }
        lhm->mru->lhm_next = n;
        n->lhm_prev = lhm->mru;
        lhm->mru = n;
        return n;
    }
}

bool lhm_remove(LinkedHashMap *lhm, int key)
{
    int i = hash(lhm, key);
    if(ll_remove(lhm->Hash[i], key)){
        lhm->size--;
        return true;
    }
    return false;
}

void lhm_destroy(LinkedHashMap* lhm){
    int i;
    for(i=0;i<lhm->mapSize; ++i){
        ll_destroy(lhm->Hash[i]);
        free(lhm->Hash[i]);
    }
}

int hash(LinkedHashMap *f, int key){
    return key*37 % f->mapSize;
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
    if(lhm->size != 4) printf("Wrong size for lhm: expexted %d\t found: %d\n", 4, lhm->size);
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