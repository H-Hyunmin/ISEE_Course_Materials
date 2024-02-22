//~ `New testbench
`timescale  1ns / 1ps
`include "uart_tx.v"
module tb_uart_tx;
// uart_tx Parameters
parameter PERIOD  = 10;
// uart_tx Inputs
reg   clk                                  = 0 ;
reg   [7:0]  tx_data                       = 0 ;
reg   tx_start                             = 0 ;
// uart_tx Outputs
wire  tx_busy                              ;
wire  txd                                  ;
initial
begin
    forever #(PERIOD/2)  clk=~clk;
end
uart_tx  u_uart_tx (
    .clk                     ( clk             ),
    .tx_data                 ( tx_data   [7:0] ),
    .tx_start                ( tx_start        ),

    .tx_busy                 ( tx_busy         ),
    .txd                     ( txd             )
);
initial
begin
$dumpfile("tb_uart_tx.vcd");
$dumpvars(0,tb_uart_tx);
tx_data = 8'b00100000;
tx_start = 1;
#40
tx_start = 0;
#200;
    $finish;
end

endmodule