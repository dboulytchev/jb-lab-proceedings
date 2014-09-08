#include <string>
#include <iostream>
#include <sys/time.h>
#include <cstdlib>

#define currentTime() stats_rtclock()
#define elapsedTime(x) (x)

using namespace std;

unsigned
stats_rtclock( void )
{
  struct timeval t;
  struct timezone tz;

  if (gettimeofday( &t, &tz ) == -1)
    return 0;
  return (unsigned)(t.tv_sec * 1000 + t.tv_usec / 1000);
}

struct node // структура для представления узлов дерева
{
    int key;
    unsigned char height;
    node* left;
    node* right;
    node(int k) { key = k; left = right = 0; height = 1; }
};

unsigned char height(node* p)
{
    return p?p->height:0;
}

int bfactor(node* p)
{
    return height(p->right)-height(p->left);
}

void fixheight(node* p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl>hr?hl:hr)+1;
}

node* rotateright(node* p) // правый поворот вокруг p
{
    node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

node* rotateleft(node* q) // левый поворот вокруг q
{
    node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

node* balance(node* p) // балансировка узла p
{
    fixheight(p);
    if( bfactor(p)==2 )
    {
        if( bfactor(p->right) < 0 )
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if( bfactor(p)==-2 )
    {
        if( bfactor(p->left) > 0  )
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; // балансировка не нужна
}

node* insert(node* p, int k) // вставка ключа k в дерево с корнем p
{
    if( !p ) return new node(k);
    if( k < p->key )
        p->left = insert(p->left, k);
    else
        p->right = insert(p->right, k);
    return balance(p);
}

node* findmin(node* p) // поиск узла с минимальным ключом в дереве p 
{
    return p->left?findmin(p->left):p;
}

node* removemin(node* p) // удаление узла с минимальным ключом из дерева p
{
    if( p->left==0 )
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

node* remove(node* p, int k) // удаление ключа k из дерева p
{
    if( !p ) return 0;
    if( k < p->key )
        p->left = remove(p->left,k);
    else if( k > p->key )
        p->right = remove(p->right,k);	
    else //  k == p->key 
    {
        node* q = p->left;
        node* r = p->right;
        delete p;
        if( !r ) return q;
        node* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

int elNumber(node* n) {
  if (n) {
    return 1 + elNumber(n->left) + elNumber(n->right);
  }
  return 0;
}

void print(node* n, string indent = "", bool last = true) {
       cout << indent;
       if (last)
       {
           cout << "\\-";
           indent += "  ";
       }
       else
       {
           cout << "|-";
           indent += "| ";
       }
       cout << n->key << "\n";

       int child_count = 2;
       if (!n->left)  child_count--;
       if (!n->right) child_count--;
       if (n->left)  print(n->left, indent, 0 == child_count - 1);
       if (n->right) print(n->right, indent, 1 == child_count - 1);
}

int main(void)
{
    long tStart = currentTime();
    
    node * nod = new node(0);
    int size = 10000;
    // insert 100 element
    for (int i = 1; i < size; i++) {
        nod = insert(nod, i);
    }
    
    //remove 41 element
    for (int i = 0; i < size; i += (size / 25)) {
        nod = remove(nod, i);
    }
    for (int i = 0; i < size / 4; i++) {
        nod = remove(nod, i);
    }
    for (int i = 10; i < 1000000; i++) {
	nod = insert(nod, i);
	nod = insert(nod, i * 2 + 3);
	nod = insert(nod, i * 7 - 22);
	nod = insert(nod, i * 3);
	nod = remove(nod, i - 2);
    }

    long tFinish = currentTime();
    cout << "\ttime "
	<< elapsedTime(tFinish - tStart) << " msec" << endl;

    return 0;
}