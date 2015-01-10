#include <boost/optional.hpp>
#include <iostream>
#include <cmath>

template<typename T>
std::ostream& operator<<(std::ostream& os,const boost::optional<T>& x)
{
  if(x)return os<<x.get();
  else return os<<"none";
}

using namespace boost;

optional<double> inv(double x)
{
  if(x==0.0)return none;
  else      return 1.0/x;
}

optional<double> sqr(double x)
{
  if(x<0.0)return none;
  else     return std::sqrt(x);
}

optional<double> arcsin(double x)
{
  if(x<-1.0||x>1.0)return none;
  else             return std::asin(x);
}

int main()
{
  std::cout<<inv(5.0)<<"\n";
  std::cout<<inv(0)<<"\n";
  std::cout<<sqr(4.0)<<"\n";
  std::cout<<sqr(-1.0)<<"\n";
  std::cout<<arcsin(0.5)<<"\n";
  std::cout<<arcsin(1.5)<<"\n";
}
