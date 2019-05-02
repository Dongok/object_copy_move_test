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

class Item {
 public:
  std::string name;

  /**
   * 기본 생성자
   *
   * @param s
   */
  Item(std::string s) : name(s) {
    std::cout << "default construct\n";
  }

  /**
   * 기본 생성자
   */
  Item() {
    std::cout << "default construct\n";
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
    std::cout << "DefaultItem : default construct\n";

    for (int i = 0; i < 1; i++) {
      this->dummy.push_back("abcdef");
    }
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
  std::cout << "TestConstruct:address default1=" << &default1 << "\n";

  DefaultItem defaultCopy = default1;
  std::cout << "TestConstruct:address defaultCopy=" << &defaultCopy << "\n";

  DefaultItem defaultMove = std::move(default1);
  std::cout << "TestConstruct:address defaultMove=" << &defaultMove << "\n";
}

/**
 * 복사, 이동 생성자가 선언된 경우
 */
void TestConstruct2() {
  Item default1;
  std::cout << "TestConstruct2:address default1=" << &default1 << "\n";

  Item defaultCopy = default1;
  std::cout << "TestConstruct2:address defaultCopy=" << &defaultCopy << "\n";

  Item defaultMove = std::move(default1);
  std::cout << "TestConstruct2:address defaultMove=" << &defaultMove << "\n";
}

void TestConstruct3() {
  Item* default1 = new Item();
  std::cout << "TestConstruct3:address default1=" << default1 << "\n";

  Item* defaultCopy = default1;
  std::cout << "TestConstruct3:address defaultCopy=" << defaultCopy << "\n";

  Item* defaultMove = std::move(default1);
  std::cout << "TestConstruct3:address defaultMove=" << defaultMove << "\n";
}

int main(int argc, const char *args[]) {
  int start(number_of_allocs);
  std::cout << "start TestConstruct===============================================\n";
  TestConstruct();

  std::cout << "start TestConstruct2===============================================\n";
  TestConstruct2();

  std::cout << "start TestConstruct3===============================================\n";
  TestConstruct3();

  //TestDefault();
  //TestItemMove()
  // TestItemCopy();
  int end(number_of_allocs);

  std::cout << "Number of Allocs: " << end - start << "\n";
  return 1;
}
