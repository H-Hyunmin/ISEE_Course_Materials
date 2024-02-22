//~ `New testbench
`include "ram.v"
`timescale  1ns / 1ps

module tb_ram;

// ram Parameters
parameter PERIOD  = 10;
parameter size = 5;


// ram Inputs
reg   CE                                   = 0 ;
reg   WR                                   = 0 ;
reg   OE                                   = 0 ;
reg   [size-1:0]  addr                     = 0 ;

// ram Outputs


// ram Bidirs
wire  [7:0]  data                          ;



ram  u_ram (
    .CE                      ( CE               ),
    .WR                      ( WR               ),
    .OE                      ( OE               ),
    .addr                    ( addr  [size-1:0] ),

    .data                    ( data  [7:0]      )
);



initial
begin
    $dumpfile("tb_ram.vcd");
    $dumpvars(0,tb_ram);
    // Write data to memory
    // Set the address
    CE = 1 ;    
    WR = 0 ;     
    OE = 0 ;     
    addr = 0 ;   
    #10   
    // Write data
    CE = 1 ;  
    WR = 1 ;  
    force  data = 12 ;     
    addr = 3 ;    
    #10   
    force  data = 6 ;  
    addr = 5 ; 
    #10 
    force    data = 9 ;   
    addr = 4 ;  
    #10  
    WR = 0 ; 
    OE=1;
    addr=3;   
    #10 
    addr=5;
    #10
    $finish;
end

endmodule