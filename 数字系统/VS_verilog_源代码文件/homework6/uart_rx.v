module uart_rx (
    input clk,
    input rxd,
    output  data_rdy,  // goes high when there is a new byte available
    output  [7:0] data  
);

localparam IDLE = 0;
localparam RECEIVING = 1;
localparam DONE = 2;

reg [2:0] state=0, next_state=0; 
reg [2:0] count;
reg [7:0] rx_data=0;
reg rx_data_rdy;

always @(posedge clk ) begin
    state <= next_state;
end
always @(*) begin
    next_state = state;
    case (state)
        IDLE: begin
            if (rxd == 0) begin
                next_state = RECEIVING;
            end
        end
        RECEIVING: begin
            if (count == 7) begin
                next_state = DONE;
            end else begin
                next_state = RECEIVING;
            end
        end
        DONE: begin
            next_state = IDLE;
        end
    endcase
end
    
always @(posedge clk ) begin
    if (state == IDLE) begin
        count <= 0;
    end else if (state == RECEIVING) begin
        count <= count + 1;
    end else if (state == DONE) begin
        count <= 0;
    end
end
always @(posedge clk ) begin
    if (state == RECEIVING) begin
        rx_data[count] <= rxd;
    end 

    end
assign data_rdy = (state == DONE);  //
assign data = rx_data;  //


endmodule