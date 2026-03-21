#ifndef SJTU_LRU_HPP
#define SJTU_LRU_HPP

#include "class-integer.hpp"
#include "class-matrix.hpp"
#include "exceptions.hpp"
#include "utility.hpp"

class Hash {
public:
    unsigned int operator()(Integer lhs) const {
        int val = lhs.val;
        return std::hash<int>()(val);
    }
};

class Equal {
public:
    bool operator()(const Integer &lhs, const Integer &rhs) const { return lhs.val == rhs.val; }
};

namespace sjtu {
template<class T>
class double_list {
private:
	struct node {
		T val;
		node *prev, *next;
		node (const T &v = T(), node *p = nullptr, node *n = nullptr) : val(v), prev(p), next(n) {}
	};
	node *head, *tail;
	int len;
public:
	/**
	 * elements
	 * add whatever you want
	 */
	

	// --------------------------
	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	 */
	double_list() {
		head=new node();
		tail=new node();
		head->next=tail;
		tail->prev=head;
		len=0;
	}
	double_list(const double_list<T> &other) {}
	~double_list() {}

	class iterator {
	private:
		node *ptr;
		double_list *list;
	public:
		/**
		 * elements
		 * add whatever you want
		 */
		// --------------------------
		/**
		 * the follows are constructors and destructors
		 * you can also add some if needed.
		 */
		iterator(node *p = nullptr, double_list *l = nullptr) : ptr(p), list(l) {}
		iterator() {}
		iterator(const iterator &t) {}
		~iterator() {}
		
		/**
		 * iter++
		 */
		iterator operator++(int) {
			iterator tmp=*this;
			++(*this);
			return tmp;
		}
		/**
		 * ++iter
		 */
		iterator &operator++() {
			
			if(ptr==list->tail) throw "invalid";
			ptr=ptr->next;
			return *this;
		}
		/**
		 * iter--
		 */
		iterator operator--(int) {
			iterator tmp=*this;
			--(*this);
			return tmp;
		}
		/**
		 * --iter
		 */
		iterator &operator--() {
			
			if(ptr==list->head->next) throw "invalid";
			ptr=ptr->prev;
			return *this;
		}
		
		/**
		 * if the iter didn't point to a value
		 * throw " invalid"
		 */
		T &operator*() const {
			if(ptr==list->tail) throw "invalid";
			return ptr->val;
		}
		
		/**
		 * other operation
		 */
		T *operator->() const noexcept {
			if(ptr==list->tail) throw "invalid";
			return &(ptr->val);
		}
		bool operator==(const iterator &rhs) const {
			return ptr==rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr!=rhs.ptr;
		}
		friend class double_list;
	};
	/**
	 * return an iterator to the beginning
	 */
	iterator begin() {
		return iterator(head->next, this);
	}
	/**
	 * return an iterator to the ending
	 * in fact, it returns the iterator point to nothing,
	 * just after the last element.
	 */
	iterator end() {
		return iterator(tail, this);
	}
	/**
	 * if the iter didn't point to anything, do nothing,
	 * otherwise, delete the element pointed by the iter
	 * and return the iterator point at the same "index"
	 * e.g.
	 * 	if the origin iterator point at the 2nd element
	 * 	the returned iterator also point at the
	 *  2nd element of the list after the operation
	 *  or nothing if the list after the operation
	 *  don't contain 2nd elememt.
	 */
	iterator erase(iterator pos) {
		if(pos.ptr==tail) return pos;
		node *p=pos.ptr;
		node *res=p->next;
		p->prev->next=p->next;
		p->next->prev=p->prev;
		len--;
		delete p;
		return iterator(res, this);
	}

	/**
	 * the following are operations of double list
	 */
	void insert_head(const T &val) {
		node *p=new node(val, head, head->next);
		head->next->prev=p;
		head->next=p;
		len++;
	}
	void insert_tail(const T &val) {
		node *p=new node(val,tail->prev, tail);
		tail->prev->next=p;
		tail->prev=p;
		len++;
	}
	void delete_head() {
		if(head->next==tail) return;
		node *p=head->next;
		head->next=p->next;
		p->next->prev=head;
		delete p;
		len--;
	}
	void delete_tail() {
		if(tail->prev==head) return;
		node *p=tail->prev;
		tail->prev=p->prev;
		p->prev->next=tail;
		delete p;
		len--;
	}
	/**
	 * if didn't contain anything, return true,
	 * otherwise false.
	 */
	bool empty() {
		return len==0;
	}
};

template<class Key, class T, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
class hashmap {
public:
	using value_type = pair<const Key, T>;
	/**
	 * elements
	 * add whatever you want
	 */
private:
	struct node {
		value_type val;
		node *next;
		node(const value_type &v = value_type(), node *n = nullptr) : val(v), next(n) {}
	};
	node **table;
	size_t size, capacity;
	float load_factor;
	Hash hasher;
	Equal equal;
	size_t hash(const Key &key) const {
		return hasher(key) % capacity;
	}
	// --------------------------

	/**
	 * the follows are constructors and destructors
	 * you can also add some if needed.
	 */
public:
	hashmap() {
		capacity=16;
		size=0;
		load_factor=0.75;
		table=new node*[capacity];
		for(size_t i=0;i<capacity;i++) table[i]=nullptr;
	}
	hashmap(const hashmap &other) {
		capacity=other.capacity;
		size=other.size;
		load_factor=other.load_factor;
		hasher=other.hasher;
		equal=other.equal;
		table=new node*[capacity];
		for(size_t i=0;i<capacity;i++) table[i]=nullptr;
		for(size_t i=0;i<capacity;i++) {
			node *p=other.table[i];
			node *tail=nullptr;
			while(p) {
				node *new_node = new node(p->val, nullptr);
    			if(tail) tail->next = new_node;
    			else table[i] = new_node;
    			tail = new_node;
    			p = p->next;
			}
		}
	}
	~hashmap() {
		for(size_t i=0;i<capacity;i++) {
			node *p=table[i];
			while(p) {
				node *next=p->next;
				delete p;
				p=next;
			}
		}
		delete [] table;
	}
	hashmap &operator=(const hashmap &other) {
		if(this==&other) return *this;
		capacity=other.capacity;
		size=other.size;
		load_factor=other.load_factor;
		hasher=other.hasher;
		equal=other.equal;
		node **new_table=new node*[capacity];
		for(size_t i=0;i<capacity;i++) new_table[i]=nullptr;
		for(size_t i=0;i<capacity;i++) {
			node *p=other.table[i];
			while(p) {
				node *new_node=new node(p->val, new_table[i]);
				new_table[i]=new_node;
				p=p->next;
			}
		}
		delete [] table;
		table=new_table;
		return *this;
	}

	class iterator {
	private:
		node **table;
		node *ptr;
		hashmap *map;
	public:
		/**
		 * elements
		 * add whatever you want
		 */
		// --------------------------
		/**
		 * the follows are constructors and destructors
		 * you can also add some if needed.
		 */
		iterator(node **t = nullptr, node *p = nullptr, hashmap *m = nullptr) : table(t), ptr(p), map(m) {}
		iterator() {}
		iterator(const iterator &t) {}
		~iterator() {}

		/**
		 * if point to nothing
		 * throw
		 */
		value_type &operator*() const {
			if(ptr==nullptr) throw "invalid";
			return ptr->val;
		}

		/**
		 * other operation
		 */
		value_type *operator->() const noexcept {
			return &(ptr->val);
		}
		bool operator==(const iterator &rhs) const {
			return ptr==rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr!=rhs.ptr;
		}
	};

	void clear() {
		for(size_t i=0;i<capacity;i++) {
			node *p=table[i];
			while(p) {
				node *next=p->next;
				delete p;
				p=next;
			}
			table[i]=nullptr;
		}
		size=0;
	}
	/**
	 * you need to expand the hashmap dynamically
	 */
	
	void expand() {
		size_t new_capacity=capacity*2;
		node **new_table=new node*[new_capacity];
		for(size_t i=0;i<new_capacity;i++) new_table[i]=nullptr;
		for(size_t i=0;i<capacity;i++) {
			node *p=table[i];
			while(p) {
				node *next=p->next;
				size_t index=hasher(p->val.first)%new_capacity;
				p->next=new_table[index];
				new_table[index]=p;
				p=next;
			}
		}
		delete [] table;
		table=new_table;
		capacity=new_capacity;
	}

	/**
	 * the iterator point at nothing
	 */
	iterator end() const {
		return iterator(table, nullptr, const_cast<hashmap*>(this));
	}
	/**
	 * find, return a pointer point to the value
	 * not find, return the end (point to nothing)
	 */
	iterator find(const Key &key) const {
		size_t index=hash(key);
		node *p=table[index];
		while(p) {
			if(equal(p->val.first, key)) return iterator(table, p, const_cast<hashmap*>(this));
			p=p->next;
		}
		return end();
	}
	/**
	 * already have a value_pair with the same key
	 * -> just update the value, return false
	 * not find a value_pair with the same key
	 * -> insert the value_pair, return true
	 */
	sjtu::pair<iterator, bool> insert(const value_type &value_pair) {
		iterator it=find(value_pair.first);
		if(it!=end()) {
			it->second=value_pair.second;
			return sjtu::pair(it, false);
		}
		if(size+1>capacity*load_factor) expand();
		size_t index=hash(value_pair.first);
		node *p=new node(value_pair, table[index]);
		table[index]=p;
		size++;
		return sjtu::pair(iterator(table, p, this), true);
	}
	/**
	 * the value_pair exists, remove and return true
	 * otherwise, return false
	 */
	bool remove(const Key &key) {
		size_t index=hash(key);
		node *p=table[index], *prev=nullptr;
		while(p) {
			if(equal(p->val.first, key)) {
				if(prev) prev->next=p->next;
				else table[index]=p->next;
				delete p;
				size--;
				return true;
			}
			prev=p;
			p=p->next;
		}
		return false;
	}
};

template<class Key, class T, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
class linked_hashmap : public hashmap<Key, T, Hash, Equal> {
public:
	typedef pair<const Key, T> value_type;
private:
	struct Listnode{
		value_type *data_ptr;
		Listnode *prev, *next;
		Listnode(const value_type *ptr = nullptr, Listnode *p = nullptr, Listnode *n = nullptr) : data_ptr(ptr), prev(p), next(n) {}
	};
	Listnode *list_head, *list_tail;
	size_t list_len;
public:
	/**
	 * elements
	 * add whatever you want
	 */
	// --------------------------
	bool move_to_front(const Key &key){
		auto it=find(key);
		if(it==end()) return false;
		value_type data=*it;
		remove(it);
		insert(data);
		return true;
	}
	void insert_to_front(const value_type &value) {
		auto hash_result = hashmap<Key, T, Hash, Equal>::insert(value);
		if(hash_result.second){
			Listnode *new_node = new Listnode(const_cast<value_type*>(&(*(hash_result.first))), list_head, list_head->next);
			new_node->prev=list_head;
			new_node->next=list_head->next;
			list_head->next->prev=new_node;
			list_head->next=new_node;
			list_len++;
		}
		else {
			move_to_front(value.first);
		}
	}
	class const_iterator;
	class iterator {
	private:
		Listnode *ptr;
		linked_hashmap *map;
	public:
		/**
		 * elements
		 * add whatever you want
		 */
		// --------------------------
		iterator(Listnode *p = nullptr, linked_hashmap *m = nullptr) : ptr(p), map(m) {}
		iterator() {}
		iterator(const iterator &other) {}
		~iterator() {}

		/**
		 * iter++
		 */
		iterator operator++(int) {
			iterator tmp=*this;
			++(*this);
			return tmp;
		}
		/**
		 * ++iter
		 */
		iterator &operator++() {
			if(ptr==map->list_tail) throw "invalid";
			ptr=ptr->next;
			return *this;
		}
		/**
		 * iter--
		 */
		iterator operator--(int) {
			iterator tmp=*this;
			--(*this);
			return tmp;
		}
		/**
		 * --iter
		 */
		iterator &operator--() {
			if(ptr==map->list_head) throw "invalid";
			ptr=ptr->prev;
			return *this;
		}

		/**
		 * if the iter didn't point to a value
		 * throw "star invalid"
		 */
		value_type &operator*() const {
			if(ptr==map->list_tail) throw "invalid";
			return *(ptr->data_ptr);
		}
		value_type *operator->() const noexcept {
			return ptr->data_ptr;
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr==rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr!=rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr==rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr!=rhs.ptr;
		}
		friend class const_iterator;
		friend class linked_hashmap;
	};

	class const_iterator {
	private:
		const Listnode *node;
		const linked_hashmap *map;
	public:
		/**
		 * elements
		 * add whatever you want
		 */
		// --------------------------
		const_iterator(const Listnode *p = nullptr, const linked_hashmap *m = nullptr) : node(p), map(m) {}
		const_iterator(const const_iterator &it) : node(it.node), map(it.map) {}
		const_iterator() {}
		const_iterator(const iterator &other) {}

		/**
		 * iter++
		 */
		const_iterator operator++(int) {
			const_iterator tmp=*this;
			++(*this);
			return tmp;
		}
		/**
		 * ++iter
		 */
		const_iterator &operator++() {
			if(node==map->list_tail) throw "invalid";
			node=node->next;
			return *this;
		}
		/**
		 * iter--
		 */
		const_iterator operator--(int) {
			const_iterator tmp=*this;
			--(*this);
			return tmp;
			
		}
		/**
		 * --iter
		 */
		const_iterator &operator--() {
			if(node==map->list_head->next) throw "invalid";
			node=node->prev;
			return *this;
		}

		/**
		 * if the iter didn't point to a value
		 * throw
		 */
		const value_type &operator*() const {
			if(node==map->list_tail) throw "invalid";
			return *(node->data_ptr);
		}
		const value_type *operator->() const noexcept {
			return node->data_ptr;
		}

		/**
		 * operator to check whether two iterators are same (pointing to the same memory).
		 */
		bool operator==(const iterator &rhs) const {
			return node==rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return node!=rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return node==rhs.node;
		}
		bool operator!=(const const_iterator &rhs) const {
			return node!=rhs.node;
		}
	};

	linked_hashmap() {
		list_head=new Listnode();
		list_tail=new Listnode();
		list_head->next=list_tail;
		list_tail->prev=list_head;
		list_len=0;
	}
	linked_hashmap(const linked_hashmap &other) {
		list_head=new Listnode();
		list_tail=new Listnode();
		list_head->next=list_tail;
		list_tail->prev=list_head;
		list_len=0;
		for(const_iterator it=other.cbegin();it!=other.cend();++it) {
			insert(*it);
		}
	}
	~linked_hashmap() {
		clear();
		delete list_head;
		delete list_tail;
	}
	linked_hashmap &operator=(const linked_hashmap &other) {
		if(this==&other) return *this;
		clear();
		hashmap<Key, T, Hash, Equal>::operator=(other);
		for(const_iterator it=other.cbegin();it!=other.cend();++it) {
			insert(*it);
		}
		return *this;
	}

	/**
	 * return the value connected with the Key(O(1))
	 * if the key not found, throw
	 */
	T &at(const Key &key) {
		iterator it=find(key);
		if(it==end()) throw "invalid";
		return it->second;
	}
	const T &at(const Key &key) const {
		auto it=const_cast<linked_hashmap*>(this)->find(key);
		if(it==end()) throw "invalid";
		return it->second;
	}
	T &operator[](const Key &key) {
		iterator it=find(key);
		if(it==end()) {
            auto result=insert({key, T()});
            return result.first->second;
		}
		return it->second;
	}
	const T &operator[](const Key &key) const {
		return at(key);
	}

	/**
	 * return an iterator point to the first
	 * inserted and existed element
	 */
	iterator begin() {
		return iterator(list_head->next, this);
	}
	const_iterator cbegin() const {
		return const_iterator(list_head->next, this);
	}
	/**
	 * return an iterator after the last inserted element
	 */
	iterator end() {
		return iterator(list_tail, this);
	}
	const_iterator cend() const {
		return const_iterator(list_tail, this);
	}
	/**
	 * if didn't contain anything, return true,
	 * otherwise false.
	 */
	bool empty() const {
		return list_len==0;
	}

	void clear() {
		Listnode *p=list_head->next;
		while(p!=list_tail) {
			Listnode *next=p->next;
			delete p;
			p=next;
		}
		list_head->next=list_tail;
		list_tail->prev=list_head;
		list_len=0;
		hashmap<Key, T, Hash, Equal>::clear();
	}

	size_t size() const {
		return list_len;
	}
	/**
	 * insert the value_piar
	 * if the key of the value_pair exists in the map
	 * update the value instead of adding a new element，
	 * then the order of the element moved from inner of the
	 * list to the head of the list
	 * and return false
	 * if the key of the value_pair doesn't exist in the map
	 * add a new element and return true
	 */
	pair<iterator, bool> insert(const value_type &value) {
		auto hash_result=hashmap<Key, T, Hash, Equal>::insert(value);
		if(hash_result.second){
			Listnode *new_node = new Listnode(const_cast<value_type*>(&(*(hash_result.first))), list_tail->prev, list_tail);
			new_node->next=list_tail;
			list_tail->prev->next=new_node;
			list_tail->prev=new_node;
			list_len++;
			return {iterator(new_node, this), true};
		}
		else{
			return {find(value.first), false};
		}
	}
	/**
	 * erase the value_pair pointed by the iterator
	 * if the iterator points to nothing
	 * throw
	 */
	void remove(iterator pos) {
		if(pos.ptr==list_tail) throw "invalid";
		Listnode *p=pos.ptr;
		hashmap<Key, T, Hash, Equal>::remove(p->data_ptr->first);
		p->prev->next=p->next;
		p->next->prev=p->prev;
		delete p;
		list_len--;
	}
	/**
	 * return how many value_pairs consist of key
	 * this should only return 0 or 1
	 */
	size_t count(const Key &key) const {
		return const_cast<linked_hashmap*>(this)->find(key) != end() ? 1 : 0;
	}
	/**
	 * find the iterator points at the value_pair
	 * which consist of key
	 * if not find, return the iterator
	 * point at nothing
	 */
	iterator find(const Key &key) {
		auto hash_result=hashmap<Key, T, Hash, Equal>::find(key);
		if(hash_result==hashmap<Key, T, Hash, Equal>::end()) return end();
		Listnode *p=list_head->next;
		while(p!=list_tail) {
			if(p->data_ptr==&(*hash_result)) return iterator(p, this);
			p=p->next;
		}
		return end();
	}
};

class lru {
	using lmap = sjtu::linked_hashmap<Integer, Matrix<int>, Hash, Equal>;
	using value_type = sjtu::pair<const Integer, Matrix<int>>;
private:
	lmap *memory;
	int capacity;

public:
	lru(int size) : capacity(size) {
		memory=new lmap();
	}
	~lru() {
		delete memory;
	}
	lru(const lru &other) = delete;
    lru &operator=(const lru &other) = delete;
	/**
	 * save the value_pair in the memory
	 * delete something in the memory if necessary
	 */
	void save(const value_type &v) {
		Integer key=v.first;
		Matrix<int> value=v.second;
		auto it=memory->find(key);
		if(it!=memory->end()) {
			Matrix<int> new_value=value;
			memory->remove(it);
			memory->insert({key, new_value});
		}
		else{
			if(memory->size()>=capacity) {
				if(!memory->empty()){
					auto last_it=memory->end();
					--last_it;
					memory->remove(last_it);
				}
			}
			memory->insert_to_front({key, value});
		}
		
	}
	/**
	 * return a pointer contain the value
	 */
	Matrix<int> *get(const Integer &v)  {
		auto it=memory->find(v);
		if(it==memory->end()) return nullptr;
		Integer key=it->first;
		Matrix<int> val=it->second;
		memory->remove(it);
		memory->insert_to_front({key, val});
		auto new_it=memory->find(key);
		if(new_it!=memory->end()) return &(new_it->second);
		else return nullptr;
	}
	/**
	 * just print everything in the memory
	 * to debug or test.
	 * this operation follows the order, but don't
	 * change the order.
	 */
	void print() {
		for(auto it=memory->begin();it!=memory->end();++it) {
			std::cout<<it->first.val<<" "<<it->second<<std::endl;
		}
	}
};
} // namespace sjtu

#endif
