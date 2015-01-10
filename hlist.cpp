#include <iostream>
    
auto list=[](auto... xs)
{
  return [=](auto f){return f(xs...);}; 
};

template<typename List> struct print_wrapper{List l;};
template<typename List> auto print(List l){return print_wrapper<List>{l};}

template<typename T>
void print_helper(std::ostream& os,T t){os<<t<<" ";}

template<typename T,typename... Ts>
void print_helper(std::ostream& os,T t,Ts... ts)
{
  print_helper(os,t);
  print_helper(os,ts...);
}

template<typename List>
std::ostream& operator<<(std::ostream& os,print_wrapper<List> pw)
{
  os<<"[";
  pw.l([&](auto... xs){print_helper(os,xs...);});
  os<<"]";
  return os;
}

template<typename List1>
auto concat(List1 l){
  return l;
}

template<typename List1,typename List2>
auto concat(List1 l1,List2 l2)
{
  return l1([=](auto... x1s){
    return l2([=](auto... x2s){
      return list(x1s...,x2s...);
    });
  });
}

template<typename List1,typename List2,typename... Lists>
auto concat(List1 l1,List2 l2,Lists... ls)
{
 return concat(concat(l1,l2),ls...);
}

auto mreturn=list;

template<typename List,typename F>
auto operator>>=(const List& l, F f)
{
  return l([=](auto... xs){return concat(f(xs)...);});
}

#define DO(var,monad,body)       \
((monad)>>=[=](const auto& var){ \
  return body;                   \
})

int main()
{
  auto l1=list(0,'a');
  auto l2=list(1,3.14);
  auto l3=list("hello",2);
  auto res=concat(l1,l2,l3);
  std::cout<<print(res)<<"\n";

  auto res2=
    DO(x,(mreturn(0,1,2)),
    DO(y,(mreturn(1,2.1,3)),
       mreturn(x+y)
    ));
  std::cout<<print(res2)<<"\n";
}
