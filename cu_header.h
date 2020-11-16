#ifndef CU_HEADER_H
#define CU_HEADER_H

#include <cinttypes>
#include <filesystem>
#include <string>
#include <vector>


//some external constants
extern int stop_everything;
extern unsigned int first_line;
//extern std::vector<int> add_to_plot;


//some functions
unsigned int count(unsigned int from_line, std::vector<uint32_t> add_vec);
int keep_counting(int period, std::string add_to_plt);
int data_gen_bin(uint64_t lines);
int plot(int period, int plot_values);
void start_plot();
void start_measure(uint64_t gen_lines, int period_milli, int plot_points, std::string add_to_plt);
void reset();
void end_plot();

#endif // CU_HEADER_H
