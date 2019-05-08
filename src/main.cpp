#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <new>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <map>

int number_of_allocs = 0;
/**
void *operator new(std::size_t size) throw(std::bad_alloc) {
  std::cout << "malloc new1\n";
  ++number_of_allocs;
  void *p = malloc(size);
  if (!p) throw std::bad_alloc();
  return p;
}

void *operator new[](std::size_t size) throw(std::bad_alloc) {
  std::cout << "malloc new2\n";
  ++number_of_allocs;
  void *p = malloc(size);
  if (!p) throw std::bad_alloc();
  return p;
}

void *operator new[](std::size_t size, const std::nothrow_t &) throw() {
  std::cout << "malloc new3\n";
  ++number_of_allocs;
  return malloc(size);
}
void *operator new(std::size_t size, const std::nothrow_t &) throw() {
  std::cout << "malloc new4\n";
  ++number_of_allocs;
  return malloc(size);
}

void operator delete(void *ptr) throw() { free(ptr); }
void operator delete(void *ptr, const std::nothrow_t &) throw() { free(ptr); }
void operator delete[](void *ptr) throw() { free(ptr); }
void operator delete[](void *ptr, const std::nothrow_t &) throw() { free(ptr); }
**/

class Ent {
  public: 
    std::string name;
    Ent(std::string a):name(a){
    }

    explicit Ent(const Ent* org){
        std::cout << "copy Ent\n";
        this->name = org->name;
    }

    ~Ent(){
      std::cout << "delete Ent\n";
    }

};

class DeItem{
  public:
    Ent* ent;

    DeItem() = delete;
    DeItem(std::string a){
      std::cout << "DeItem create\n";
      this->ent = new Ent(a);
    }
};

class CpItem{
  public:
    Ent* ent;

    CpItem(std::string a){
      std::cout << "cp Item Create\n";
      this->ent = new Ent(a);
    }

    CpItem(const CpItem &src) {
      std::cout << "cp Item CopyCreate\n";
      this->ent = new Ent(src.ent);
    }

    CpItem(CpItem &&src){
      std::cout << "cp Item MoveCreate\n";
      this->ent = src.ent;
    }
};

class Item {
 public:
  std::string name;
  Ent* ent;

  /**
   * 기본 생성자
   *
   * @param s
   */
  Item(std::string s) : name(s) {
    std::cout << "default construct\n";
    this->ent = new Ent("defaultEnt");
  }

  /**
   * 기본 생성자
   */
  Item() {
    std::cout << "default construct\n";
    this->ent = new Ent("1");
    //std::cout << "this->name=" << this->name << "\n";
  };

  /**
   * 생성시 이미 생성된 값을 "=" 으로 Right Value를 참조해서 생성될 때
   * 호출됨
   *
   * Item b
   * Item a = b;
   *
   * @param src
   */
  Item(Item &src) {
    std::cout << "copy construct\n";
    this->ent = src.ent;
    //std::cout << "this->name=" << this->name << "\n";
  }

  /**
   * 생성시 이미 생성된 변수를 std::move 로 받았을 때 호출됨
   *
   * Item b;
   * Item a = std::move(b);
   *
   * @param src
   */
  Item(Item &&src) : name(src.name) {
    std::cout << "move construct\n";
    this->ent = src.ent;
    //std::cout << "this->name=" << this->name << "\n";
  }

  Item &operator=(Item &&src) {
    std::cout << "move operator\n";
    return src;
  }

  Item &operator=(const Item &src) {
    std::cout << "copy operator\n";
    if (this == &src) {
      std::cout << "copy same\n";
      return *this;
    }
    name = src.name;
    return *this;
  }

  Item &operator=(Item &src) {
    std::cout << "a\n";
    return *this;
  }
};

class DefaultItem {
 private:
  std::vector<std::string> dummy;

 public:
  DefaultItem() {
  }
};

Item copyItem() {
  return Item("copy");
}

Item moveItem() {
  return std::move(Item());
}

void TestItemMove() {
  std::chrono::time_point<std::chrono::system_clock> start, end;

  start = std::chrono::system_clock::now();
  for (int i = 0; i < 100; i++) {
    Item x = moveItem();
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "stdmove Item duration1=" << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (int i = 0; i < 1000; i++) {
    Item x = moveItem();
  }
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  std::cout << "stdmove Item duration2=" << elapsed_seconds.count() << "s\n";

}

void TestItemCopy() {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::chrono::duration<double> elapsed_seconds;

  start = std::chrono::system_clock::now();
  for (int i = 0; i < 10; i++) {
    Item x = copyItem();
  }
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;
  std::cout << "get Item duration=1" << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (int i = 0; i < 10; i++) {
    Item x = copyItem();
  }
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;

  std::cout << "get Item duration=2" << elapsed_seconds.count() << "s\n";
}

DefaultItem getD() {
  DefaultItem a;
  return a;
}

DefaultItem getStdMoveD() {
  DefaultItem a;
  return std::move(a);
}

void TestDefault() {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  for (int i = 0; i < 100000; i++) {
    DefaultItem x = getStdMoveD();
  }
  end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "stdmove duration=" << elapsed_seconds.count() << "s\n";

  start = std::chrono::system_clock::now();
  for (int i = 0; i < 100000; i++) {
    DefaultItem x = getD();
  }
  end = std::chrono::system_clock::now();
  elapsed_seconds = end - start;

  std::cout << "getD duration=" << elapsed_seconds.count() << "s\n";
}

void TestCopy() {
  std::string x = "12313123";

  std::cout << "start\n";

  Item a;
  a.name = "original";
  std::cout << "a=" << &a << "\n";

  std::cout << "b = a\n";
  Item b;
  b = a;
  std::cout << "b=" << &b << "\n";
  b.name = "xxxx";
  std::cout << "b.name=" << b.name << "\n";
  std::cout << "a.name=" << a.name << "\n";

  std::cout << "c(a)\n";
  Item c(a);
  std::cout << "c=" << &c << "\n";

  std::cout << "d(a)\n";
  Item d = std::move(a);
  std::cout << "d=" << &d << "\n";
  d.name = "dddd";
  std::cout << "d.name=" << d.name << "\n";
  std::cout << "a.name=" << a.name << "\n";

  std::cout << "e = std::move(a)\n";
  Item e = std::move(a);
  e.name = "eeee";
  std::cout << "e.name=" << e.name << "\n";
  std::cout << "a.name=" << a.name << "\n";
}

/**
* 기본 생성자만 있는 경우 생성 테스트
**/
void TestConstruct() {

  DefaultItem default1;
  std::cout << "TestConstruct:address default1=" << &default1 << "\n\n";

  DefaultItem defaultCopy = default1;
  std::cout << "TestConstruct:address defaultCopy=" << &defaultCopy << "\n\n";

  DefaultItem defaultMove = std::move(default1);
  std::cout << "TestConstruct:address defaultMove=" << &defaultMove << "\n\n";
}

/**
 * 복사, 이동 생성자가 선언된 경우
 */
void TestConstruct2() {
  Item default1;
  std::cout << "TestConstruct2:address default1=" << &default1 << "\n\n";

  Item defaultCopy = default1;
  std::cout << "TestConstruct2:address defaultCopy=" << &defaultCopy << "\n\n";

  Item defaultMove = std::move(default1);
  std::cout << "TestConstruct2:address defaultMove=" << &defaultMove << "\n\n";
}

void TestConstruct3() {
  Item* default1 = new Item();
  std::cout << "TestConstruct3:address default1=" << default1 << "\n\n";

  Item* defaultCopy = default1;
  std::cout << "TestConstruct3:address defaultCopy=" << defaultCopy << "\n\n";

  Item* defaultMove = std::move(default1);
  std::cout << "TestConstruct3:address defaultMove=" << defaultMove << "\n\n";
}

std::map<int , DeItem> MoveReturnDeItemMap(){
  std::map<int , DeItem> ret;
  DeItem a = DeItem("move_name_0");
  ret.insert(std::pair<int,DeItem>(0,a));
  std::cout << "Move Org DeItem Address=" << &ret.at(0) << "\n";
  a.ent->name="move_deItem_change_name";
  //ret.insert(std::pair<int,DeItem>(1,DeItem("move_name_1")));
  //ret.insert(std::pair<int,DeItem>(2,DeItem("name_2")));
  return std::move(ret);
}

std::map<int , DeItem> DefaultReturnDeItemMap(){
  std::map<int , DeItem> ret;
  DeItem a = DeItem("de_name_0");
  ret.insert(std::pair<int,DeItem>(0,a));
  std::cout << "Default Org DeItem Address=" << &ret.at(0) << "\n";
  a.ent->name="deItem_change_name";
  //ret.insert(std::pair<int,DeItem>(1,DeItem("De_name_1")));
  //ret.insert(std::pair<int,DeItem>(2,DeItem("name_2")));
  return ret;
}

std::map<int , CpItem> DefaultReturnCpItemMap(){
  std::map<int , CpItem> ret;
  CpItem a = CpItem("name_0");
  ret.insert(std::pair<int,CpItem>(0,a)); // <- Copy Construct 
  std::cout << "Default Org CpItem Address=" << &ret.at(0) << "\n";
  a.ent->name="default cpItem ChangeName";
  ret.insert(std::pair<int,CpItem>(0,CpItem("CpItem2"))); //<- 일반 생성자가 호출됨
  //CpItem 의 move Construct 발생
  return ret;
}

std::map<int , CpItem> MoveReturnCpItemMap(){
  std::map<int , CpItem> ret;
  CpItem a = CpItem("name_0");
  ret.insert(std::pair<int,CpItem>(0,a));
  std::cout << "Move Org CpItem Address=" << &ret.at(0) << "\n";
  a.ent->name="move cpItem ChangeName";
  ret.insert(std::pair<int,CpItem>(1,CpItem("aaaa")));
  //CpItem 의 move Construct 발생
  return std::move(ret);
}

void MoveReturnCpItemMap1(){
  std::map<int , CpItem> ret;
  ret.insert(std::pair<int,CpItem>(0,CpItem("aaaa")));
  std::cout << "void Move Org CpItem Address=" << &ret.at(0) << "\n";

}

void WhyUseTest2(){
  MoveReturnCpItemMap1();
  std::cout << "\n===================================================\n\n";
  
  std::map<int , DeItem> defaultReturn = DefaultReturnDeItemMap();
  defaultReturn.at(0).ent->name="changeDeName";
  for(std::map<int , DeItem>::iterator deIt = defaultReturn.begin(); deIt != defaultReturn.end(); deIt++ ){
      std::cout << "DefaultDe" << "="; 
      std::cout << deIt->first << ":";
      std::cout << deIt->second.ent->name << "\n";
  }
  std::cout << "\n===================================================\n\n";

  std::map<int , DeItem> moveReturn = MoveReturnDeItemMap();
  for(std::map<int , DeItem>::iterator deIt = moveReturn.begin(); deIt != moveReturn.end(); deIt++ ){
      std::cout << "MoveDe" << "="; 
      std::cout << deIt->first << ":";
      std::cout << deIt->second.ent->name << "\n";
  }
  std::cout << "\n===================================================\n\n";

  std::map<int , CpItem> cpReturn = DefaultReturnCpItemMap();
  for(std::map<int , CpItem>::iterator deIt = cpReturn.begin(); deIt != cpReturn.end(); deIt++ ){
      std::cout << "DefaultCp" << "="; 
      std::cout << &deIt << ","; 
      std::cout << deIt->first << ":";
      std::cout << deIt->second.ent->name << "\n";
  }
  std::cout << "\n===================================================\n\n";

  std::map<int , CpItem> moveCpReturn = MoveReturnCpItemMap();
  // for(std::map<int , CpItem>::iterator deIt = moveCpReturn.begin(); deIt != moveCpReturn.end(); deIt++ ){
  //     std::cout << "MoveCp" << "="; 
  //     std::cout << &deIt << ","; 
  //     std::cout << deIt->first << ":";
  //     std::cout << deIt->second.ent->name << "\n";
  // }
  std::cout << "\n===================================================\n\n";
  std::cout << "map에 넣을때 최초 생성 후 pair를 만들때 move, pair에서 map에 등록할때 move가 발생 , funtion return 에서는 아무것도 발생하지 않는다..\n\n";
}

void WhyUseTest1() {
  DeItem orgDe = DeItem("orgDe");
  std::cout << "orgDe.ent->name=" << orgDe.ent->name << "," << orgDe.ent << "\n";

  DeItem copyDe = orgDe;
  std::cout << "copyDe.ent->name=" << copyDe.ent->name << "," << copyDe.ent << "\n";

  DeItem moveDe = std::move(orgDe);
  std::cout << "moveDe.ent->name=" << moveDe.ent->name << "," << moveDe.ent << "\n";

  orgDe.ent->name="EndOfCopy";  
  std::cout << "orgDe.ent->name의 이름을 변경\n";
  std::cout << "orgDe.ent->name=" << orgDe.ent->name << "," << orgDe.ent << "\n";
  std::cout << "copyDe.ent->name=" << copyDe.ent->name << "," << copyDe.ent << "\n";
  std::cout << "moveDe.ent->name=" << moveDe.ent->name << "," << moveDe.ent << "\n";

  std::cout << "DeItem 은 orgDe의 ent의 name이 다른 변수에도 영향을 미친다.\n";
  CpItem org = CpItem("org");
  std::cout << "org.ent->name=" << org.ent->name << "," << org.ent << "\n\n";

  CpItem copy = org;
  std::cout << "copy.ent->name=" << copy.ent->name << "," << copy.ent << "\n\n";

  CpItem move = std::move(org);
  std::cout << "move.ent->name=" << move.ent->name << "," << move.ent << "\n\n";

  org.ent->name="CpItemCopyEnd";
  std::cout << "CpItem org의 ent->name을 변경\n";

  std::cout << "org.ent->name=" << org.ent->name << "," << org.ent << "\n\n";
  std::cout << "copy.ent->name=" << copy.ent->name << "," << copy.ent << "\n\n";
  std::cout << "move.ent->name=" << move.ent->name << "," << move.ent << "\n\n";

  std::cout << "class의 포인터 멤버는 별도의 생성자를 지정하지 않으면 참조된다..\n\n\n"; 
}

int main(int argc, const char *args[]) {
  int start(number_of_allocs);
  std::cout << "start TestConstruct===============================================\n\n";
  TestConstruct();

  std::cout << "start TestConstruct2===============================================\n\n";
  TestConstruct2();

  std::cout << "start TestConstruct3===============================================\n\n";
  TestConstruct3();


  std::cout << "왜 복사 , 이동을 구별해야 할까? \n";
  WhyUseTest1();
  WhyUseTest2();

  //TestDefault();
  //TestItemMove()
  // TestItemCopy();
  int end(number_of_allocs);
  

  std::cout << "Number of Allocs: " << end - start << "\n";
  return 1;
}
