`include "half.v"

module full_adder(
                  input a,
                  input b,
                  input cout,    //来自上一级的进位
                  output sum,
                  output cin
                  );

wire sum_a;
wire cin_a;
wire cin_b;
half_adder HALF_ADDER_A(
           .a(a),
           .b(b),
           .sum(sum_a),
           .cout(cin_a)
             );

half_adder HALF_ADDER_B(
           .a(cout),
           .b(sum_a),
           .sum(sum),
           .cout(cin_b)
             );
assign cin=cin_a|cin_b;
endmodule
