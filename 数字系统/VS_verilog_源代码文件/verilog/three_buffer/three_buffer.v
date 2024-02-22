module three_buffer 
    #(parameter n = 4)
    (input E,
    input [n-1:0] A,
    output[n-1:0] Y
    );
    assign Y = E ? A : {n{1'bz}} ;
endmodule