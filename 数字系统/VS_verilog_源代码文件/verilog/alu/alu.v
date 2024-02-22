module alu (
input [n-1:0]a,b,
input [2:0]s,
output reg [n-1:0] F
);
    parameter n = 4;
    always @(*) begin
        case(s)
            3'b000: F = a & b;
            3'b001: F = a | b;
            3'b010: F = a + b;
            3'b011: F = a - b;
            3'b100: F = a ^ b;
            3'b101: F = ~a;
            3'b110: F = ~b;
            3'b111: F = 1'b0;
        endcase
    end
endmodule