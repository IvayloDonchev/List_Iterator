//Линеен списък с вграден итератор
#include <iostream>
#include <initializer_list>
using namespace std;

template <typename T>
class List {			//линеен едносвързан списък
	struct Node {
		T key;
		Node* next;
		Node():key(), next(nullptr){}
		Node(T x):key(x), next(nullptr){}
	};
	Node* top;							//first element
public:
	List():top(nullptr){}				//default constructor
	List(T x):top(new Node(x)){}		//constructor
	List(initializer_list<T>);			//initializer list constructor
	~List();							//destructor
	List(const List&);					//copy constructor
	List(List&&);						//move constructor
	List& operator =(const List&);		//copy assignment
	List& operator =(List&&);			//move assignment
	bool push_front(T);					//add new element to the top
	bool push_back(T);					//add new element to the bottom
	T& operator [](int);				//index operator
	size_t size();						//number of elements in the list
	friend ostream& operator << (ostream&, const List&);
	class iterator {					//част от класа List
		Node* p;
	public:
		iterator():p(nullptr){}
		iterator(List<T>& t):p(t.top){}
		iterator(const iterator& other):p(other.p){}
		T operator ++() {
			if(p->next)
				p=p->next;
			else
				p=nullptr;
			return T(p);
		}
		T current() const {
			if(!p) return 0;
			return p->key;
		}
		T operator *() const {
			return current();
		}
		bool operator ==(const iterator& other) const {
			return p == other.p;
		}
		bool operator !=(const iterator& other) const {
			return p != other.p;
		}
	};
	iterator begin(){return iterator(*this);}
	iterator end(){return iterator();}
	friend class iterator;	//за да може да се използва извън класа
};

template <typename T>
List<T>::List(initializer_list<T> src)
{
	top=nullptr;
	for(auto p:src)
		push_back(p);
}

template<typename T>
List<T>::~List()
{
	while(top)
	{
		Node* p = top;
		top = top->next;
		delete p;
	}
	top = nullptr;
}

template <typename T>
List<T>::List(const List& other):top(new Node(other.top->key))
{
	Node* r = top;
	for(Node* p = other.top->next; p; p=p->next)
	{
		Node* q = new Node(p->key);
		r->next = q;
		r = q;
	}
}

template <typename T>
List<T>::List(List&& other):top(move(other.top))
{
	other.top = nullptr;
}

template <typename T>
List<T>& List<T>::operator =(const List& other)
{
	if(this == &other) return *this;
	this->~List();	//извикване на деструктора, който ще освободи паметта на левия списък
	top = new Node(other.top->key);
	Node* r = top;
	for(Node* p = other.top->next; p; p=p->next)
	{
		Node* q = new Node(p->key);
		r->next =q;
		r = q;
	}
	return *this;
}

template <typename T>
List<T>& List<T>::operator =(List<T>&& other)
{
	if(this == &other) return *this;
	this->~List();
	top = move(other.top);
	other.top = nullptr;
	return *this;
}

template <typename T>
bool List<T>::push_front(T x)
{
	Node* p = new Node(x);
	if(!p) return false;
	p->next = top;
	top = p;
	return true;
}

template <typename T>
bool List<T>::push_back(T x)
{
	Node* p = new Node(x);
	if(!p) return false;
	if(!top)
		top = p;
	else
	{
		Node* q;
		for(q=top; q->next; q=q->next);
		q->next = p;
	}
	return true;
}

template <typename T>
T& List<T>::operator [](int ind)
{
	Node* p=top;
	for(int i=0; i<ind; ++i)
		p=p->next;
	return p->key;
}

template <typename T>
size_t List<T>::size()
{
	size_t len=0;
	for(Node* p=top; p; p=p->next)
		++len;
	return len;
}

ostream& operator << (ostream& out, const List<int>& L)
{
	for(auto p=L.top; p; p=p->next)
		out << p->key << " ";
	return out;
}

int main()
{
	List<int> l1;
	for(int i=1; i<=10; ++i)
		l1.push_back(i);
	for(size_t i=0; i<l1.size(); ++i)
		cout << l1[i] << endl;
	cout << l1 << endl;
	for(List<int>::iterator i=l1.begin(); i!=l1.end(); ++i)
		cout << *i << ' ';
	cout << endl;

	cout << l1[9] << endl;
	l1[9] = 100;
	cout << l1 << endl;
	cout << "size= " << l1.size() << endl;

	cout << "............................................\n";
	for (auto x : l1)
		cout << x << " ";
	cout << "\n............................................\n";
	List<int> l2(l1);
	cout << l2 << endl;
	l1.push_front(-5);
	cout << "list l1: " << l1 << endl;
	cout << "list l2: " << l2 << endl;

	List<int> l3(move(l2));
	cout << "list l3: " << l3 << endl;
	cout << "list l2: " << l2 << endl;

	l1=l3;
	cout << "list l1: " << l1 << endl;
	cout << "list l3: " << l3 << endl;

	l3 = move(l1);
	cout << "list l1: " << l1 << endl;
	cout << "list l3: " << l3 << endl;

	List<int> l4{5,4,3,2,1};
	cout << "list l4: " << l4 << endl;
}
