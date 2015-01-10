#include <iostream>
#include <list>
#include <utility>
#include <type_traits>

template<typename T,typename Q>
std::ostream& operator<<(std::ostream& os,const std::pair<T,Q>& p)
{
  return os<<"["<<p.first<<","<<p.second<<"]";
}

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

template<typename Pred>
auto filter(Pred pred)
{
  return [=](const auto& x){
    return pred(x)?
      mreturn<list>(x):
      list<std::remove_cv_t<std::remove_reference_t<decltype(x)>>>{};
  };
}

int main()
{
  // {x+y | x <- [0 1 2], y <- [1 2 3], even (x+y)}, without filter helper
  std::cout<<
    DO(x,list<int>({0,1,2}),
    DO(y,list<int>({1,2,3}),
       (x+y)%2==0?mreturn<list>(x+y):list<int>{}
    ))
  <<"\n";
  
  auto even=[](const auto& n){return n%2==0;};

  // {x+y | x <- [0 1 2], y <- [1 2 3], even (x+y)}, with filter helper
  std::cout<<
    DO(x,list<int>({0,1,2}),
    DO(y,list<int>({1,2,3}),
       filter(even)(x+y)
    ))
  <<"\n";

  // {x+y | x <- [0 1 2], even x, y <- [1 2 3]}
  std::cout<<
    DO(x,list<int>({0,1,2})>>=filter(even),
    DO(y,list<int>({1,2,3}),
       mreturn<list>(x+y)
    ))
  <<"\n";

  // {x+y | x <- [0 1 2], y <- [1 2 3], even y}
  std::cout<<
    DO(x,list<int>({0,1,2}),
    DO(y,list<int>({1,2,3})>>=filter(even),
       mreturn<list>(x+y)
    ))
  <<"\n";

  // {(x,y) | x <- [0 1 2], y <- [1 2 3]}
  std::cout<<
    DO(x,list<int>({0,1,2}),
    DO(y,list<int>({1,2,3}),
       mreturn<list>(std::make_pair(x,y))
    ))
  <<"\n";

  // {(x,y) | x <- [0 1 2], y <- [1 2 3], (even x) || (even y)}
  std::cout<<
    DO(x,list<int>({0,1,2}),
    DO(y,list<int>({1,2,3}),
       filter([=](const auto&){return (x%2==0)||((y%2==0));})
         (std::make_pair(x,y))
    ))
  <<"\n";
}
