//program to read datafile fast, recursively!
//compile as: g++ -std=c++17 -O3 fileread_bin_periodic.cpp -o fileread_bin_periodic -pthread
/*Datafile should be 8 bytes for time-tag [LSB to MSB]
plus 4 bytes for event bitstring*/

// #include <iostream>
// #include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>
#include "cu_header.h"



using namespace std;

//some external constants
//extern int stop_everything;

//some constants
unsigned int ln_length = 12; //line length in bytes
int inputs = 16; //number of input channels

// clock_t tbegin = clock();

//function to count and print out once
unsigned int count(unsigned int from_line, vector<uint32_t> add_vec)
{
  // clock_t tbegin = clock();
  FILE * datafile;
  datafile = fopen ("./files/data.bin" , "rb"); //open input file

  fseek(datafile, from_line*(ln_length), SEEK_SET);

  //keep only info for input channels
  uint64_t addresses = static_cast<uint64_t>(pow(2,inputs));

  unsigned int lines_read = 0;
  auto counters = make_unique<uint32_t[]>(addresses);
//  uint32_t* counters = new uint32_t[addresses](); //will hold the counts

  //set up reading buffer
  unsigned int buffer_size = 200000;
//  auto buffer = make_unique<unsigned char[]>(buffer_size);
  unsigned char* buffer = new unsigned char[buffer_size];
  int lines_in_buf = static_cast<int>(buffer_size/ln_length);

  //set up time-tag based analysis buffer
  // uint32_t* ev_array = new uint32_t[100000000];
  // uint64_t* tt_array = new uint64_t[100000000];

  //each time-tag and event
  // uint64_t time_tag = 0;
  uint32_t event = 0;

  int tmp_lines = 0;

  // with fread
  while ((tmp_lines = fread(buffer, ln_length, lines_in_buf, datafile)) != 0)
  {

      for (int i=0; i<tmp_lines; i++) {

        // time_tag = 0 + buffer[7+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[6+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[5+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[4+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[3+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[2+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[1+ln_length*i];
        // time_tag = (time_tag << 8) + buffer[0+ln_length*i];
        //
        // tt_array[lines_read] = time_tag;

        event = 0 + buffer[11+ln_length*i];
        event = (event << 8) + buffer[10+ln_length*i];
        event = (event << 8) + buffer[9+ln_length*i];
        event = (event << 8) + buffer[8+ln_length*i];

        // ev_array[lines_read] = event;

        if (event <= addresses) {
        ++counters[event];
        }
        ++lines_read;
      }

  }

  fclose (datafile);

  // write to output file
  ofstream ofile ("./files/counts.txt", ios_base::app); //file with all counts
  ofstream plotfile ("./files/temp_counts.txt", ios_base::app); //file with counts to plot
  if (ofile.is_open() && plotfile.is_open())
  {
    for(unsigned int count = 0; count < addresses; count ++){
        ofile << counters[count] << " " ;
    }

    for(unsigned int add = 0; add < add_vec.size(); add ++){
        plotfile << counters[add_vec[add]] << " " ;
    }

    ofile << endl;
    ofile.close();
    plotfile << endl;
    plotfile.close();
  }
  else cout << "Unable to open file";

//  delete[] counters;
  delete[] buffer;

  // cout << "Time " << (double)(clock() - tbegin) / CLOCKS_PER_SEC << endl;
  return lines_read;
}

int keep_counting(int period, std::string add_to_plt) {

//  unsigned int first_line = start_from;

  vector<uint32_t> add_vec;
  stringstream ss(add_to_plt);
  uint32_t value;

  while( ss >> value)
         add_vec.push_back(value);

  //check exec time for debugging
  // auto start = std::chrono::steady_clock::now();

  //periodic loop
  while (!stop_everything)
  {
    auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(period);

    first_line += count(first_line, add_vec);
    //print out lines read for debugging
    // cout << "Counted " << start_from << " total events!" << endl;
    //check exec time for debugging
    //check exec time for debugging
    // std::chrono::duration<double> diff = std::chrono::steady_clock::now()-start;
    // std::cout << "Time now: " << diff.count() << "\n";

    std::this_thread::sleep_until(x);


  }
  return 0;
}
