#include <iostream>
#include <memory>
#include <chrono>
#include <vector>


class Item {
	public:
		std::string name;
		//default
		Item():name("default"){
			//std::cout << "default construct\n";
			//std::cout << "this->name=" << this->name << "\n";
		};

                
		// copy
		Item(Item& src):name("copy"){
			//std::cout << "copy construct\n";			
			//std::cout << "this->name=" << this->name << "\n";
		}

		Item(Item && src ):name("move"){
			//std::cout << "move construct\n";
			//std::cout << "this->name=" << this->name << "\n";
		}

		Item& operator=(Item&& src){
			std::cout << "move operator\n";
			name = std::move(src.name);
			return *this;
		}

		Item& operator=(const Item& src){
			std::cout << "copy operator\n";
			if(this == &src ){
				return *this;
			}
			name = std::move(src.name);
			return *this;
		}
};

class DefaultItem {
    private:
        std::vector<std::string> dummy;

    public:
        DefaultItem(){
            for(int i =0; i < 100; i++){
                this->dummy.push_back("abcdef");
            }
        }
};


Item copyItem(){
    return Item();
}

Item moveItem(){
    return std::move(Item());
}


void TestItemMove(){
    std::chrono::time_point<std::chrono::system_clock> start, end; 

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 1000000; i++){
        Item x = moveItem();
    }
    end = std::chrono::system_clock::now(); 
    std::chrono::duration<double> elapsed_seconds = end - start; 
    std::cout << "stdmove Item duration1=" << elapsed_seconds.count()  << "s\n";

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 1000000; i++){
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
    for(int i =0; i < 1000000; i++){
        Item x = copyItem();
    }
    end = std::chrono::system_clock::now(); 
    elapsed_seconds = end - start; 
    std::cout << "get Item duration=1" << elapsed_seconds.count()  << "s\n";

    start = std::chrono::system_clock::now(); 
    for(int i =0; i < 1000000; i++){
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
        TestDefault();
        TestItemMove();
        TestItemCopy();
	return 1;
}
