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
    n->lhm_next = NULL;
    n->lhm_prev = NULL;
    n->ll_next = NULL;
    n->ll_prev = NULL;
    return n;
}

typedef struct _linkedList {
    Node* first;
    Node* last;
    int size;
} LinkedList;

LinkedList* ll_init(){
    LinkedList* ll = (LinkedList*)malloc(sizeof(LinkedList));
    ll->first = NULL;
    ll->last = NULL;
    ll->size = 0;
}
Node* ll_get(LinkedList* ll, int key){
    if(ll==NULL) return NULL;
    Node* n = ll->first;
    while(n!=NULL){
        if(n->key==key){
            return n;
        }
        n=n->ll_prev;
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
        last->ll_prev = n;
        n->ll_next = last;
        ll->last = n;
    }
    ll->size++;
    return n;
}

Node* ll_remove(LinkedList* ll, int key){
    Node* n = ll_get(ll,key);
    if(n==NULL) return n;
    Node* prev = n->ll_prev;
    Node* next = n->ll_next;
    if(ll->first==n)
        ll->first=prev;
    if(ll->last==n)
        ll->last=next;
    if(prev!=NULL)
        prev->ll_next = next;
    if(next!=NULL)
        next->ll_prev = prev;
    ll->size--;
    return n;
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
    int i;
    LinkedHashMap* lhm = malloc(sizeof(LinkedHashMap));
    lhm->size =0;
    lhm->mapSize =(int)(maxSize / loadFactor);
    lhm->maxSize = maxSize;
    lhm->Hash = (LinkedList**)malloc(sizeof(LinkedList*) * lhm->mapSize);
    for(i=0;i<lhm->mapSize;i++)
        lhm->Hash[i] = ll_init();
    lhm->alpha = loadFactor;
    return lhm;
}

Node* lhm_get(LinkedHashMap *lhm, int key)
{
    int i = hash(lhm, key);
    LinkedList* ll = lhm->Hash[i];
    return ll_get(ll,key);
}

bool lhm_remove(LinkedHashMap *lhm, int key)
{
    int i = hash(lhm, key);
    Node* n = ll_remove(lhm->Hash[i], key);
    if(n != NULL){
        lhm->size--;
        if(lhm->lru!=n && lhm->mru!=n){
            Node* prev = n->lhm_prev;
            Node* next = n->lhm_next;
            prev->lhm_next = next;
            next->lhm_prev = prev;
        }else {
            if (lhm->lru == n) {
                lhm->lru = n->lhm_next;
                if (lhm->lru != NULL)
                    lhm->lru->lhm_prev = NULL;
            }
            if (lhm->mru == n) {
                lhm->mru = n->lhm_prev;
                if (lhm->mru != NULL)
                    lhm->mru->lhm_next = NULL;
            }
        }
        free(n);
        return true;
    }
    return false;
}
bool lhm_pop(LinkedHashMap *lhm){
    if(lhm->size<1)
        return false;
    return lhm_remove(lhm,lhm->lru->key);
}

Node* lhm_put(LinkedHashMap *lhm, int key, int value)
{
    Node* n = lhm_get(lhm,key);
    if(n!=NULL)
        lhm_remove(lhm,key);

    int i = hash(lhm, key);

    n = ll_insert(lhm->Hash[i], key, value);
    n->lhm_prev = lhm->mru;
    lhm->mru = n;
    if(n->lhm_prev!=NULL)
        n->lhm_prev->lhm_next = n;
    if(lhm->lru==NULL)
        lhm->lru = n;
    lhm->size++;
    if(lhm->size>lhm->maxSize)
        lhm_pop(lhm);
    return n;
}

void lhm_destroy(LinkedHashMap* lhm){
    int i;
    for(i=0;i<lhm->mapSize; ++i){
        ll_destroy(lhm->Hash[i]);
        free(lhm->Hash[i]);
    }
}

int hash(LinkedHashMap *f, int key){
    return key % f->mapSize;
}
void test1(){
    Node* n;
    LinkedHashMap* lhm;
    lhm = lhm_init(8, 0.5);
    lhm_put(lhm, 4, 21);
    lhm_put(lhm, 20, 24);
    lhm_put(lhm, 36, 25);
    lhm_put(lhm, 5, 26);
    lhm_put(lhm, 4, 26);
    if(lhm->size != 4) printf("Wrong size for lhm: expexted %d\t found: %d\n", 4, lhm->size);
    if(lhm->Hash[4]->size!=3) printf("Wrong size for ll as Hash[4]: expexted 3\t found: %d\n",lhm->Hash[4]->size);
    n = lhm_get(lhm,4);
    if(n->value!=26) printf("Wrong value for index 4 expected was 26");
    n = lhm_get(lhm,20);
    if(n->value!=24) printf("Wrong value for index 20 expected was 24.");
    bool success = lhm_remove(lhm,20);
    if(!success) printf("Failed to remove element.");
    n = lhm_get(lhm,20);
    if(n!=NULL) printf("Removed element was found.");
    lhm_destroy(lhm);
}
void testLRU(){
    int i;
    Node* n;
    LinkedHashMap* lhm;
    lhm = lhm_init(4, 0.5);
    for(i=1;i<6;i++) {
        lhm_put(lhm,i,i*2);
    }
    if(lhm->size != 4) printf("Wrong size for lhm: expexted %d\t found: %d\n", 4, lhm->size);
    n = lhm_get(lhm,1);
    if(n!=NULL) printf("Failed to remove LRU");
    for(i=2;i<6;i++){
        n = lhm_get(lhm,i);
        if(n->value!=i*2) printf("Wrong value for index %d\t expected:%d\t found: %d",i,i*2,n->value);
    }
    lhm_destroy(lhm);
}
int main(void)
{
    test1();
    testLRU();
    return 0;
}