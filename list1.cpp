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

int main()
{
  std::cout<<decinc(0)<<"\n";
}
