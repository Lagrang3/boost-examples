#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/vector.hpp>
namespace mpi = boost::mpi;

#include <iostream>
#include <vector>

struct myT
{
    int x;
};
template<class Archive>
inline void serialize(
    Archive & ar, 
    myT & t, 
    const unsigned int file_version
){
    ar & t.x;
}

int main()
{
    mpi::environment env;
    mpi::communicator world;
    
    std::vector< std::vector<myT> > in(2);
    
    if(world.rank()==0)
    {
        in[0] = std::vector<myT>{{1},{2}};
        in[1] = std::vector<myT>{{3}};
    }else
    {
        in[0] = std::vector<myT>{{4},{5},{6}};
        in[1] = std::vector<myT>{{7},{8}};
    }
    
    std::vector< std::vector<myT> > out;
    mpi::all_to_all(world,in,out);
    
    for(int t=0;t<world.size();++t)
    {
        if(t==world.rank())
        {
            std::cerr << "Task " << t << '\n';
            for(int i=0;i<out.size();++i)
            {
                auto &v = out[i];
                std::cerr << "\treceived from "<< i << '\n';
                std::cerr << "\t";
                for(auto p : v)
                {   
                    std::cerr << p.x << ' '; 
                }
                std::cerr << '\n';
            }
        }
        world.barrier();
    }
    return 0;
}
