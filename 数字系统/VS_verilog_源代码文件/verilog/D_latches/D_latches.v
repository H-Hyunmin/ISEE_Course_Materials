module D_latches (
    input E,
    input D,
    output reg Q
);
    always @(E or D) begin
        if (E==1) Q<=D;
    end;
endmodule