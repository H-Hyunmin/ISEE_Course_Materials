module register
#(parameter n=8)
(
 input clock,clear,load,
 input [n-1:0] D,
    output reg [n-1:0] Q
);
    always @(posedge clock or negedge clear ) begin
        if (clear==0) begin
            Q<=0;
        end
        else if (load==1) begin
            Q<=D;
        end
    end
endmodule