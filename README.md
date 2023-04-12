# tevents — header-only tiny event system

## How to use
This is header-only library, so all you need is to copy it to your project (or connect as git submodule), add its folder to include path, then include the ```tevents.hpp``` header.

```cpp
#include <tevents.hpp>

using namespace te;
```

Now you need to create ```Event```, then ```Subscribe``` your functions, ```Invoke``` event to call subscribed functions, and ```Unsubscribe``` if needed. Also you can use ```operator ()``` as ```Invoke```

### Non-class member method subscription

```cpp
void CallbackSum(int a, int b)
{
    std::cout << "Sum: " << a + b << std::endl;
}

void CallbackSub(int a, int b)
{
    std::cout << "Diff: " << a - b << std::endl;
}

...

Event<int, int> event;

event.Subscribe(CallbackSum);
event.Subscribe(CallbackSub);

event.Invoke(5, 4);
/*
Will print:
    Sum: 9
    Diff: 1
*/

event(7, 3); // Same as event.Invoke(7, 3);
/*
Will print:
    Sum: 10
    Diff: 4
*/


event.Unsubscribe(CallbackSum);
event.Unsubscribe(CallbackSub);
```

### Class member method subscription

Since member methods does not provide information about their instance, you must associate a callback with an instance of the class. This rule does not apply to static methods, they can be subscribed to an event like regular functions.

For subscription, you can use both the target class instances themselves and pointers to them, but it is important to ensure that the instances participating in the subscription are always up-to-date, otherwise you risk getting an invalid pointer dereference on event call.
```cpp
class TestClass
{
public:
    static void MemberCallbackStatic(int a, int b)
    {
        std::cout << "Static member Sum: " << a + b << std::endl;
    }

    void MemberCallback(int a, int b)
    {
        std::cout << "Non-static member Diff: " << a - b << std::endl;
    }
};

...

Event<int, int> event;
TestClass test;

event.Subscribe(TestClass::MemberCallbackStatic);
event.Subscribe(test, &TestClass::MemberCallback);

event.Invoke(32, 11);
/*
Will print:
    Static member Sum: 43
    Non-static member Diff: 21
*/

event(12, 5); // Same as event.Invoke(12, 5);
/*
Will print:
    Static member Sum: 17
    Non-static member Diff: 7
*/

event.Unsubscribe(TestClass::MemberCallbackStatic);
event.Unsubscribe(test, &TestClass::MemberCallback);
```