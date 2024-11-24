#include <iostream>
#include <vector>
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

class MyObj {
public:
  MyObj(): value(0) { std::cout << "MyObj::MyObj()" << std::endl; }
  MyObj(int value): value(value) { std::cout << "MyObj::MyObj(" << value << ")" << std::endl; }
  MyObj(MyObj && obj)
  : value(obj.value)
  {
    std::cout << "MyObj::MyObj(MyObj &&): value = " << obj.value << std::endl;
  }
  MyObj & operator=(MyObj && obj)
  {
    value = obj.value;
    std::cout << "MyObj::operator=(MyObj &&): value = " << obj.value << std::endl;
    return *this;
  }

  MyObj(MyObj const &) = delete;
  MyObj & operator=(MyObj const &) = delete;

  ~MyObj() { std::cout << "MyObj::~MyObj():value=" << value << std::endl; }
  int getValue() const { return value; }
  int value;
};

class MyContainer {
public:
  MyContainer() = default;
  ~MyContainer() = default;
  MyContainer(MyContainer const &) = delete;
  MyContainer & operator=(MyContainer const &) = delete;
  MyContainer(MyContainer &&) = default;
  MyContainer & operator=(MyContainer &&) = default;

  void addObject(int value) {
    objects.emplace_back(value);
  }

  MyObj const * getObj(std::size_t index) const {
    if (index < objects.size()) { return &objects[index]; }
    else { return nullptr; }
  }

  std::vector<MyObj const *> getObjects() const {
    std::vector<MyObj const *> out; out.reserve(objects.size());
    for (auto const & item: objects) {
      out.push_back(&item);
    }
    return out;
  }

  static MyContainer & GetInstance() {
    static MyContainer container;
    return container;
  }

  std::vector<MyObj> objects;
};

namespace nb = nanobind;

NB_MODULE(nbexample_ext, m) {
  m.def("hello", []() { std::cout << "Hello World" << std::endl; });
  nb::class_<MyObj>(m, "MyObj")
    .def("getValue", &MyObj::getValue)
    ;

  nb::class_<MyContainer>(m, "MyContainer") //, nb::is_weak_referenceable())
    .def("addObject", &MyContainer::addObject)
    .def("getObj", &MyContainer::getObj, nb::rv_policy::reference)
    .def("getObjects", &MyContainer::getObjects, nb::rv_policy::reference)
    ;

  m.def("getContainer", MyContainer::GetInstance, nb::rv_policy::reference);
}
