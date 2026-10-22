module counter #(
    parameter WIDTH = 8    //parametrises the number of bits in the counter as 9 because it is indexed at 0
)(
    // interface signals
    input logic     clk,            //clock
    input logic     rst,            //reset
    input logic     en,             //counter enable
    output logic [WIDTH-1:0] count  //count output - WIDTH-1 to 0 is the bit width where the highest indexed is MSB
);

always_ff @ (posedge clk)           // 1'b0 means the bit width is 1 for the binary number 0
    if(rst) count <= {WIDTH{1'b0}}; // if reset = 1 then count is set to 00000000
    else    count <= count + {{WIDTH-1{1'b0}}, en}; // if reset = 0 only when en =1 then count = count + b00000001
                                                    // if reset = 0 oly when en =0 then count = count +b00000000
endmodule                           //curly brackets is concatenation of 7 zeros following by enable which is 1 or 0
