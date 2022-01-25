#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/cartesian_communicator.hpp>
#include <boost/mpi/collectives.hpp>
namespace mpi = boost::mpi;

#include <iostream>
#include <exception>
#include <cmath>
#include <vector>

int main()
{
    mpi::environment env;
    mpi::communicator world;
    
    mpi::cartesian_dimension dims[] = {{2,true},{3,true}};
    
    try
    {
        mpi::cartesian_communicator cart(world, mpi::cartesian_topology{dims} );
        int val=1;
        val = mpi::all_reduce(cart,val,std::plus<int>());
        auto c =cart.coordinates(cart.rank());
        //std::cout << cart.rank() << ' ' << c[0]  << ' '<< c[1] << '\n';
        // if(cart.rank()==0)
        // {
        // //for(int i=0;i<10;++i)if(cart.rank()==0)std::cout << cart.rank({0,i}) << '\n';
        // std::cout << cart.topology()[0].size << '\n';
        // std::cout << val << '\n';
        // }
        
        for(int i=0;i<cart.size();++i)
        {
            if(i==cart.rank())
            {
                std::cout << "process " << i <<":\n";
                std::cout << "coordinates " << c[0]  << ' '<< c[1] << '\n';
                
                for(int d=0;d<=cart.ndims();++d)
                {
                    std::cout << "  dim " << d <<":\n";
                    auto p = cart.shifted_ranks(d,1);
                    std::cout << "    first:" << p.first << "\n";
                    std::cout << "    second:" << p.second << "\n";
                    auto proot = cart.shifted_ranks(d,c[d]);
                    std::cout << "    root_first:" << proot.first << "\n";
                    std::cout << "    root_second:" << proot.second << "\n";
                }
            }
            cart.barrier();
        }
        
    }catch(mpi::exception &e)
    {
        std::cerr << e.what()  << "\n" ;
    }
    return 0;
}
