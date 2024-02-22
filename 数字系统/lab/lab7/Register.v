module Register
#(parameter n=8)
(
 input clk,rst,load,
 input [n-1:0] D,
    output reg [n-1:0] Q
);
    always @(posedge clk or negedge rst ) begin
        if (rst==0) begin
            Q<=0;
        end
        else if (load==1) begin
            Q<=D;
        end
    end
endmodule