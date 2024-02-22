module ram 
#(parameter size = 5)
(
    input CE,WR,OE,
    input [size-1:0] addr,
    inout [7:0] data

);
    reg [7:0] mem [0:2**size-1];
    reg [7:0] data_out;
    // Tri-state buffer
    assign data =(CE && OE &&~WR)? data_out : 8'bz;
    // Write
    always @(CE or WR or addr or data) begin
      if (CE && WR) begin
        mem[addr] = data;
      end
    end
    // Read
    always @(CE or OE or addr or WR) begin
      if (CE && OE && ~WR) begin
        data_out = mem[addr];
      end
    end
endmodule