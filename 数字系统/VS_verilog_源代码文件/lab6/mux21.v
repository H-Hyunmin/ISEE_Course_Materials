module mux21
#(parameter n=4)
(
    input S,  
    input [n-1:0]D1,
    input [n-1:0]D0,
    output [n-1:0]Y
);
    
    assign Y = S ? D1 : D0;
endmodule