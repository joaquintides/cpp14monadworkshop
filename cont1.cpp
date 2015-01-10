#include <iostream>

template<typename C>
auto add(int x,int y,C c){return c(x+y);}

template<typename C>
auto square(int x,C c){return c(x*x);}

template<typename C>
auto pyth(int x,int y,C c) // x*x + y*y
{
  return square(x,[=](int xx){
    return square(y,[=](int yy){
      return add(xx,yy,c);
    });
  });
}

int main()
{
  pyth(3,4,[](int r){std::cout<<r<<"\n";});
}
