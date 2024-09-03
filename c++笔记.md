## 1 常成员函数，常对象，常成员变量

常对象和常成员变量

```c++
#include <iostream>
using namespace std;

// c++中可以把一个对象声明为const类型，即常对象，这个对象在整个生命周期中就不可被更改，所以定义的时候要由构造函数，初始化列表进行初始化
// 常对象不可以访问类中的非常成员函数，只能访问常成员函数

class Clock
{
private:
    const int h; // 常成员变量
    const int m;
    int const s; //两种定义方法
    int x;

public:
    Clock(int a, int b, int c) : h(a), m(b), s(c)
    {
        x = 99;
        cout << "构造函数被调用" << endl;
    }
    // 非常成员函数
    int ShowTime()
    {
        cout << h << ":" << m << ":" << s << endl;
        return 0;
    }
    // 常成员函数
    int Getx() const
    {
        cout << x << endl;
        return 0;
    }
};

int main()
{
    const Clock A(12, 10, 20);
    const Clock B(14, 20, 30);
    A.Getx();
    // A.ShowTime(); 常对象不能访问类中的非常成员函数

    return 0;
}
```



常成员函数

```C++
#include <iostream>
using namespace std;

// 函数开头的const 用来修饰函数的返回值，表示返回值是const的，返回值不能被修改
// 函数头部的结尾加上const表示常成员函数，常成员函数只能读取成员变量的值，不能修改成员变量的值

class CDate
{
public:
    int year() const { return y; }
    int month() const
    {
        // m++; 报错，表达式必须是可更改的左值，常成员函数中不能更新类的数据成员
        return m;
    }
    int day() { return d++; }
    CDate(int a, int b, int c) : y(a), m(b), d(c)
    {
        cout << "构造函数被调用" << endl;
    }

private:
    int y;
    int m;
    int d;
};

int main()
{
    CDate const date(2020, 10, 20);
    // int dat = date.day(); 常对象不能调用非常成员函数
    int year = date.year();
    cout << year << endl;
    return 0;
}
```



## 2 this指针



this指针是一个指向类对象的指针，用于在成员函数内部访问和操作该对象的成员。始终指向调用成员函数的那个具体对象。



this指针的默认类型：

在一个普通的（非常成员）函数中，this指针的类型是指向类**类型**的非常量版本的常量指针，假如有一个类MyClass，在一个普通的成员函数中（非常成员），this指针的类型就是MyClass *const，这是一个常量指针，常量指针意味着指针本身不可修改，不能改变this指针的指向，但可以修改他指向的对象的值



> 指针本身是常量，不能进行操作this = otherpoint
>
> 指向的对象是非常量，可以通过this->member 来修改成员变量



```c++
class MyClass
{
public:
    void readMember()
    {
        this->member = 10;
        this->member++;
    }

private:
    int member;
};
```

this可以省略



在常成员函数中的变化，当成员函数被声明为const时，this指针的类型变为const MyClass *const，此时this指针既是一个常量指针，又指向一个常量对象。

```c++
class MyClass
{
public:
    void readMember() const
    {
        int value = this->member; // 读取成员变量是合法的
        // this->member = 10;  //不能在常成员函数中修改成员变量
        // this->member++;
    }

private:
    int member;
};
```



## 3 封装继承多态的理解



### 多态

多态的意思就是多种形态，相同方法调用，但是有不同的实现方式，概括为一个接口，多种方法。

有两种多态形式

- 静态多态
- 动态多态



#### 静态多态

被称为是编译期间的多态，编译器在编译期间完成，编译器根据函数实参的类型，推断出要调用哪个函数，如果有对应的函数就调用该函数，否则出现编译错误。

静态多态有两种实现方式：

- 函数重载
- 函数模版

函数重载就像是有多种含义的词，例如spring，根据上下文知道他的具体含义，重载函数的关键是函数参数列表，包括函数的参数数目和类型，参数的排列。所以重载函数与返回值，参数名无关。

```c++
// print()函数
void print(const char* str,int width);
void print(double i ,int width);
void print(const char* str);
// 使用print()函数时，编译器将根据所采取的用法使用有相应特征标的原型
print("abc",12);
print(2.2,55);
print("def");
```

错误重载

```c++
void print(const char *str, int width);
int print(const char *str, int width);
```



函数模版

函数模版是通用的函数描述，使用泛型来定义函数，其中泛型可用具体的类型（int，double等）替换。通过将类型作为参数，传递给模版，使编译器生成该类型的函数

```c++
template <typename T>
void swapf(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}
```



#### 动态多态

运行时多态通常使用**虚函数**来实现，基类定义虚函数，然后派生类可以覆盖（override）这些函数。通过基类指针或引用调用这些函数时，实际执行的是派生类中的版本。



c++11中引入了override关键字，用于显示地指示派生类中的某个成员函数是要覆盖基类中的虚函数，提高安全性，避免无意间没有覆盖基类的虚函数。



**纯虚函数**

纯虚函数是在基类中声明但不实现的虚函数，其声明方式是在函数声明的结尾处添加 =0。类中如果包含至少一个纯虚函数，则该类成为抽象类，不能实例化对象。

纯虚函数的作用是定义接口规范，强制派生类必须实现这些函数，从而实现接口的统一化和标准化。







使用对象指针的优点：

动态绑定

使用对象指针（特别是指向基类的指针）可以实现运行时多态性。可以通过基类指针访问派生类的重写方法。



使用对象指针时，适用于需要运行时多态、动态创建和管理对象，需要处理更复杂的对象声明周期的情况。





## 智能指针

std::make_unique

https://blog.csdn.net/m0_51913750/article/details/130879949



















