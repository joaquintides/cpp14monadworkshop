#include <iostream>
#include <list>
#include <utility>

template<typename T>
std::ostream& operator<<(std::ostream& os,const std::list<T>& l)
{
  os<<"[";
  for(const auto& x:l)os<<x<<" ";
  os<<"]";
  return os;
}

template<typename T>
struct list:std::list<T>
{
  using base=std::list<T>;
  using base::base;
  list(const T& x):base(1,x){} // compatibility with mreturn
};

list<int> decinc(int x)
{
  return {x-1,x+1};
}

template<template<typename> class M,typename T>
M<T> mreturn(const T& x)
{
  return x;
}

template<typename T,typename F>
auto operator>>=(const list<T>& l, F f)
{
   decltype(f(std::declval<T>())) ret={};
   for(const auto& t:l)ret.splice(ret.end(),f(t));
   return std::move(ret);
}

#define DO(var,monad,body)       \
((monad)>>=[=](const auto& var){ \
  return body;                   \
})

int main()
{
  auto res=
    DO(x,list<int>({0,1,2}),
    DO(y,list<int>({1,2,3}),
       mreturn<list>(x+y)
    ));
  
  std::cout<<res<<"\n";
}
