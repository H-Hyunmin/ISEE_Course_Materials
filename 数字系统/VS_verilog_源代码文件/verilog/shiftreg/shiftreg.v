module shiftreg 
#(parameter n = 8)
(
    input clk,
    input shift,
    input serial_in,
    output reg [n-1:0] Q=0
);
  always @(posedge clk ) begin
     if (shift) begin
      Q<={serial_in,Q[n-1:1]};
     end
  end  
endmodule