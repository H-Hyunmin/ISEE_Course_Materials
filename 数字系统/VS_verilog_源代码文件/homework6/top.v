`include "uart_tx.v"
`include "uart_rx.v"
module top (
    input clk,
    input rxd,
    output txd
);

    wire [7:0] data;   
    wire dete_busy;
    uart_tx  u_uart_tx (
        .clk                     ( clk             ),
        .tx_data                 ( data      [7:0] ),
        .tx_start                ( dete_rdy        ),
        .tx_busy                 ( dete_busy       ),
        .txd                     ( txd             )
    );
    uart_rx  u_uart_rx (
        .clk                     ( clk             ),
        .rxd                     ( rxd             ),
        .data_rdy                ( dete_rdy        ),
        .data                    ( data      [7:0] )
    );
        
endmodule