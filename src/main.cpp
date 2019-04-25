#include <iostream>
#include <memory>

class Item {
	public:
		std::string name;
		//default
		Item():name("default"){
			std::cout << "default construct\n";
			std::cout << "this->name=" << this->name << "\n";
		};

		// copy
		Item(Item& src):name("copy"){
			std::cout << "copy construct\n";			
			std::cout << "this->name=" << this->name << "\n";
		}

		Item(Item && src ):name("move"){
			std::cout << "move construct\n";
			std::cout << "this->name=" << this->name << "\n";
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



int main(int argc , const char* args[]){
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
	return 1;
}