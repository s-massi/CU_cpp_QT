#include "cu_header.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

using namespace std;

//some external constants
//extern int stop_everything;
static FILE *fplt;

void start_plot()
{
    fplt=popen("gnuplot","w");

    fprintf(fplt, "set key outside\n");
    fprintf(fplt, "set grid ytics lt 0 lw 1 lc rgb \"#bbbbbb\"\n");
    fprintf(fplt, "set grid xtics lt 0 lw 1 lc rgb \"#bbbbbb\"\n");

}

int plot(int period, int plot_values)
{
    fprintf(fplt, "s=system('head -n 1 addresses_to_plot.cfg')\n");
//    fprintf(fplt, "s='%s'\n", add_to_plt.c_str());
    fprintf(fplt, "N=`awk 'NR==1 {print NF}' addresses_to_plot.cfg`\n");


    //periodic loop
    while (!stop_everything)
    {
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(period);

        fprintf(fplt, "plot for [col=1:N] '<tail -n %i ./files/temp_counts.txt' "
                      "using 0:col with lines lw 3 title word(s,col)\n", plot_values);
        fflush(fplt);

        std::this_thread::sleep_until(x);


    }

    return 0;
}

void end_plot()
{
    pclose(fplt);
}
