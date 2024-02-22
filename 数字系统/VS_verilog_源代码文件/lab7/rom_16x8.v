module rom 
#(parameter size = 4)
    (
    input E,
    input [size-1:0] addr,
    output [7:0] data
    );
reg [7:0] mem[0:size**2-1];
    initial begin
        mem[0] <= 8'b01100000;
        mem[1] <= 8'b10000000;
        mem[2] <= 8'b10100000;
        mem[3] <= 8'b11000001;
        mem[4] <= 8'b11111111;
    end
    assign  data = E? mem[addr] : 8'bzzzzzzzz;
endmodule