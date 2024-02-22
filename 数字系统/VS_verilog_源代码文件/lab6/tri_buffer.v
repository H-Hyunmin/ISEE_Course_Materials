module tri_buffer 
    #(parameter n = 4)
    (input E,
    input [n-1:0] D,
    output[n-1:0] Y
    );
    assign Y = E ? D : {n{1'bz}} ;
endmodule