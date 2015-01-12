#include <iostream>
#include <functional>
#include <utility>
#include <boost/optional.hpp>

template<typename T>
class cont
{
  template<typename F>
  cont(const T& x,F f):x([=](){return x;}),f(f){}

public:
  cont(const T& x):x([=](){return x;}){}
  template<typename F>
  cont(const cont& c,F f):c([=](){return c;}),f(f){}
  
  T run()const
  {
    return x?
      (f?f(x()).run():x()):
      f(c().run()).run();
  }

  std::pair<T,boost::optional<cont>> step()const
  {
    if(x){
      if(f)return f(x()).step();
      else return std::make_pair(x(),boost::optional<cont>{});
    }
    else{
      auto res=c().step();
      return std::make_pair(
        res.first,
        res.second?cont{res.second.get(),f}:cont{res.first,f});
    }
  }
  
private:
  std::function<T()>     x;
  std::function<cont()>  c;
  std::function<cont(T)> f;
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

auto fac(int n)
{
  std::cout<<"constructing fac("<<n<<")\n";

  if(n==0){
    return mreturn<cont>(1);
  }
  else{
    return
      DO(m,fac(n-1),(
         std::cout<<"computing "<<m<<"*"<<n<<"\n",
         mreturn<cont>(m*n)
      ));
  }
}

int main()
{
  auto f=fac(5);
  std::cout<<"fac(5)="<<f.run()<<"\n";
  for(;;){
     auto p=f.step();
     std::cout<<"step: "<<p.first<<"\n";
     if(!p.second)break;
     f=p.second.get();
  }
}
