module mux21
#(parameter n=4)
(
    input S,  
    input [n-1:0]A,
    input [n-1:0]B,
    output [n-1:0]Y
);
    
    assign Y = S ? A : B;
endmodule