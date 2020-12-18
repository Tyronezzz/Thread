## Thread

### Reference:
https://www.cplusplus.com/reference/thread/thread/joinable/
https://cloud.tencent.com/developer/article/1584149
https://www.runoob.com/w3cnote/cpp-std-thread.html

https://www.jianshu.com/p/f191e88dcc80
https://blog.csdn.net/richenyunqi/article/details/89530589


### Pre-knowledge

#### 拷贝构造函数
通常用于以下情况：

- 通过使用另一个同类型的对象来初始化新创建的对象。
- 复制对象把它作为参数传递给函数。
- 复制对象，并从函数返回这个对象。

如果在类中没有定义拷贝构造函数，编译器会自行定义一个。如果类带有指针变量，并有动态内存分配，则它必须有一个拷贝构造函数。拷贝构造函数的最常见形式如下：

```
classname (const classname &obj) {
   // 构造函数的主体
}

```


#### 拷贝赋值运算符
用一个已存在的对象去覆盖另一个已存在的对象，是赋值运算。

```
class Person
{
public:
    Person()
    {
        cout << "Default Constructor" << endl;
    }
    Person(const Person& p)
    {
        _name = p._name;
        cout << "Copy Constructor" << endl;
    }
    
    Person& operator=(const Person& p)
    {
        cout << "Assign" << endl;
        _name = p._name;
        return *this;
    }
    
private:
    string _name;
};
```


#### Functional programming in C++
函数式编程中，函数的调用以pipeline的形式进行。C++中，我们同样可以把函数（指针）作为一个参数传入，在子函数中调用。


```
int compute(int a, int b, int(*f)(int, int))
    return f(a, b);
}

compute(1, 2, plus<int>());
```


- std::function 是一个可调用对象包装器，是一个类模板，可以容纳除了类成员函数指针之外的所有可调用对象，它可以用统一的方式处理函数、函数对象、函数指针，并允许保存和延迟它们的执行。std::function可以取代函数指针的作用，因为它可以延迟函数的执行，特别适合作为回调函数使用。它比普通函数指针更加的灵活和便利。

- std::bind 可将std::bind函数看作一个通用的函数适配器，它接受一个可调用对象，生成一个新的可调用对象来“适应”原对象的参数列表。std::bind将可调用对象与其参数一起进行绑定，绑定后的结果可以使用std::function保存。

```
double my_divide (double x, double y) {return x/y;}
auto fn_half = std::bind (my_divide, std::placeholders::_1, 2);  
std::cout << fn_half(10) << '\n';         
```


```
#include <iostream>
#include <functional>
using namespace std;

// 普通函数
int add(int a, int b){return a+b;} 

// lambda表达式
auto mod = [](int a, int b){ return a % b;};

// 函数对象类
struct divide{
    int operator()(int denominator, int divisor){
        return denominator/divisor;
    }
};

std::function<int(int ,int)>  a = add; 
std::function<int(int ,int)>  b = mod ; 
std::function<int(int ,int)>  c = divide(); 


int main()
{
    cout<<a(1,2)<<endl;
    cout<<b(4,2)<<endl;
    cout<<c(12,6)<<endl;
    
    return 0;
}
```

### std::ref() and &

函数模板 ref 与 cref 是生成 std::reference_wrapper 类型对象的帮助函数，它们用模板实参推导确定结果的模板实参。所以std::ref()返回的实际上是一个reference_wrapper而不是T&，可以从一个指向不能拷贝的类型的对象的引用生成一个可拷贝的对象。 std::reference_wrapper 的实例是对象（它们可被复制或存储于容器），但它们能隐式转换成 T& ，故能以之为以引用接收底层类型的函数的参数

```
#include <functional>
#include <iostream>
 
void f(int& n1, int& n2, const int& n3)
{
    std::cout << "In function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    ++n1; // increments the copy of n1 stored in the function object
    ++n2; // increments the main()'s n2
}
 
int main()
{
    int n1 = 1, n2 = 2, n3 = 3;
    std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
    n1 = 4;
    n2 = 5;
    n3 = 6;
    std::cout << "Before function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
    bound_f();
    std::cout << "After function: " << n1 << ' ' << n2 << ' ' << n3 << '\n';
}

> Before function: 4 5 6
> In function: 1 5 6
> After function: 4 6 6

```
bind()时，& 形参获取的是copy, ref获取的是引用。
std::ref只是尝试模拟引用传递，并不能真正变成引用，在非模板情况下，std::ref根本没法实现引用传递，只有模板自动推导类型时，ref能用包装类型reference_wrapper来代替原本会被识别的值类型，而reference_wrapper能隐式转换为被引用的值的引用类型，但是并不能被用作&类型。

而回到刚开始的那个多线程代码，thread的方法传递引用的时候，我们希望使用的是参数的引用，而不是浅拷贝，所以必须用ref来进行引用传递。





### Key word
 
#### join
The function returns when the thread execution has completed.


#### detached
Detaches the thread represented by the object from the calling thread, allowing them to execute independently from each other.
After a call to this function, the thread object becomes non-joinable and can be destroyed safely.


#### joinable
Returns whether the thread object is joinable.
A thread object is not joinable in any of these cases:
- if it was default-constructed.
- if it has been moved from (either constructing another thread object, or assigning to it).
- if either of its members join or detach has been called.

