module shift 
 #(parameter n = 8)
(
    input [1:0]s,
    input [n-1:0]date,
    output reg [n-1:0]outdate
);
always @(s or date) begin
    case(s)
        2'b00: outdate = date;
        2'b01: outdate = date<<1;
        2'b10: outdate = date>>1;
        2'b11: outdate = {date[0],date[n-1:1]};
    endcase
end
endmodule