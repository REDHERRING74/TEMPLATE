struct node {
    int sz, rev, delta, key;
    node *p, *ch[2];

    void reverse() {
        if (sz == 0) return;
        rev ^= 1;
        swap(ch[0], ch[1]);
    }

    void add(int v) {
        if (sz == 0) return;
        key += v;
        delta += v;
    }

    void pushdown() {
        if (delta) {
            ch[0]->add(delta);
            ch[1]->add(delta);
            delta = 0;
        }
        if (rev) {
            ch[0]->reverse();
            ch[1]->reverse();
            rev = 0;
        }
    }

    void update() {
        sz = ch[0]->sz + ch[1]->sz + 1;
    }
};

node *root, *null;

#define keytree root->ch[1]->ch[0]

template <int N>
struct Splay {
    int sz, top;
    node data[N], *stk[N];

    void init() {
        top = sz = 0;
        null = NewNode(0);
        null->sz = 0;

        root = NewNode(0);
        root->ch[1] = NewNode(0);
        root->ch[1]->p = root;
    }

    node *NewNode(int val) {
        node *p;
        if (top) p = stk[top--];
        else p = &data[sz++];
        p->sz = 1;
        p->ch[0] = p->ch[1] = p->p = null;
        p->key = val;
        return p;
    }

    node* build(int a[], int l, int r) {
        if (l > r) return null;
        int mid = (l + r) >> 1;
        node *p = NewNode(a[mid]);
        p->ch[0] = build(a, l, mid - 1);
        p->ch[1] = build(a, mid + 1, r);
        if (p->ch[0] != null) p->ch[0]->p = p;
        if (p->ch[1] != null) p->ch[1]->p = p;
        p->update();
        return p;
    }

    void maketree(int a[], int l, int r) {
        init();
        keytree = build(a, l, r);
        keytree->p = root->ch[1];
        splay(keytree, null);
    }

    void rotate(node *x, int c) {
        node *y = x->p;
        y->pushdown();
        x->pushdown();
        y->ch[!c] = x->ch[c];

        if (x->ch[c] != null)
            x->ch[c]->p = y;

        x->p = y->p;

        if (y->p != null)
            y->p->ch[ y == y->p->ch[1] ] = x;

        x->ch[c] = y;
        y->p = x;
        y->update();
        if (y = root) root = x;
    }

    void splay(node *x, node *f) {
        x->pushdown();
        while (x->p != f) {
            if (x->p->p == f) {
                rotate(x, x->p->ch[0] == x);
                break;
            }
            node *y = x->p;
            node *z = y->p;
            int c = (y == z->ch[0]);
            if (x == y->ch[c]) {
                rotate(x, !c);
                rotate(x, c);
            } else {
                rotate(y, c);
                rotate(x, c);
            }
        }
        x->update();
    }

    void select(int kth, node *x) {
        node *cur = root;
        while (1) {
            cur->pushdown();
            int tmp = cur->ch[0]->sz + 1;
            if (tmp == kth) break;
            if (tmp < kth) {
                kth -= tmp;
                cur = cur->ch[1];
            } else
                cur = cur->ch[0];
        }
        splay(cur, x);
    }

    void add(int l, int r, int v) {
        select(l, null);
        select(r + 2, root);
        keytree->add(v);
        splay(keytree, null);
    }

    void reverse(int l, int r) {
        select(l, null);
        select(r + 1, root);
        keytree->reverse();
    }

    void insert(int x, int v) {
        select(x + 1, null);
        select(x + 2, root);
        keytree = NewNode(v);
        keytree->p = root->ch[1];
        root->ch[1]->update();
        splay(keytree, null);
    }

    void ins(node *x, int v) {
        int c = x->key < v;
        x->pushdown();
        if (x->ch[c] == null) {
            x->ch[c] = NewNode(v);
            x->ch[c]->p = x;
            splay(x->ch[c], null);
        } else
            ins(x->ch[0], v);
    }

    void insert(int x, int l, int r, int a[]) {
        select(x + 1, null);
        select(x + 2, root);
        keytree = build(a, l, r);
        keytree->p = root->ch[1];
        root->ch[1]->update();
        splay(keytree, null);
    }

    void erase(node *x) {
        if (x == null) return;
        erase(x->ch[0]);
        erase(x->ch[1]);
        stk[++top] = x;
    }

    void dele(int l, int r) {
        select(l, null);
        select(r + 2, root);
        erase(keytree);
        keytree = null;
        root->ch[1]->update();
        root->update();
    }

    void deleroot() {
        node *oldroot = root;
        root = root->ch[1];
        root->p = null;
        select(1, null);
        root->ch[0] = oldroot->ch[0];
        root->ch[0]->p = root;
        root->update();
        stk[++top] = oldroot;
    }

    void dele(int x) {
        select(x + 1, null);
        deleroot();
    }

    void vis(node *x) {
        if (x == null) return;
        x->pushdown();
        vis(x->ch[0]);
        printf("node %d, key %d\n", (int)(x - data), x->key);
        vis(x->ch[1]);
    }

    void debug() {
        vis(root);
    }

};
