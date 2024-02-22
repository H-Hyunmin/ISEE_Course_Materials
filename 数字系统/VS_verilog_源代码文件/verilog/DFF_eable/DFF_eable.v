module DFF_eable #(
    parameter n=4
) (
    input clk,
    input reset,
    input eable,
    input [n-1:0] D,
    output reg [n-1:0] Q=0
); 
    always @(posedge clk or posedge reset) begin
        if (reset==1) begin
            Q<=0;
        end
        else if (eable==1) begin
            Q<=D;
        end
    end
endmodule