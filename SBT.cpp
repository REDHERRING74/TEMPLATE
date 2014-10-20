const int inf = 0x3f3f3f3f;

struct node {
    node *ch[2];
    int sz, key;
    void update() {
        sz = ch[0]->sz + ch[1]->sz + 1;
    }
};

struct SBT {
    node *root, *null;

    SBT() {
        null = new node;
        null->sz = 0;
        null->ch[0] = null->ch[1] = null;
        root = null;
    }

    node *NewNode(int val) {
        node *p = new node;
        p->sz = 1, p->key = val;
        p->ch[0] = p->ch[1] = null;
        return p;
    }

    //the element of a[] must be ordered
    node *build(int a[], int l, int r) {
        if (l > r) return null;
        int mid = (l + r) >> 1;
        node *p = NewNode(a[mid]);
        p->ch[0] = build(a, l, mid - 1);
        p->ch[1] = build(a, mid + 1, r);
        p->update();
        return p;
    }

    void maketree(int a[], int l, int r) {
        root = build(a, l, r);
    }

    void rotate(node* &x, int c) {
        node *k = x->ch[!c];
        x->ch[!c] = k->ch[c];
        k->ch[c] = x;
        k->sz = x->sz;
        x->update();
        x = k;
    }

    void maintain(node* &x, bool c) {
        if (x == null) return;
        if (x->ch[c]->ch[c]->sz > x->ch[!c]->sz)
            rotate(x, !c);
        else if (x->ch[c]->ch[!c]->sz > x->ch[!c]->sz) {
            rotate(x->ch[c], c);
            rotate(x, !c);
        }
        else return;

        maintain(x->ch[0], 0);
        maintain(x->ch[1], 1);
        maintain(x, 0);
        maintain(x, 1);
    }

    //ins(root, val) -- insert a node with value val
    void ins(node* &x, int val) {
        if (x == null) x = NewNode(val);
        else {
            ++x->sz;
            int d = val >= x->key;
            ins(x->ch[d], val);
            maintain(x, d);
        }
    }

    //del(root, val) -- delete a node with value val
    int del(node* &x, int val) {
        --x->sz;
        int d = x->key < val;
        if (x->key == val || x->ch[d] == null) {
            int ret = x->key;
            if (x->ch[d] == null) {
                node *y = x->ch[!d];
                delete x;
                x = y;
            } else x->key = del(x->ch[0], x->key + 1);
            return ret;
        } else return del(x->ch[d], val);
    }

    //LessAndEqual(root, val) -- get the number of node with a value less than or equal to val
    int LessAndEqual(node *x, int val) {
        if (x == null) return 0;
        int ret = x->ch[0]->sz + 1;
        if (x->key <= val)
            return ret + LessAndEqual(x->ch[1], val);
        else
            return LessAndEqual(x->ch[0], val);
    }

    //pred(root, val) -- get the biggest value that is smaller than val
    int pred(node *x, int val) {
        if (x == null) return -inf;
        if (x->key > val)
            return pred(x->ch[0], val);
        else
            return max(x->key, pred(x->ch[1], val));
    }

    //succ(root, val) -- get the smallest value that is bigger than val
    int succ(node *x, int val) {
        if (x == null) return inf;
        if (x->key < val)
            return succ(x->ch[1], val);
        else
            return min(x->key, succ(x->ch[0], val));
    }

    //select(root, k) -- return the k-th smallest value
    int select(node *x, int k) {
        int sz = x->ch[0]->sz + 1;
        if (sz == k) return x->key;
        if (sz < k)
            return select(x->ch[1], k - sz);
        return select(x->ch[0], k);
    }

    //Rank(root, val) -- return the rank of value val
    int Rank(node *x, int val) {
        int ret = x->ch[0]->sz + 1;
        if (x->key == val) return ret;
        if (x->key > val)
            return Rank(x->ch[0], val);
        return ret + Rank(x->ch[1], val);
    }
};
