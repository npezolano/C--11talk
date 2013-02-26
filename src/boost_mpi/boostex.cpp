//Nicholas Pezolano

#include <vector>
#include <string>
#include <iostream>
#include <iostream>
#include <string>
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>

using namespace std;
namespace mpi = boost::mpi;

#define WORKTAG 1
#define DIETAG 2

int itr = 0;

static void master(mpi::communicator world);
static void slave(mpi::communicator world);
static int get_next_work_item(int &work,const int size_work, std::vector<int> &data);
static void do_work(int work,int &results);


int main(int argc, char **argv){
  int rank;
  mpi::environment env(argc, argv);
  mpi::communicator world;

   rank = world.rank();

  if (rank == 0) {
    master(world);
  } else {
    slave(world);
  }
  return 0;
}

static void master(mpi::communicator world){
  int ntasks, rank;  
  vector<int> data;
  int work;
  int result;

   for(int i = 0; i< 10; i++){
    data.push_back(i);
  }

   const int size_work = (int)data.size();

  rank = world.rank(); //int rank(ID) of processor
  ntasks = world.size();//int total number of processors

  for (rank = 1; rank < ntasks; ++rank) {

    get_next_work_item(work,size_work,data); 
   
     world.send(rank,WORKTAG,work);
  }
  
  int ret = get_next_work_item(work,size_work,data);

  while (ret == 0){
    mpi::status status = world.recv(mpi::any_source,mpi::any_tag,result);

      world.send(status.source(),WORKTAG,work);
     
      ret = get_next_work_item(work,size_work,data);
  }

  for (rank = 1; rank < ntasks; ++rank) {
    world.recv( mpi::any_source, mpi::any_tag,result);
  }

  for (rank = 1; rank < ntasks; ++rank) {
    world.send(rank,DIETAG,0);
  }
}

static void slave(mpi::communicator world) {

  int work;
  int result;

  while (1) {

    mpi::status status = world.recv(mpi::any_source,mpi::any_tag,work);
    if (status.tag() == DIETAG) {
      return;
    }
      do_work(work,result);
      world.send(0,0,result);
  }
}


static int get_next_work_item(int &work,const int size_work, vector<int> &data) {

  if (itr >= size_work) {
    return -1;
  }

  work = data[itr];
  cout<<"symbol = "<<work
      <<"  iter = "<<itr
      <<endl;

    itr++;
  
  return 0;
}

static void do_work(int work, int &result) {
  result = work;
  }


/*
mpic++ -Wall -O2 -std=c++0x boost.cpp -o boost /usr/lib/libmysqlcppconn.so /home/npezolano/libs/lib/libboost_serialization.so /home/npezolano/libs/lib/libboost_mpi.so /home/npezolano/libs/lib/libboost_serialization.so.1.52.0 /home/npezolano/libs/lib/libboost_mpi.so.1.52.0 


mpic++ -Wall -O2 -std=c++0x boost.cpp -o boost  /home/npezolano/libs/lib/libmysqlcppconn.so /home/npezolano/libs/lib/libboost_serialization.so /home/npezolano/libs/lib/libboost_mpi.so /home/npezolano/libs/lib/libboost_serialization.so.1.52.0 /home/npezolano/libs/lib/libboost_mpi.so.1.52.0 
*/
