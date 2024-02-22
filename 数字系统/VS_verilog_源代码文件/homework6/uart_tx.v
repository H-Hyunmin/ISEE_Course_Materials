module uart_tx (
    input clk,
    input [7:0] tx_data,
    input tx_start,
    output tx_busy,
    output reg txd
);
    localparam IDLE = 0;
    localparam START = 1;
    localparam SENDING = 2;
    localparam DONE = 3;
    reg [2:0] state=IDLE, next_state=IDLE; 
    reg [2:0] count;


    always @(posedge clk ) begin
        state <= next_state;
    end    
    always @(*) begin
        next_state = state;
        case (state)
            IDLE: begin
                if (tx_start) begin
                    next_state = START;
                end
            end
            START: begin
                next_state = SENDING;
            end
            SENDING: begin
                if (count == 7) begin
                    next_state = DONE;
                end else begin
                    next_state = SENDING;
                end
            end
            DONE: begin
                next_state = IDLE;
            end
        endcase
    end
    always @(posedge clk ) begin
        if (state == SENDING) begin
            count <= count + 1;
        end else if (state == DONE||state == IDLE) begin
            count <= 0;
        end
        end
    always @(posedge clk ) begin
        if (state == START) begin
            txd <= 0;
        end else if (state == SENDING) begin
            txd <= tx_data[count];
        end else if (state == DONE|state==IDLE) begin
            txd <= 1;
        end
        end
    assign tx_busy = (state == SENDING);  
endmodule