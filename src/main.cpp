#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <new>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>

int number_of_allocs = 0;

void* operator new(std::size_t size) throw(std::bad_alloc) {
  ++number_of_allocs;
  void *p = malloc(size);
  if(!p) throw std::bad_alloc();
  return p;
}

void* operator new  [](std::size_t size) throw(std::bad_alloc) {
  ++number_of_allocs;
  void *p = malloc(size);
  if(!p) throw std::bad_alloc();
  return p;
}

void* operator new  [](std::size_t size, const std::nothrow_t&) throw() {
  ++number_of_allocs;
  return malloc(size);
}
void* operator new   (std::size_t size, const std::nothrow_t&) throw() {
  ++number_of_allocs;
  return malloc(size);
}


void operator delete(void* ptr) throw() { free(ptr); }
void operator delete (void* ptr, const std::nothrow_t&) throw() { free(ptr); }
void operator delete[](void* ptr) throw() { free(ptr); }
void operator delete[](void* ptr, const std::nothrow_t&) throw() { free(ptr); }

class Item {
	public:
		std::string name;
		//default
		Item(std::string s):name(s){
			std::cout << "default construct\n";
		}

		Item(){
			std::cout << "default construct\n";
			//std::cout << "this->name=" << this->name << "\n";
		};

                
		// copy
		Item(Item& src){
			std::cout << "copy construct\n";			
			//std::cout << "this->name=" << this->name << "\n";
		}

		Item(Item && src ):name(src.name){
			//std::cout << "move construct\n";
			//std::cout << "this->name=" << this->name << "\n";
		}

		Item& operator=(Item&& src){
			std::cout << "move operator\n";
			return src;
		}

		Item& operator=(const Item& src){
			std::cout << "copy operator\n";
			if(this == &src ){
				std::cout << "copy same\n";
				return *this;
			}
			name = src.name;
			return *this;
		}

		Item& operator=(Item& src){
		std::cout << "a\n";
			return *this;
		}
};

class DefaultItem {
    private:
        std::vector<std::string> dummy;

    public:
        DefaultItem(){
            for(int i =0; i < 1; i++){
                this->dummy.push_back("abcdef");
            }
        }
};


Item copyItem(){
    return Item("copy");
}

Item moveItem(){
    return std::move(Item());
}


void TestItemMove(){
    std::chrono::time_point<std::chrono::system_clock> start, end; 

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 100; i++){
        Item x = moveItem();
    }
    end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end - start; 
    std::cout << "stdmove Item duration1=" << elapsed_seconds.count()  << "s\n";

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 1000; i++){
        Item x = moveItem();
    }
    end = std::chrono::system_clock::now(); 
    elapsed_seconds = end - start; 
    std::cout << "stdmove Item duration2=" << elapsed_seconds.count()  << "s\n";

}

void TestItemCopy(){
    std::chrono::time_point<std::chrono::system_clock> start, end; 
    std::chrono::duration<double> elapsed_seconds; 

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 10; i++){
        Item x = copyItem();
    }
    end = std::chrono::system_clock::now(); 
    elapsed_seconds = end - start; 
    std::cout << "get Item duration=1" << elapsed_seconds.count()  << "s\n";

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 10; i++){
        Item x = copyItem();
    }
    end = std::chrono::system_clock::now(); 
    elapsed_seconds = end - start; 

    std::cout << "get Item duration=2" << elapsed_seconds.count()  << "s\n";
}

DefaultItem getD(){
    DefaultItem a;
    return a;
}

DefaultItem getStdMoveD(){
    DefaultItem a;
    return std::move(a);
}

void TestDefault(){
    std::chrono::time_point<std::chrono::system_clock> start, end; 
    start = std::chrono::system_clock::now(); 

    for(int i =0; i < 100000; i++){
        DefaultItem x = getStdMoveD();
    }

    end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end - start; 

    std::cout << "stdmove duration=" << elapsed_seconds.count()  << "s\n";

    start = std::chrono::system_clock::now(); 


    for(int i =0; i < 100000; i++){
        DefaultItem x = getD();
    }

    end = std::chrono::system_clock::now(); 
    elapsed_seconds = end - start; 

    std::cout << "getD duration=" << elapsed_seconds.count()  << "s\n";
}


void TestCopy(){
        std::string x="12313123";

	std::cout << "start\n";

	Item a;
	a.name = "original";
	std::cout << "a=" << &a <<"\n";

	std::cout << "b = a\n";
	Item b ;
	b = a;
	std::cout << "b=" << &b <<"\n";
	b.name = "xxxx";
	std::cout << "b.name=" << b.name <<"\n";
	std::cout << "a.name=" << a.name <<"\n"; 

	std::cout << "c(a)\n";
	Item c(a);
	std::cout << "c=" << &c <<"\n";

	std::cout << "d(a)\n";
	Item d = std::move(a);
	std::cout << "d=" << &d <<"\n";
	d.name ="dddd";
	std::cout << "d.name=" << d.name <<"\n";
	std::cout << "a.name=" << a.name <<"\n"; 


	std::cout << "e = std::move(a)\n";
	Item e = std::move(a);
	e.name ="eeee";
	std::cout << "e.name=" << e.name <<"\n";
	std::cout << "a.name=" << a.name <<"\n";
}

int main(int argc , const char* args[]){
 int start(number_of_allocs);

        //TestDefault();
        //TestItemMove();
        TestItemCopy();
  int end(number_of_allocs);

  std::cout << "Number of Allocs: " << end-start << "\n";
	return 1;
}
