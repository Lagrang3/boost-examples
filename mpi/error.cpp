#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/cartesian_communicator.hpp>
#include <boost/mpi/collectives.hpp>
namespace mpi = boost::mpi;

#include <iostream>
#include <exception>
#include <cmath>
#include <vector>

class my_error : public std::exception
{
    mpi::communicator commun;
    std::string message;
    
    public:
    my_error(mpi::communicator& com,std::string mes):
        commun{com}, message{mes}    
    {
    }
    const char* what()const noexcept{return message.c_str();}
    
};

bool mpi_condition(mpi::communicator &com, bool cond) {
     return mpi::all_reduce( com ,cond, std::logical_or<bool>());
}
int main()
{
    mpi::environment env;
    mpi::communicator world;
    try
    {
       int a=1;
       if(  mpi_condition(world, world.rank()==1 ))
        throw my_error(world,"shit!");
    }catch(mpi::exception &e)
    {
        std::cerr << "boost::mpi::exception " << e.what()  << "\n" ;
        //env.abort(1);
    }catch(std::exception &e)
    {
        std::cerr << "std::exception " << e.what()  << "\n" ;
        world.barrier();
        //env.abort(1);
    }catch(...)
    {
        std::cerr << "unknown exception\n";
        //env.abort(1);
    }
    return 0;
}
