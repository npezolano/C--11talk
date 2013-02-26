#include <thread>
#include <iostream>
#include <random>
#include <future>
#include <vector>
#include <random>

#define N 10000

typedef std::mt19937 MyRNG;  
uint32_t seed_val;           
MyRNG rng;         

std::normal_distribution<double> normal_dist(0.0, 1.0);

double rnorm(){
  double sum;
  for(int i = 0; i < N; i++){
    sum += normal_dist(rng);
  } 
  return sum;
}

int main(){
  std::vector<std::future<double> > futures;

  for(size_t i = 0; i < 4; ++i){
    std::packaged_task<double()> pt(rnorm);
    futures.push_back(pt.get_future());
    std::thread(std::move(pt)).detach();
  }
  
  for(size_t n=0; n < futures.size(); ++n){
        std::cout << futures[n].get() << ' ' << std::endl;
  }

  return 0;
}
