#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"                            //mandatory header files

int main(int argc, char **argv, char **env){
    int i;                                              //counts the number of cycles to simulate
    int clk;                                            //the module clock signal
    int pause_cycles = 0;                               //track the remaining pause cycles
    bool has_paused = false;

    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vcounter* top = new Vcounter;                       //Instantiate the counter module as Vcounter which is the name of all generated files = DUT
    //init trace dump
    Verilated::traceEverOn(true);                       
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp,99);
    tfp->open ("counter.vcd");                          //turn on signal tracing and tell verilator to dump the waveform data to counter.vcd
    
    //initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;                                        //Set initial signal levels, top is the name of the top-level entity, only the top level signals are visible

    //run simulation for many clock cycles
    for (i=0; i<300; i++) {                             //this is the for-loop where simulation happens, i counts the clock cycles

        if (top->count == 0x9 && pause_cycles==0 && !has_paused ){
            pause_cycles = 3;                           //sets pause duration to 3 cycles
            has_paused = true;                          //lock out re-triggering
        }

        if (pause_cycles > 0){
            top->en = 0;                                    //freeze counting by setting en = 0
            pause_cycles --;                                //decrement remaining pause cycles
        }
        else{
            top->en=1;                                      //resume counting
        }

            //dump variables into a VCD file and toggle clock
        for (clk=0; clk<2; clk++) {                     
            tfp->dump (2*i+clk);                        //unit is picoseconds
            top->clk= !top->clk;                        //this is the for-loop that toggles the clock & outputs the trace for each half of the clock cycle, & force the model to evaluate at both edges of the clk 
            top->eval ();
        }

        top->rst = (i <2);                  //change rst and en signals during simulation

        if(Verilated::gotFinish()) exit(0) ;
    }
    tfp->close();
    exit(0);
}


