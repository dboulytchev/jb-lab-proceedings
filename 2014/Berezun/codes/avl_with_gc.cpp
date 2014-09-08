#include <string>
#include <iostream>
#include <libgc/libgc.h>
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

/// represents node structure
struct node {
    int key;
    unsigned char height;
    gc_ptr<node> left;
    gc_ptr<node> right;
    node (int k = 0) : key(k), left(0), right(0), height(1) {}
};

unsigned char height (gc_ptr<node> p) {
    return p ? p->height : 0;
}

/// return balance factor
int bfactor (gc_ptr<node> p) {
    return height(p->right) - height(p->left);
}

/// fix height of node p
/// NB: all chields p must have right height
void fixheight (gc_ptr<node> p) {
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

/// right turn around p
gc_ptr<node> rotateright (gc_ptr<node> p) {
    gc_ptr<node> q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

/// left turn around q
gc_ptr<node> rotateleft (gc_ptr<node> q) {
    gc_ptr<node> p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

/// balance tree p
gc_ptr<node> balance (gc_ptr<node> p) {
    fixheight(p);
    if (bfactor(p) == 2) {
        if (bfactor(p->right) < 0) {
            p->right = rotateright(p->right);
	}
        return rotateleft(p);
    }
    if (bfactor(p) == -2) {
        if (bfactor(p->left) > 0) {
            p->left = rotateleft(p->left);
	}
        return rotateright(p);
    }
    return p;
}

/// instert key k in tree p
gc_ptr<node> insert (gc_ptr<node> p, int k) {
    if (!p) return gc_new<node, int>(k);
    if (k < p->key) p->left = insert(p->left, k);
    else p->right = insert(p->right, k);
    return balance(p);
}

/// find node with minimum key value in tree p 
gc_ptr<node> findmin (gc_ptr<node> p) {
    return p->left ? findmin(p->left) : p;
}

/// delete a node with minimum key value frome tree p
gc_ptr<node> removemin (gc_ptr<node> p) {
    if (p->left == 0) {
      return p->right;
    }
    p->left = removemin(p->left);
    return balance(p);
}

/// delete key k frome node p
gc_ptr<node> remove (gc_ptr<node> p, int k) {
    if (!p) return 0;
    if (k < p->key) {
        p->left = remove(p->left,k);
    } else if (k > p->key) {
        p->right = remove(p->right,k);
    } else {
        gc_ptr<node> q = p->left;
        gc_ptr<node> r = p->right;
        // delete p;
        if (!r) return q;
        gc_ptr<node> min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}

/// print node to console
/// @param indent --- string to put before key
void print(gc_ptr<node> n, string indent = "", bool last = true) {
    cout << indent;
    if (last) {
	cout << "\\-";
	indent += "  ";
    } else {
	cout << "|-";
	indent += "| ";
    }
    cout << n->key << "\n";

    int child_count = 2;
    if (!n->left)  child_count--;
    if (!n->right) child_count--;
    if (n->left)   print(n->left, indent, 0 == child_count - 1);
    if (n->right)  print(n->right, indent, 1 == child_count - 1);
}

int main(void)
{
    long tStart = currentTime();
    
    gc_ptr<node> nod = gc_new<node, int> (0);
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
    mark_and_sweep();
    
    long tFinish = currentTime();
    cout << "\ttime "
	<< elapsedTime(tFinish - tStart) << " msec" << endl;

    return 0;
}