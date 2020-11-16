//compile with: g++ -std=c++17 -O3 -lpthread data_gen_bin.cpp -o data_gen_bin

#include <iostream>
#include <random>
#include <limits>
#include <thread>
#include <chrono>
#include "cu_header.h"

//some external constants
//extern int stop_everything;

int data_gen_bin(uint64_t lines)
{
  std::random_device rd;     //Get a random seed from the OS entropy device, or whatever
  std::mt19937_64 eng(rd()); //Use the 64-bit Mersenne Twister 19937 generator
                             //and seed it with entropy.

  //Define the distribution, by default it goes from 0 to MAX(unsigned long long)
  //or what have you.
  std::uniform_int_distribution<uint64_t> tt;
  std::uniform_int_distribution<uint8_t> ev;

  FILE * pFile;

  pFile = std::fopen ("./files/data.bin","ab");

  //start of time_tag
  uint64_t tt_start = tt(eng);
  std::cout << "start from: " << tt_start << std::endl;

  uint8_t empty = 0;

  std::cout << "stop?: " << stop_everything << std::endl;
  //Generate random numbers
  uint64_t n=0;
  while(n<lines && !stop_everything)
  {
    uint64_t t_start = tt_start + n;
    std::fwrite (&t_start, 8, 1, pFile);
    uint8_t event = ev(eng);
    std::fwrite (&event, 1, 1, pFile);
    std::fwrite (&empty, 1, 1, pFile);
    std::fwrite (&empty, 1, 1, pFile);
    std::fwrite (&empty, 1, 1, pFile);
    n++;
    std::this_thread::sleep_for(std::chrono::nanoseconds(10));
  }

  std::fclose (pFile);

  return 0;
}
