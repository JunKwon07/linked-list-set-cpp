#include <initializer_list>
#include <iostream>
#include <cassert>

class Set;
class Node;
std::ostream &operator<<( std::ostream &out, Set const &rhs );
 
  /////////////////////////////
 /// Set class declaration ///
/////////////////////////////
class Set {
  public:
    Set( std::initializer_list<int> initial_values );
   ~Set();
    Set( Set const &orig );
    Set( Set      &&orig );
    Set &operator=( Set const &orig );
    Set &operator=( Set      &&orig );
    bool        empty() const;
    std::size_t size()  const;
    void clear();
 
    Node *find( int const &item ) const;
 

    std::size_t insert( int const &item );
 
  
    std::size_t insert( int const array[], std::size_t const begin, std::size_t const end );
 
    std::size_t erase( int const &item );
    std::size_t merge( Set &other );

 
    // Set operations
    Set &operator|=( Set const &other );
    Set &operator&=( Set const &other );
    Set &operator^=( Set const &other );
    Set &operator-=( Set const &other );
 
    Set operator|( Set const &other ) const;
    Set operator&( Set const &other ) const;
    Set operator^( Set const &other ) const;
    Set operator-( Set const &other ) const;
 
    bool operator<=( Set const &other ) const;
    bool operator>=( Set const &other ) const;
    bool operator<( Set const &other ) const;
    bool operator>( Set const &other ) const;
 
    bool operator==( Set const &other ) const;
    bool operator!=( Set const &other ) const;
   
  private:
    Node *p_head_;
    Node *p_tail_;
    std::size_t size_;
 
  friend std::ostream &operator<<( std::ostream &out, Set const &rhs );
};


  ///////////////////////////////
 /// Node class declaration  ///
///////////////////////////////
class Node {
  public:
    Node( int new_value, Node *new_next );
    ~Node();
    int   value() const;
    Node *next()  const;

  private:
    int   value_;
    Node *next_;

  friend class Set;
};

 
  ///////////////////////////////
 /// Node class Definition   ///
///////////////////////////////


// Node constructor
Node::Node( int new_value, Node *new_next ) {
  value_ = new_value;
  next_ = new_next; 
}  
Node::~Node(){
  delete next_; 
}
 
int Node::value() const {
  return value_;
}

Node *Node::next() const {
  return next_;
}


  ///////////////////////////////
 /// Set class definition    ///
///////////////////////////////

// Initializing constructor
Set::Set( std::initializer_list<int> initial_values): p_head_{nullptr}, p_tail_{nullptr}, size_{}{
  for (int const &value : initial_values){
    insert(value); 
  }
}

// Destructor
Set::~Set() {
  clear(); 
}
 
// Copy constructor
Set::Set( Set const &orig ): p_head_{nullptr}, p_tail_{nullptr}, size_{} {
  Node* head{orig.p_head_};
  while(head){
    insert(head->value());
    head = head->next();
  }
}

// Move constructor
Set::Set( Set &&orig ): p_head_{nullptr}, p_tail_{nullptr}, size_{} {
  std::swap(size_, orig.size_);
  std::swap(p_tail_, orig.p_tail_);
  std::swap(p_head_, orig.p_head_);
}
 
// Copy assignment
Set &Set::operator=( Set const &orig ) {
  if (this == &orig){
    return *this;
  }
  clear();
  Node* head{orig.p_head_};
  while (head){
    insert(head->value());
    head = head->next();
  }

  return *this;
}

// Move assignment
Set &Set::operator=( Set &&orig ) {
  clear();
  std::swap (p_head_, orig.p_head_);
  std::swap (p_tail_, orig.p_tail_);
  std::swap (size_, orig.size_);
  return *this;
}
 
// Empty
bool Set::empty() const {
  return p_head_ == nullptr && p_tail_ == nullptr;
}

// Size
size_t Set::size() const {
  return size_;
}
 
 
// Clear
void Set::clear() {
  delete p_head_;
  p_head_ = nullptr; 
  p_tail_ = nullptr;
  size_ = 0;
}
 
// Find
Node *Set::find( int const &item ) const {
  Node* head{p_head_}; 
  if (empty()){
    return nullptr;
  }
  while (head){
    if (head->value() == item){
      return head; 
    } 
    head = head->next();
  }
  return nullptr;
}
 
// Insert the item into the set
std::size_t Set::insert( int const &item ) {
  if (find(item)){
    return 0;
  }

  Node* node{new(std::nothrow) Node(item, nullptr)};
  
  if (empty()){
    p_head_ = node;
    p_tail_ = node;
  }else {
      p_tail_->next_ = node;
      p_tail_ = node;
  } 
  size_++;
  return 1;
}
 
std::size_t Set::insert( int const array[], std::size_t const begin, std::size_t const end ) {
  std::size_t count{};
  for (std::size_t i{begin}; i < end; ++i){
    count += insert(array[i]);
  }
  return count;
}

std::size_t Set::erase( int const &item ) {
  if (empty()){
    return 0;
  }
  Node* head{p_head_};
  Node* prev{nullptr};
  if (head->value() == item){
    if (size_ == 1) {
      clear();
    } else {
      p_head_ = head->next_; 
      --size_;
      head->next_ = nullptr;
      delete head;
    }
    
    return 1;
  }

  while(head){
    if (head->value() == item){
      prev->next_ = head->next();
      --size_;
      head->next_ = nullptr;
      delete head;
      return 1;
    }
    prev = head; 
    head = head->next_;
  }
  return 0;
}

void testerase(){
  Set A{1,2,3,4,3,6};
  std::cout << A.erase(3) << std::endl;
  std::cout << A << std::endl;
}

 
std::size_t Set::merge( Set &other ) {
  std::size_t count{ 0 };
  Node* otherhead{other.p_head_};
  Node* otherprev{nullptr};
  while(otherhead){
    if (!find(otherhead->value())){
      //cut node
      Node* node{otherhead};
      if(!otherprev){
        other.p_head_ = otherhead->next_;
      }else{
        otherprev->next_ = otherhead->next_; 
      }
      otherhead = otherhead->next(); 
      node->next_ = nullptr;
      --other.size_;

      //insert
      if (this->empty()){
        p_head_ = node;
        p_tail_ = node; 
      }else{
        p_tail_->next_ = node; 
        p_tail_ = node;
      }
      ++size_; 
      ++count;
    }else{
      otherprev = otherhead;
      otherhead = otherhead->next();
    }
  }
  return count;
}

  //////////////////////
 /// Set operations ///
//////////////////////
Set &Set::operator|=( Set const &other ) { 
  if (this == &other){
    return *this;
  }
  Node* otherhead{other.p_head_};
  while(otherhead){
    this->insert(otherhead->value());
    otherhead = otherhead->next_;
  }
  return *this;
}

 
Set &Set::operator&=( Set const &other ) {
  if (this == &other){
    return *this;
  }
  if (empty() || other.empty()) {
    clear();
    return *this;
  }
  Node* head(p_head_);
  while (head){
    if(!(other.find(head->value_))){
      Node* node{head->next_};
      erase(head->value_);
      head = node;
    }
    else{head = head->next_;} 
  }
  return *this;
}

Set &Set::operator^=( Set const &other ) {
  if (this == &other){
    clear(); 
    return *this;
  }
  Set C{*this};
  return (*this |= other) -= (C &= other);
}
 
Set &Set::operator-=( Set const &other ) {
  if (this == &other){
    clear();
    return *this;
  }
  Set C{*this};
  C &= other; 
  Node* C_head{C.p_head_};
  while (C_head){
    erase(C_head->value());
    C_head = C_head->next_;
  }
  return *this;
}
 
void testoperator(){
  Set A{1,2,3};
  Set B{2,3,4,5};
   A ^= B;

  std::cout << A << std::endl;
  
}


Set Set::operator|( Set const &other ) const {
  Set result{*this};
  result |= other;
  return result;
}
 
Set Set::operator&( Set const &other ) const {
  Set result{*this};
  result &= other;
  return result;
}
 
Set Set::operator^( Set const &other ) const {
  Set result{*this};
  result ^= other;
  return result;
}
 
Set Set::operator-( Set const &other ) const {
  Set result{*this};
  result -= other;
  return result;
}
 

bool Set::operator>=( Set const &other ) const {
  if (this == &other){
    return true;
  }
  Node* otherhead{other.p_head_};
  while (otherhead){
    if (!find(otherhead->value_)){
      return false;
    }
    otherhead = otherhead->next_;
  }
  return true;
}
 
bool Set::operator<=( Set const &other ) const {
  return other >= *this; 
}
 
bool Set::operator>( Set const &other ) const {
  return this->size_ > other.size_ && *this >= other;
}
 
bool Set::operator<( Set const &other ) const {
  return other > *this;
}
 
bool Set::operator==( Set const &other ) const {
  return *this >= other && *this <= other;
}

bool Set::operator!=( Set const &other ) const {
  return !(*this == other);
}

std::ostream &operator<<( std::ostream &out, Set const &rhs ) {
  out << "{";
  if ( !rhs.empty() ) {
    out << rhs.p_head_->value();   
    for ( Node *ptr{ rhs.p_head_->next() }; ptr != nullptr; ptr = ptr->next() ) {
      out << ", " << ptr->value();  
    }
  }
  out << "}";
 
  return out;
}

