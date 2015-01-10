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

auto fac(int n)
{
  std::cout<<"constructing fac("<<n<<")\n";
  if(n==0){
    return mreturn<cont>(1);
  }
  else{
    return fac(n-1)>>=[=](int m){
      std::cout<<"computing "<<m<<"*"<<n<<"\n";
      return mreturn<cont>(m*n);
    };
  }
}

int main()
{
  auto f=fac(5);
  std::cout<<"running f\n";
  auto r=f.run();
  std::cout<<"fac(5)=="<<r<<"\n";
}
