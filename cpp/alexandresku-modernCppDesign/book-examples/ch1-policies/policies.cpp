#include <iostream>
#include <string>

class Object {
 public:
  virtual ~Object() {}
  virtual Object* clone() const = 0;
};
class Widget : public Object {
 public:
  Widget* clone() const override { return new Widget; }
};
class Gadget : public Object {
 public:
  Gadget* clone() const override { return new Gadget; }
};

template <class T>
struct OpNewPolicy {
  static std::string name() { return typeid(OpNewPolicy).name(); }
  static T* create() { return new T; }

 protected:
  ~OpNewPolicy() {}
};

template <class T>
struct MallocPolicy {
  static std::string name() { return typeid(MallocPolicy).name(); }
  static T* create() {
    void* p = std::malloc(sizeof(T));
    if (p == nullptr) return nullptr;
    return new (p) T;
  }

 protected:
  ~MallocPolicy() {}
};

template <class T>
struct PrototypePolicy {
  static std::string name() { return typeid(PrototypePolicy).name(); }
  PrototypePolicy(T* p = nullptr) : p_(p) {}
  T* create() { return p_ ? p_->clone() : nullptr; }
  T* getPrototype() { return p_; }
  void setPrototype(T* p) { p_ = p; }

 protected:
  ~PrototypePolicy() {}

 private:
  T* p_;
};

template <template <class> class CreationPolicy = OpNewPolicy>
class WidgetManager : public CreationPolicy<Widget> {
 public:
  void doSomething() {
    const Widget* g = this->create();
    std::cout << "Gadget created: " << (void*)g << " using "
              << CreationPolicy<Widget>::name() << "\n";
    delete g;
  }

  void switchPrototype(Widget* w) {
    CreationPolicy<Widget>& policy = *this;
    delete policy.getPrototype();
    policy.setPrototype(w);
  }
};

using WidgetMgr = WidgetManager<>;
using PrototypeWidgetMgr = WidgetManager<PrototypePolicy>;

int main() {
  WidgetMgr mgr;
  mgr.doSomething();
  Widget* w = WidgetMgr::create();

  PrototypeWidgetMgr mgr2;
  mgr2.switchPrototype(w);
  mgr2.doSomething();

  delete w;
  return 0;
}