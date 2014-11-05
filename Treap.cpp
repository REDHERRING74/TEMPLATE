struct node {
    int key;
    int fix;
    int sz;
    node *ch[2];
    node() {}
    node(int key, int fix): key(key), fix(fix) {}

    void update() {
        sz = ch[0]->sz + ch[1]->sz + 1;
    }
};

struct Treap {
    node *root, *null;

    Treap() {
        srand(time(0));
        null = new node(0, numeric_limits<int>::max());
        null->ch[0] = null->ch[1] = null;
        null->sz = 0;
        root = null;
    }

    node *NewNode(int val) {
        node *p = new node(val, rand());
        p->ch[0] = p->ch[1] = null;
        p->sz = 1;
        return p;
    }

    void Rotate(node *&x, int d) {
        node *y = x->ch[!d];
        x->ch[!d] = y->ch[d];
        y->ch[d] = x;
        x->update();
        y->update();
        x = y;
    }

    void Insert(node *&x, int val) {
        if (x == null) {
            x = NewNode(val);
            return;
        }
        int d = val <= x->key;
        Insert(x->ch[d], val);
        if (x->ch[d]->fix < x->fix)
            Rotate(x, !d);
        else
            x->update();
    }

    void Delete(node *&x, int val) {
        if (x->key == val) {
            if (x->ch[0] == null || x->ch[1] == null) {
                node *t = x;
                if (x->ch[0] == null)
                    x = x->ch[1];
                else
                    x = x->ch[0];
                delete t;
            } else {
                int d = x->ch[0]->fix < x->ch[1]->fix;
                Rotate(x, d);
                Delete(x->ch[1], val);
            }
            return;
        }
        int d = x->key < val;
        Delete(x->ch[d], val);
        x->update();
    }

    node *Pred(node *x, int val, node *opt) {
        if (x == null) return opt;
        if (x->key <= val)
            return Pred(x->ch[1], val, x);
        return Pred(x->ch[0], val, opt);
    }

    node *Succ(node *x, int val, node *opt) {
        if (x == null) return opt;
        if (x->key >= val)
            return Succ(x->ch[0], val, x);
        return Succ(x->ch[1], val, opt);
    }

    int Rank(node *x, int val) {
        if (x == null) return 0;
        int cur = x->ch[0]->sz + 1;
        if (x->key == val) return cur;
        if (x->key > val)
            return Rank(x->ch[0], val);
        return Rank(x->ch[1], val) + cur;
    }

    int Select(node *x, int k) {
        int cur = x->ch[0]->sz + 1;
        if (k == cur) return x->key;
        if (k < cur)
            return Select(x->ch[0], k);
        return Select(x->ch[1], k - cur);
    }

};
