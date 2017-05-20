#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Cache.hpp"

#define KB  * 1024

int main(int argc, char *argv[]) {

  if(argc < 2) {
    std::cerr << "Please run with format: ./cache-sim input_trace.txt output.txt" << std::endl;
    return 1;
  }

  std::vector<std::pair<bool, unsigned long long>> trace;
  unsigned long long addr;
  std::string behavior, line;

  std::ifstream infile(argv[1]);
  while(getline(infile, line)) {
    std::stringstream s(line);
    //std::cerr << line << std::endl;
    s >> behavior >> std::hex >> addr;
    trace.push_back(std::make_pair(behavior == "L", addr));
  }

  // TODO: Decide upon execution format for cache

  std::ostringstream out;
  unsigned long direct_mapped_cache_sizes[] = {1 KB, 4 KB, 16 KB, 32 KB};
  for(auto cache_size : direct_mapped_cache_sizes) {
    Cache dmc(trace, cache_size, 1, false, false, false);
    out << dmc.process() << ((cache_size != 32 KB) ? " " : "");
  }

  out << std::endl;

  unsigned long set_assoc_cache_ways[] = {2, 4, 8, 16};
  for(auto way : set_assoc_cache_ways) {
    Cache sac(trace, 16 KB, way, false, false, false);
    out << sac.process() << ((way != 16) ? " " : "");
  }

  out << std::endl;

  {
    Cache fac_lru(trace, 16 KB, (16 KB) / 32, false, false, false);
    out << fac_lru.process();
  }

  out << std::endl;
  out << "FULLY ASSOCIATIVE WITH HOT COLD NOT IMPLEMENTED YET" << std::endl;

  {
    for(auto way : set_assoc_cache_ways) {
      Cache sac_noalloc(trace, 16 KB, way, true, false, false);
      out << sac_noalloc.process() << ((way != 16) ? " " : "");
    }
  }

  out << std::endl;

  {
    for(auto way : set_assoc_cache_ways) {
      Cache sac_prefetch(trace, 16 KB, way, false, true, false);
      out << sac_prefetch.process() << ((way != 16) ? " " : "");
    }
  }

  out << std::endl;

  {
    for(auto way : set_assoc_cache_ways) {
      Cache sac_prefetch_on_miss(trace, 16 KB, way, false, true, true);
      out << sac_prefetch_on_miss.process() << ((way != 16) ? " " : "");
    }
  }

  //std::cout << out.str() << std::endl;

  //////////////////////
  // Output handling //
  ////////////////////
  std::ostream* output = &std::cout;
  std::ofstream fout;
  if(argc == 3) {
    std::cout << "Outputting to " << argv[2] << "..." << std::endl;
    fout.open(argv[2]);
    output = &fout;
  } else std::cout << "Outputting to standard out..." << std::endl;

  // TODO: Replace with test_all equivalent for cache
  *output << out.str() << std::endl;//pred.test_all();

  return 0;
}
