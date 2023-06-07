#include <iostream>
#include <vector>

struct Object
{
    void doSomething()
    {
        std::cout << "Object is doing something" << '\n';
    }

    ~Object()
    {
        std::cout << "Object is being destroyed" << '\n';
    }
};

class ObjectPool
{
public:
    Object *acquireObject()
    {
        Object *obj = nullptr;

        if (!pool.empty())
        {
            obj = pool.back();
            pool.pop_back();
        }
        else
        {
            obj = new Object();
        }

        return obj;
    }

    void releaseObject(Object *obj)
    {
        pool.push_back(obj);
    }

    ~ObjectPool()
    {
        for (auto obj : pool)
        {
            delete obj;
        }
    }

private:
    std::vector<Object *> pool;
};
int main(int, char **)
{
    ObjectPool pool;
    Object *obj1 = pool.acquireObject();
    obj1->doSomething();
    Object *obj2 = pool.acquireObject();
    obj2->doSomething();
    pool.releaseObject(obj1);
    pool.releaseObject(obj2);
    Object *obj3 = pool.acquireObject();
    obj3->doSomething();
    pool.releaseObject(obj3);
    return 0;
}
