module summation_forloop (
    input reset,
    output reg [7:0] out
);
    reg[7:0]sum;
    reg[3:0]i;
   always @(reset) begin
    sum=0;
    for(i=0;i<=10;i=i+1)
    begin
        sum=sum+i;
    end
    out=sum;
   end

endmodule