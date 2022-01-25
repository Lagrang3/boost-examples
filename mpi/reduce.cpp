#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
namespace mpi = boost::mpi;

#include <utility>
#include <iostream>
#include <algorithm>

struct myT
{
    int x;
};

struct myT_plus
{
    myT operator()(myT a,myT b)const noexcept
    {
        return myT{a.x+b.x};
    }
};

using pair =  std::pair<int,double>;

struct pair_plus
{
    pair operator()(pair a,pair b)const noexcept
    {
        return pair{ a.first+b.first, a.second+b.second  };
    }
};

namespace boost{
namespace serialization{

template<class Archive>
inline void serialize(
    Archive & ar, 
    myT & t, 
    const unsigned int file_version
){
    ar & t.x;
}

template<class Archive>
inline void serialize(
    Archive & ar, 
    pair & t, 
    const unsigned int file_version
){
    ar & t.first;
    ar & t.second;
}

}
}

template<class T, class it, class op>
T myaccumulate(it beg, it end, T start, op O)
{
    for(;beg!=end;++beg)
      start = O(start,*beg);
    return start;
}

int main()
{
    mpi::environment env;
    mpi::communicator world;
    
    {
        int a=1;
        int s = mpi::all_reduce(world,a,std::plus<int>()); // ok
        
        std::vector<int> v{1,2,3};
        //int s2 = std::accumulate(v.begin(),v.end(),0,std::plus<int>()); // ok
        int s2 = myaccumulate(v.begin(),v.end(),0,std::plus<int>()); // ok
        
        if(world.rank()==0)
        {
            std::cout << "sum1: " << s << '\n';
            std::cout << "sum2: " << s2 << '\n';
        }
    }
    
    {
        int a=1;
        //int s = mpi::all_reduce(world,a,[](int x,int y){return x+y;}); // ok
        
        std::vector<int> v{1,2,3};
        // int s2 = std::accumulate(v.begin(),v.end(),0,[](int x,int y){return x+y;}); // ok
        int s2 = myaccumulate(v.begin(),v.end(),0,[](int x,int y){return x+y;}); // ok
        
        if(world.rank()==0)
        {
            //std::cout << "sum1: " << s << '\n';
            std::cout << "sum2: " << s2 << '\n';
        }
    }
    {
        myT a{1};
        myT s = mpi::all_reduce(world,a,myT_plus()); // ok
        
        std::vector<myT> v(3,myT{1});
        myT s2 = std::accumulate(v.begin(),v.end(),myT{0},myT_plus()); // ok
        
        if(world.rank()==0)
        {
            std::cout << "sum1: " << s.x << '\n';
            std::cout << "sum2: " << s2.x << '\n';
        }
    }
    {
        pair a{1,1};
        pair s = mpi::all_reduce(world,a,pair_plus()); // ok
        
        std::vector<pair> v(3,pair{1,1});
        pair s2 = std::accumulate(v.begin(),v.end(),pair{0,0},pair_plus()); // ok
        
        if(world.rank()==0)
        {
            std::cout << "sum1: " << s.first << '\n';
            std::cout << "sum2: " << s2.first << '\n';
        }
    }
    return 0;
}
