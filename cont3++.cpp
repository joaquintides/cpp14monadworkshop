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

struct fac_state{int n,res;};

auto fac_impl(const fac_state& s)
{
  std::cout<<"constructing fac_impl("<<s.n<<")\n";
  if(s.n==0){
    return mreturn<cont>(s);
  }
  else{
    return fac_impl(fac_state{s.n-1,s.res*s.n});
  }
}

auto fac(int n)
{
  return mreturn<cont>(fac_state{n,1})>>=fac_impl;
}

int main()
{
  auto f=fac(5);
  std::cout<<"running f\n";
  auto r=f.run().res;
  std::cout<<"fac(5)="<<r<<"\n";
}
