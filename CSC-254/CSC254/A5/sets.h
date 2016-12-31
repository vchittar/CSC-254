#include <set>
#include <iostream>
#include <string.h>
#include <type_traits>

// CSC254 A5: Templates
// Yufei (Edward) Du & Vishnu Chittari
// sets.h

using std::set;
using std::cout;
using std::string;

//Normal data types such as int and double
template<typename T>
class comp {
  public:
    bool precedes(const T& a, const T& b) const {
		return a < b;
    }
    bool equals(const T& a, const T& b) const {
		return a == b;
    }
};

//comp class for char* data type
template<>
class comp <char*>{
  public:
    bool precedes(char* a, char* b) const {
        // Code from my own CSC172 C pointer lab
		// Compare the start of the strings, if they are not the same, check if the character in a is less than the character in b. 
		// Otherwise keep looking the next character until one of them ends.
		while (*a != '\0')
		{
			if(*a != *b)
			{
				return (*a - *b)<0;
			}
			a++;
			b++;
		}
		return false;
    }
    bool equals(char* a, char* b) const {
        // Code from my own CSC172 C pointer lab
		// Compare the start of the strings, if they are not the same, return false. 
		// Otherwise keep looking the next character until one of them ends.
		while (*a != '\0')
		{
			if(*a != *b)
			{
				return false;
			}
			a++;
			b++;
		}
		return true;
    }
};

// comp class for string data type because the compiler complained that char* implementation of string
// is deprecated. 
// Mostly identical with the comp class for char*. The only difference is that all pointer representations 
// are now arrays. 
template<>
class comp<string>{
  public:
    bool precedes(string a, string b) const {
		int i = 0;
		int	j = 0;
		while (a[i] != '\0')
		{
			if(a[i] != b[i])
			{
				return (a[i] - b[i])<0;
			}
			i++;
			j++;
		}
		return false;
    }
    bool equals(string a, string b) const {
		int i = 0;
		int	j = 0;
		while (a[i] != '\0')
		{
			if(a[i] != b[i])
			{
				return false;
			}
			i++;
			j++;
		}
		return true;
    }
};

// Abstract base class from which all sets are derived.
//
template<typename T, typename C = comp<T> >
class simple_set {
  public:
    virtual ~simple_set<T, C>() { }
        // destructor should be virtual so that we call the right
        // version when saying, e.g.,
        // simple_set* S = new derived_set(args);
        //  ...
        // delete S;
    virtual simple_set<T, C>& operator+=(const T item) = 0;
        // inserts item into set
        // returns a ref so you can say, e.g.
        // S += a += b += c;
    virtual simple_set<T, C>& operator-=(const T item) = 0;
        // removes item from set, if it was there (otherwise does nothing)
    virtual bool contains(const T& item) const = 0;
        // indicates whether item is in set
};

//---------------------------------------------------------------

// Example of a set that implements the simple_set interface.
// Borrows the balanced tree implementation of the standard template
// library.  Note that you are NOT to use any standard library
// collections in your code (though you may use strings and streams).
//
template<typename T>
class std_simple_set : public virtual simple_set<T>, protected set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
  public:
    virtual ~std_simple_set<T>() { }  // will invoke std::~set<T>()
    virtual std_simple_set<T>& operator+=(const T item) {
        set<T>::insert(item);
        return *this;
    }
    virtual std_simple_set<T>& operator-=(const T item) {
        (void) set<T>::erase(item);
        return *this;
    }
    virtual bool contains(const T& item) const {
        return (set<T>::find(item) != set<T>::end());
    }
};

//---------------------------------------------------------------

// Characteristic array implementation of set.
// Requires instantiation with guaranteed low and one-more-than-high
// bounds on elements that can be placed in the set.  Should compile
// and run correctly for any element class T that can be cast to int.
// Throws out_of_bounds exception when appropriate.
//
class out_of_bounds { };    // exception
template<typename T>
class carray_simple_set : public virtual simple_set<T> {
  private:
    // Lowest bound, highest bound
	int low;
    int high;
	// The main table
	bool *data;
  public:
    carray_simple_set(const T l, const T h) {   // constructor
		low = l;
		high = h;
		data = new bool[high - low];
    }
    virtual ~carray_simple_set() {              // destructor
		delete [] data;
    }
    virtual carray_simple_set<T>& operator+=(const T item) {
        // if item is less than lowest bound or if it is higher than highest bound, throw out_of_bounds
		if(item-low < 0 || high-item <=0){
			out_of_bounds();
			return *this;
		}else{
			data[((int)item)-low] = true;
		}
		 
		return *this;
    }
    virtual carray_simple_set<T>& operator-=(const T item) {
        // if item is less than lowest bound or if it is higher than highest bound, throw out_of_bounds
		if(item-low < 0 || high-item <=0){
			out_of_bounds();
			return *this;
		}else{
			data[((int)item)-low] = false;
		}
		return *this;
    }
    virtual bool contains(const T& item) const {
        // if item is less than lowest bound or if it is higher than highest bound, throw out_of_bounds
		if(item-low < 0 || high-item <=0){
			out_of_bounds();
			return false;
		}else{
			return data[((int)item)-low];
		}
    }
};

//---------------------------------------------------------------

// Naive hash function object.
// Provides a default for any type that can be cast to int.
//
template<typename T>
class cast_to_int {
  public:
    int operator()(const T n) {
        return (int) n;
    }
};

// Hash table implementation of set.
// Requires instantiation with guaranteed upper bound on number of elements
// that may be placed in set.  Throws overflow if bound is exceeded.
// Can be instantiated without second generic parameter if element type
// can be cast to int; otherwise requires hash function object.
//
class overflow { };         // exception
template<typename T, typename F = cast_to_int<T> >
class hashed_simple_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
    // You'll need some data members here.
    // I recommend you pick a hash table size p that is a prime
    // number >= n, use F(e) % p as your hash function, and rehash
    // with kF(e) % p after the kth collision.  (But make sure that
    // F(e) is never 0.)
	
	// Mian table
	int *hash;
	// Boolean array to represent if hash[i] is occupied or not
	bool *occupied;
	// Size
	int size;
  public:
    hashed_simple_set(const int n) {    // constructor
        // Set size and initialize arrays
		size = n;
		hash = new int[n];
		occupied = new bool[n];
    }
    virtual ~hashed_simple_set() { 
		//For some reasons size cannot be deleted.
		delete [] hash;
		delete [] occupied;
	}    // destructor
    virtual hashed_simple_set<T, F>& operator+=(const T item) {
		int itemvalue = F()(item);
		//Hash function: Knuth Variant on Division
		//Cite: https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
		int key = (itemvalue * (itemvalue + 3)) % size;
		int origkey = key;
		//Collision. Used linear probing.
		while(occupied[key]){
			if(occupied[key] && hash[key] == itemvalue){
				
				return *this;
			}
			key++;
			if(key == origkey){
				//Table is full
				overflow();
				
				return *this;
			}
			if(key == size)
				key = 0;
		}
		occupied[key] = true;
		hash[key] = itemvalue;
		return *this;
    }
    virtual hashed_simple_set<T, F>& operator-=(const T item) {
         
		int itemvalue = F()(item);
		//Hash function: Knuth Variant on Division
		//Cite: https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
		int key = (itemvalue * (itemvalue + 3)) % size;
		int origkey = key;
		//Collision. Used linear probing.
		while(occupied[key]){
			if(occupied[key] && hash[key] == itemvalue){
				occupied[key] = false;
				return *this;
			}
			key++;
			if(key == origkey){
				//Not found
				return *this;
			}
			if(key == size)
				key = 0;
		}
		return *this;
    }
    virtual bool contains(const T& item) const {
		int itemvalue = item;
		//Hash function: Knuth Variant on Division
		//Cite: https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
		int key = (itemvalue * (itemvalue + 3)) % size;
		int origkey = key;
		//Collision or original element before collision is deleted. Used linear probing.
		while(!occupied[key] || hash[key] != item){
			if(occupied[key] && hash[key] == itemvalue){
				return true;
			}
			key++;
			if(key == origkey){
				//Not found and table is full
				return false;
			}
			if(key == size)
				key = 0;
		}
		//Not found
		return true;
    }
};

//---------------------------------------------------------------

// Sorted array implementation of set; supports binary search.
// Requires instantiation with guaranteed upper bound on number of
// elements that may be placed in set.  Throws overflow if bound is
// exceeded.
//
template<typename T, typename C = comp<T> >
class bin_search_simple_set : public virtual simple_set<T> {
	// Size
    int storage;
	// Current number of elements
	int itemNum;
	// Main table
	T *table;
	// Comparator
	C comparator;
	// relation: Only used in range_set. Contains inclusiveness data of each element in table
	bool *relation;
  private:
    //Helper method for contains()
	//Recursively lookup the element
	virtual bool lookup_rec(T item, int low, int high) const{
		int i = ((high-low)/2)+low;
		if(comparator.equals(item, table[i]))
			return true;
		if(high-low == 0)
			return false;
		if(high-i == 1)
			return lookup_rec(item, high, high);
		if(comparator.precedes(item, table[i])){
			return lookup_rec(item, low, i);
		}else{
			return lookup_rec(item, i, high);
		}
	}
	//Helper method for inserting low or high bound in the range
	//Only used with range_set
	virtual void insert_range(T item, bool inclusive){
		//Table is full
		if(itemNum == storage){
			overflow();
			return;
		}
		//Table is empty
		if(itemNum == 0){
			table[0] = item;
			relation[0] = inclusive;
			itemNum++;
			return;
		}
		//Locate where it needs to be inserted
		int i = itemNum-1;
		bool modify = false;
		while(comparator.precedes(item, table[i])){
			i--;
			if(i == -1){
				int j = itemNum-1;
				while(j > i){
					table[j+1]=table[j];
					relation[j+1] = relation[j];
					j--;
				}
				table[i+1] = item;
				relation[i+1] = inclusive;
				itemNum++;
				return;
			}
			modify = true;
		}
		if(modify){
			int j = itemNum-1;
			while(j > i){
				table[j+1]=table[j];
				relation[j+1] = relation[j];
				j--;
			}
		}
		table[i+1] = item;
		relation[i+1] = inclusive;
		itemNum++;
		return;
	}
	
    // 'virtual' on simple_set ensures single copy if multiply inherited
    // You'll need some data members here.
  public:
	bin_search_simple_set(int range){
		storage = range;
		itemNum = 0;
		table = new T[storage];
		relation = new bool[storage];
	}
	virtual ~bin_search_simple_set() { 
		//delete &size;
		delete [] table;
		delete [] relation;
	}    // destructor
	virtual bin_search_simple_set<T,C>& operator+=(const T item) { //Table is full
		if(itemNum == storage){
			overflow();
			return *this;
		}
		if(itemNum == 0){
			table[0] = item;
			itemNum++;
			return *this;
		}
        //See where it needs to be inserted
		int i = itemNum-1;
		bool modify = false;
		while(comparator.precedes(item, table[i])){
			i--;
			if(i == -1){
				int j = itemNum-1;
				while(j > i){
					table[j+1]=table[j];
					j--;
				}
				table[i+1] = item;
				itemNum++;
				return *this;
			}
			modify = true;
		}
		if(comparator.equals(item, table[i])){
			return *this;
		}
		if(modify){
			int j = itemNum-1;
			while(j > i){
				table[j+1]=table[j];
				j--;
			}
		}
		table[i+1] = item;
		itemNum++;
		return *this;
	}

    virtual bin_search_simple_set<T,C>& operator-=(const T item) {
        int i = itemNum-1;
		bool modify = false;
		while(comparator.precedes(item, table[i])){
			i--;
			modify = true;
		}
		if(!comparator.equals(item, table[i])){
			return *this;
		}
		if(modify){
			while(i+1 < itemNum){
				table[i]=table[i+1];
				i++;
			}
		}
		itemNum--;
		return *this;
    }
	virtual bool contains(const T& item) const {
		return lookup_rec(item, 0, itemNum-1);
	}
	virtual void insertRange(T low, bool lowinclusive, T high, bool highinclusive){
        //takes the lows and highs of the subrange and stores it as ints
		int rangelow = subrange(low);
		int rangehigh = subrange(high);
        //if we have the same number for range then they are included already
		if(rangelow == rangehigh && rangelow != -1)
			return;
        //if lower bound is not in the range but higher bound is in the range
		else if(rangelow == -1 && rangehigh != -1){
            //Get which "gap" this low is. If -1 then it means that it is lower than any range
			int gaplow = gap(low);
            //if low is right next to the range that high is in, just replace the lower bound of the range that high is in with low as the new lower bound.
			if((gaplow+1) == rangehigh){
				table[rangehigh] = low;
				relation[rangehigh] = lowinclusive;
			}else{
                //change the lower bound of the range right above low to low and get rid of everything between
				//this lower bound and the higher bound of the range that high is in.
				//If low is lower than the lowest range, then gaplow+1 will be 0 which is the lower bound of the lowest range.
				table[gaplow+1] = low;
				relation[gaplow+1] = lowinclusive;
				int offset = rangehigh - (gaplow+1);
				rangehigh++;
				while(rangehigh < itemNum){
					table[rangehigh-offset]=table[rangehigh];
					relation[rangehigh-offset]=relation[rangehigh];
					rangehigh++;
				}
				itemNum-=offset;
			}
        //Similar logic for the situation if low is in a range and high is not. 
		}else if(rangelow != -1 && rangehigh == -1){
			int gaphigh = gap(high);
			if(rangelow == gaphigh - 1){
				table[rangelow+1] = high;
				relation[rangelow+1] = highinclusive;
			}else{
				table[gaphigh] = high;
				relation[gaphigh] = highinclusive;
				int offset = gaphigh - (rangelow+1);
				while(gaphigh < itemNum){
					table[gaphigh-offset]=table[gaphigh];
					relation[gaphigh-offset]=relation[gaphigh];
					gaphigh++;
				}
				itemNum-=offset;
			}
		}
		//If neither of them is in a range
		else if(rangelow == -1 && rangehigh == -1){
			int gaplow = gap(low);
			int gaphigh = gap(high);
			if(gaplow == gaphigh){
				insert_range(low, lowinclusive);
				insert_range(high, highinclusive);
			}else{
				table[gaplow+1] = low;
				relation[gaplow+1] = lowinclusive;
				table[gaphigh] = high;
				relation[gaphigh] = highinclusive;
				int offset = gaphigh - (gaplow+1);
				if(gaphigh-offset != gaphigh){
					while(gaphigh < itemNum){
						table[gaphigh-offset]=table[gaphigh];
						relation[gaphigh-offset]=relation[gaphigh];
						gaphigh++;
					}
					itemNum-=offset;
				}
			}
		}//If both of them are in a range but not the same range
		else{
			int offset = rangehigh - rangelow;
			while(rangehigh+1 < itemNum){
				table[rangehigh-offset+1]=table[rangehigh+1];
				relation[rangehigh-offset+1]=relation[rangehigh+1];
				rangehigh++;
			}
			itemNum-=offset;
		}
	}
	
	virtual void removeRange(T low, bool lowinclusive, T high, bool highinclusive){
		int rangelow = subrange(low);
		int rangehigh = subrange(high);
        //the insert logic is redone in remove range fashion. All inclusiveness are reversed so that they are properly removed.
        //If they are in the same range then we just add low as the higher bound and high as the next lower bound, and inclusiveness are reversed. 
		if(rangelow == rangehigh && rangelow != -1)
		{
			insert_range(low, !lowinclusive);
			insert_range(high, !highinclusive);
			return;
		}
		else if(rangelow == -1 && rangehigh != -1){
            //set gaplow as an int if the previous conditions happen
			int gaplow = gap(low);
            //if the next element in gaplow is the same as rangehigh then that is the new low
			if((gaplow+1) == rangehigh){
				table[rangehigh] = high;
				relation[rangehigh] = !highinclusive;
			}else{
                //the new high in the range become the overall high
				table[rangehigh] = high;
				relation[rangehigh] = !lowinclusive;
				int offset = rangehigh - (gaplow+1);
				while(rangehigh < itemNum){
					table[rangehigh-offset]=table[rangehigh];
					relation[rangehigh-offset]=relation[rangehigh];
					rangehigh++;
				}
				itemNum-=offset;
			}
		}else if(rangelow != -1 && rangehigh == -1){
			int gaphigh = gap(high);
			if(rangelow == gaphigh - 1){
				table[gaphigh] = low;
				relation[gaphigh] = !lowinclusive;
			}else{
				table[rangelow+1] = low;
				relation[rangelow+1] = !lowinclusive;
				
				if(gaphigh == -1){
					itemNum = rangelow+2;
					return;
				}
				int offset = gaphigh - (rangelow+1);
				gaphigh++;
				while(gaphigh < itemNum){
					table[gaphigh-offset]=table[gaphigh];
					gaphigh++;
				}
				itemNum-=offset;
			}
		}else if(rangelow == -1 && rangehigh == -1){
			int gaplow = gap(low);
			int gaphigh = gap(high);
			if(gaplow == -1){
				if(gaphigh == -1){
					itemNum = 0;
					return;
				}else{
					int counter = 0;
					gaphigh++;
					while(gaphigh < itemNum){
						table[counter] = table[gaphigh];
						gaphigh++;
						counter++;
					}
					itemNum = counter;
					return;
					
				}
			}
			if(gaphigh == -1){
				itemNum = gaplow+1;
				return;
			}
			if(gaplow == gaphigh){
				return;
			}else{
				
				int offset = gaphigh - (gaplow);
				gaphigh++;
				while(gaphigh < itemNum){
					table[gaphigh-offset]=table[gaphigh];
					gaphigh++;
				}
				itemNum-=offset;
				
			}
		}else{
			table[rangelow+1] = low;
			relation[rangelow+1] = !lowinclusive;
			table[rangehigh] = high;
			relation[rangehigh] = !highinclusive;
			int offset = rangehigh - (rangelow+2);
			while(rangehigh < itemNum){
				table[rangehigh-offset]=table[rangehigh];
				rangehigh++;
			}
			itemNum-=offset;
		}
	}
	

	virtual bool included(T item) const{
        //dont include dupes in the same range
		return subrange(item) != -1;
	}
	
	virtual int subrange(T item) const{
        //Returns the index of the lower bound of the range item is in
		//Returns -1 if item is not in any range
		int i = 0;
		while(i+1 < itemNum){
			//cout<<"LowData = "<<table[i]<<" and relation = "<<relation[i]<<"\n";
			//cout<<"HighData = "<<table[i+1]<<" and relation = "<<relation[i+1]<<"\n";
			if(comparator.precedes(table[i], item) || (comparator.equals(item, table[i]) && relation[i])){
				//cout<<"Passed. Data = "<<table[i]<<" and relation = "<<relation[i]<<"\n";
				if(comparator.precedes(item, table[i+1])){
					return i;
				}else if(relation[i+1]&&comparator.equals(item, table[i+1])){
					return i;
				}
			}
			i+=2;
		}
		return -1;
	}
	
	virtual int gap(T item) const{
		//Pretty much the same as subrange but returns the index of the higher bound of the range that is right lower than item. Only called if subrange returns -1.
		//Returns -1 if item is either smaller than the current smallest range or larger than the current largest range
		int i = 1;
		while(i+1 < itemNum){
			if(comparator.precedes(table[i], item) || (comparator.equals(item, table[i]) && relation[i])){
				if(comparator.precedes(item, table[i+1])){
					return i;
				}else if(relation[i+1]&&comparator.equals(item, table[i+1])){
					return i;
				}
			}
			i+=2;
		}
		return -1;
	}
};

//===============================================================
// RANGE SETS

// Function object for incrementing.
// Provides a default for any integral type.
//
template<typename T>
class increment {
    //static_assert(std::is_integral<T>::value, "Integral type required.");
  public:
    T operator()(T a) const {
        return ++a;
    }
};

// Range type.  Uses comp<T> by default, but you can provide your
// own replacement if you want, e.g. for C strings.
//
class empty_range { };      // exception
template<typename T, typename C = comp<T> >
class range {
    const T L;      // represents all elements from L
    const bool Lc;  // inclusive?
    const T H;      // through H
    const bool Hc;  // inclusive?
    const C cmp;    // can't be static; needs explicit instantiation
    static const empty_range err;
  public:
    range(const T l, const bool lc, const T h, const bool hc)
            : L(l), Lc(lc), H(h), Hc(hc), cmp() {
        if (cmp.precedes(h, l) || (cmp.equals(l, h) && (!Lc || !Hc))) throw err;
    }
    // no destructor needed
    T low() const { return L; }
    bool closed_low() const { return Lc; }
    T high() const { return H; }
    bool closed_high() const {return Hc; }
    bool contains(const T& item) const {
        return ((cmp.precedes(L, item) || (Lc && cmp.equals(L, item)))
            && (cmp.precedes(item, H) || (Hc && cmp.equals(item, H))));
    }
    // You may also find it useful to define the following:
    // bool precedes(const range<T, C>& other) const { ...
    // bool overlaps(const range<T, C>& other) const { ...
};

// You may find it useful to define derived types with two-argument
// constructors that embody the four possible combinations of open and
// close-ended:
//
// template<typename T, typename C = comp<T>>
// class CCrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class COrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class OCrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class OOrange : public range<T, C> { ...

// This is the abstract class from which all range-supporting sets are derived.
//
template<typename T, typename C = comp<T> >
class range_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
  public:
    virtual range_set<T,C>& operator+=(const range<T, C> r) = 0;
    virtual range_set<T,C>& operator-=(const range<T, C> r) = 0;
};

//---------------------------------------------------------------

// As implemented in the standard library, sets contain individual
// elements, not ranges.  (There are range insert and erase operators, but
// (a) they use iterators, (b) they take time proportional to the number of
// elements in the range, and (c) they require, for deletion, that the
// endpoints of the range actually be in the set.  An std_range_set, as
// defined here, avoids shortcomings (a) and (c), but not (b).  Your
// bin_search_range_set should avoid (b), though it will have slow insert
// and remove operations.  A tree_range_set (search tree -- extra credit)
// would have amortized log-time insert and remove for individual elements
// _and_ ranges.
//
template<typename T, typename C = comp<T>, typename I = increment<T> >
class std_range_set : public virtual range_set<T, C>, public std_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    static_assert(std::is_integral<T>::value, "Integral type required.");
    I inc;
  public:
    // The first three methods below tell the compiler to use the
    // versions of the simple_set methods already found in std_simple_set
    // (given true multiple inheritance it can't be sure it should do that
    // unless we tell it).
    virtual std_simple_set<T>& operator+=(const T item) {
        return std_simple_set<T>::operator+=(item);
    }
    virtual std_simple_set<T>& operator-=(const T item) {
        return std_simple_set<T>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return std_simple_set<T>::contains(item);
    }
    virtual range_set<T>& operator+=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};

//---------------------------------------------------------------

// insert an appropriate carray_range_set declaration here
template<typename T, typename C = comp<T>, typename I = increment<T> >
class carray_range_set : public virtual range_set<T, C>, public carray_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    //static_assert(std::is_integral<T>::value, "Integral type required.");
    I inc;
  public:
    // The first three methods below tell the compiler to use the
    // versions of the simple_set methods already found in carray_simple_set
    // (given true multiple inheritance it can't be sure it should do that
    // unless we tell it).
	carray_range_set(const T l, const T h) : carray_simple_set<T>(l,h){   // constructor
        
    }
	virtual ~carray_range_set() {              // destructor
		//delete &inc;
    }
    virtual carray_simple_set<T>& operator+=(const T item) {
        return carray_simple_set<T>::operator+=(item);
    }
    virtual carray_simple_set<T>& operator-=(const T item) {
        return carray_simple_set<T>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return carray_simple_set<T>::contains(item);
    }
    virtual range_set<T,C>& operator+=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T,C>& operator-=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};
//---------------------------------------------------------------

// insert an appropriate hashed_range_set declaration here
template<typename T, typename C = comp<T>, typename I = increment<T> >
class hashed_range_set : public virtual range_set<T, C>, public hashed_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    //static_assert(std::is_integral<T>::value, "Integral type required.");
    I inc;
  public:
    // The first three methods below tell the compiler to use the
    // versions of the simple_set methods already found in hashed_simple_set
    // (given true multiple inheritance it can't be sure it should do that
    // unless we tell it).
	hashed_range_set(int size) : hashed_simple_set<T>(size){   // constructor
        // replace this line:
        //(void) l;  (void) h;
    }
	virtual ~hashed_range_set() {              // destructor
		//delete &inc;
    }
    virtual hashed_simple_set<T>& operator+=(const T item) {
        return hashed_simple_set<T>::operator+=(item);
    }
    virtual hashed_simple_set<T>& operator-=(const T item) {
        return hashed_simple_set<T>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return hashed_simple_set<T>::contains(item);
    }
    virtual range_set<T,C>& operator+=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }
    virtual range_set<T,C>& operator-=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};
//---------------------------------------------------------------

// insert an appropriate bin_search_range_set declaration here
template<typename T, typename C = comp<T> >
class bin_search_range_set : public virtual range_set<T, C>, public bin_search_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    //static_assert(std::is_integral<T>::value, "Integral type required.");
  public:
    // The first three methods below tell the compiler to use the
    // versions of the simple_set methods already found in bin_search_simple_set
    // (given true multiple inheritance it can't be sure it should do that
    // unless we tell it).
	bin_search_range_set(int size) : bin_search_simple_set<T>(size){   // constructor
        // replace this line:
        //(void) l;  (void) h;
    }
	virtual ~bin_search_range_set() {              // destructor
    }
    virtual bin_search_simple_set<T>& operator+=(const T item) {
		bin_search_simple_set<T>::removeRange(item, true, item, true);
        return *this;
    }
    virtual bin_search_simple_set<T>& operator-=(const T item) {
        return bin_search_simple_set<T>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return bin_search_simple_set<T>::included(item);
    }
    virtual range_set<T,C>& operator+=(const range<T, C> r) {
		bin_search_simple_set<T>::insertRange(r.low(), r.closed_low(), r.high(), r.closed_high());
        return *this;
    }
    virtual range_set<T,C>& operator-=(const range<T, C> r) {
        bin_search_simple_set<T>::removeRange(r.low(), r.closed_low(), r.high(), r.closed_high());
        return *this;
    }
};
//===============================================================

// comparator for C strings
//
class lexico_less {
  public:
    bool precedes(const char *a, const char *b) const {
        return strcmp(a, b) < 0;
    }
    bool equals(const char *a, const char *b) const {
        return strcmp(a, b) == 0;
    }
};

typedef enum{mon, tue, wed, thu, fri} weekday;