module clock_divider 
#(parameter [15:0] half = 16'd10)
(
    input clk_in,
    input  reset,
    output reg clk=0
);
    reg [15:0] count = 16'd0;
    always @(posedge clk_in)
    if (reset==1) begin
        clk=0;
        count=0;
    end
    else 
    begin
        if (count == half)
        begin
            clk <= ~clk;
            count <= 16'd0;
        end
        else
        begin
            count <= count + 16'd1;
        end
    end
endmodule