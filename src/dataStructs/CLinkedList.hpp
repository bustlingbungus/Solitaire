#ifndef CLINKEDLIST_HPP
#define CLINKEDLIST_HPP

/* A Node in a CLinkedList */
template <typename T>
struct CNode
{
    CNode<T> *next = nullptr;
    T elem;
};

/* A circularly linked list */
template <typename T>
class CList
{
    public:
        /* Creates an empty CLinkedList */
        CList() : cursor(nullptr), n(0) {}
        /* Deallocates resources */
        ~CList() { clear(); }

        bool empty() const { return cursor == nullptr; }
        int size() const { return n; }

        /* Inserts a new node i nodes in front of the cursor */
        void insert(CNode<T> *node, const int& i) 
        {
            if (cursor == nullptr) {
                cursor = node;
                cursor->next = node;
            } else {
                CNode<T> *v = cursor;
                for (int j=0;j<i;j++) v = v->next;
                node->next = v->next;
                v->next = node;
            }
            n++;
        }

        /* Inserts a new element i nodes in front of the cursor */
        void insert(const T& elem, const int& i)
        {
            CNode<T> *node = new CNode<T>;
            node->elem = elem;
            insert(node, i);
        }

        /* Inserts a new node in front of the cursor */
        void insert_front(CNode<T> *node) { insert(node, 0); }
        /* Inserts a new element in front of the cursor */
        void insert_front(const T& elem) { insert(elem, 0); }

        /* Sets the element i nodes away from the cursor */
        void set(const int& i, const T& elem) const
        {
            CNode<T> *v = cursor->next;
            for (int j=0;j<i;j++) v = v->next;
            v->elem = elem;
        }

        /* Removes the node following from the list */
        void remove(CNode<T> *node)
        {
            if (node == cursor && n == 1) {
                delete cursor;
                cursor = nullptr;
            } else {
                CNode<T> *o = node->next;
                if (o == cursor) cursor = cursor->next;
                node->next = o->next;
                delete o;
            }
            n--;
        }

        /* Removes the ith node from the list */
        void remove(const int& i)
        {
            CNode<T> *v = cursor;
            for (int j=0;j<i;j++) v = v->next;
            remove(v);
        }

        /* Removes the front node of the list */
        void remove_front() { remove(0); }

        /* Returns the pointer i nodes from the front */
        CNode<T>* at(const int& i) const
        {
            CNode<T> *v = cursor->next;
            for (int j=0;j<i;j++) v = v->next;
            return v;
        }

        /* Returns the front element */
        T front() const { return at(0)->elem; }
        /* Returns the back pointer */
        CNode<T>* back_ptr() const { return cursor; }
        /* Returns the element at the back pointer */
        T back() const { return cursor->elem; }

        /* Returns the element i nodes fron the front */
        T operator[](const int& i) const { return at(i)->elem; }

        /* Empties the list, deallocating resources */
        void clear() {
            while (!empty()) remove_front();
        }

        /* Advances the cursor i nodes in the list */
        void advance(const int& i = 1) {
            for (int j=0;j<i;j++) cursor = cursor->next; 
        }

        /* Sets the cursor to the specified pointer */
        void setCursor(CNode<T> *node) {
            cursor = node;
        }

        void operator=(const CList<T>& other) {
            cursor = other.back_ptr();
            n = other.size();
        }

    private:

        /* the current position in the list */
        CNode<T> *cursor;
        /* the size of the list */
        int n;
};

#endif