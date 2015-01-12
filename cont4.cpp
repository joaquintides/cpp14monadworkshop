#include <iostream>
#include <functional>
#include <string>
#include <utility>

template<typename T>
class cont
{
public:
  cont(const T& x):run_([=](){return x;}){}
  template<typename T2,typename F>
  cont(const cont<T2>& c,F f):run_([=](){return f(c.run()).run();}){}
  
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
  using M=decltype(f(std::declval<const T>()));
  return M{c,f};
}

#define DO(var,monad,body)       \
((monad)>>=[=](const auto& var){ \
  return body;                   \
})

auto square(int x)
{
  return mreturn<cont>(x*x);
}

auto to_string(int x)
{
  return mreturn<cont>(std::to_string(x));
}

auto length(const std::string str)
{
  return mreturn<cont>(str.size());
}

auto sqare_digits(int x)
{
  return
  DO(xx,square(x),
  DO(str,to_string(xx),(
     std::cout<<"x:"<<x<<"\n",
     std::cout<<"xx:"<<xx<<"\n",
     std::cout<<"str:"<<str<<"\n",
     length(str)
  )));
}

int main()
{
  std::cout<<sqare_digits(32).run()<<"\n";
}
