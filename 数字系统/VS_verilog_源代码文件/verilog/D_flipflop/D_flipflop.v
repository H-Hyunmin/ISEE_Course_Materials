module D_flipflop
#(parameter n = 4)
(
    input [n-1:0]D,
    input clk,
    output reg [n-1:0]Q=0
);
    always @(posedge clk ) begin
        Q<=D;
    end
endmodule