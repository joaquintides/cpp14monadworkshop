#include <iostream>
#include <functional>

template<typename T>
class cont
{
public:
  cont(const T& x):run_([=](){return x;}){}
  template<typename F>
  cont(const cont& c,F f):run_([=](){return f(c.run()).run();}){}
  
  T run()const{return run_();}
  
private:
  std::function<T()> run_;
};

template<template<typename> class M,typename T>
M<T> mreturn(const T& x)
{
  return x;
}

template<typename T,typename F>
auto operator>>=(const cont<T>& c, F f)
{
  return cont<T>{c,f};
}

#define DO(var,monad,body)       \
((monad)>>=[=](const auto& var){ \
  return body;                   \
})

auto add(int x,int y)
{
  return mreturn<cont>(x+y);
}

auto square(int x)
{
  return mreturn<cont>(x*x);
}

auto pyth(int x,int y)
{
  return
  DO(xx,square(x),
  DO(yy,square(y),
     add(xx,yy)
  ));
}

int main()
{
  std::cout<<pyth(3,4).run()<<"\n";
}
