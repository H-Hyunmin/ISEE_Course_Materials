`include "adder_4bits.v"
module adder_8bits (
    input [7:0] a,   // 8位输入a
    input [7:0] b,   // 8位输入b
    input ci,         // 进位输入
    output [7:0] s,  // 8位和输出
    output co         // 进位输出
);

    wire [1:0] carry; // 内部进位

    // 生成2个4位加法器实例
    genvar i;
    generate
        for (i = 0; i < 2; i = i + 1) begin : adder_gen
            if (i == 0) begin
                adder_4bits adder_4bits_inst (
                    .a(a[3:0]),
                    .b(b[3:0]),
                    .ci(ci),
                    .s(s[3:0]),
                    .co(carry[0])
                );
            end else begin
                adder_4bits adder_4bits_inst (
                    .a(a[i*4+3:i*4]),
                    .b(b[i*4+3:i*4]),
                    .ci(carry[i-1]),
                    .s(s[i*4+3:i*4]),
                    .co(carry[i])
                );
            end
        end
    endgenerate

    assign co = carry[1];

endmodule




// 32位四级流水线加法器
module pipeline_adder (
    input clk,         // 时钟信号
    input [31:0] a,    // 32位输入a
    input [31:0] b,   // 32位输入b
    input ci,          // 进位输入
    output [31:0] s,  // 32位和输出
    output  co      // 进位输出
);
    // 定义5级流水线寄存器
    reg [31:0] a_reg1, a_reg2, a_reg3, a_reg4, a_reg5;
    reg [31:0] b_reg1, b_reg2, b_reg3, b_reg4, b_reg5;
    reg ci_reg1=0, ci_reg2=0, ci_reg3=0, ci_reg4=0, ci_reg5=0;
    reg [31:0] s_reg1 = 32'h0, s_reg2 = 32'h0, s_reg3 = 32'h0, s_reg4 = 32'h0;
    // 定义4个8位加法器的输入输出
    wire [7:0] s_wire1, s_wire2, s_wire3, s_wire4;
    wire co_wire1, co_wire2, co_wire3, co_wire4;

    // 第一级流水线寄存器
    always @(posedge clk) begin
        a_reg1 <= a;
        b_reg1 <= b;
        ci_reg1 <= ci;
    end

    // 第二级流水线寄存器
    always @(posedge clk) begin
        a_reg2 <= a_reg1;
        b_reg2 <= b_reg1;
        ci_reg2 <= co_wire1;
        s_reg1 <= {24'b0, s_wire1};

    end

    // 第三级流水线寄存器
    always @(posedge clk) begin
        a_reg3 <= a_reg2;
        b_reg3 <= b_reg2;
        ci_reg3 <= co_wire2;
        s_reg2 <= {16'b0, s_wire2, s_reg1[7:0]};

    end

    // 第四级流水线寄存器
    always @(posedge clk) begin
        a_reg4 <= a_reg3;
        b_reg4 <= b_reg3;
        ci_reg4 <= co_wire3;
        s_reg3 <= {8'b0, s_wire3, s_reg2[15:0]};

    end

    // 第五级流水线寄存器
    always @(posedge clk) begin
        a_reg5 <= a_reg4;
        b_reg5 <= b_reg4;
        ci_reg5 <= co_wire4;
        s_reg4 <= {s_wire4, s_reg3[23:0]};

    end

    // 实例化4个8位加法器
    adder_8bits adder_8bits_inst1 (
        .a(a_reg1[7:0]),
        .b(b_reg1[7:0]),
        .ci(ci_reg1),
        .s(s_wire1),
        .co(co_wire1)
    );

    adder_8bits adder_8bits_inst2 (
        .a(a_reg2[15:8]),
        .b(b_reg2[15:8]),
        .ci(ci_reg2),
        .s(s_wire2),
        .co(co_wire2)
    );

    adder_8bits adder_8bits_inst3 (
        .a(a_reg3[23:16]),
        .b(b_reg3[23:16]),
        .ci(ci_reg3),
        .s(s_wire3),
        .co(co_wire3)
    );

    adder_8bits adder_8bits_inst4 (
        .a(a_reg4[31:24]),
        .b(b_reg4[31:24]),
        .ci(ci_reg4),
        .s(s_wire4),
        .co(co_wire4)
    );

    // 输出结果
    assign s = s_reg4;
    assign co = ci_reg5;

endmodule




//********************************************************
//下面是用锁存器实现的版本
//********************************************************

// module latch_nbits #(
//     parameter n = 32
// ) (
//     input wire [n-1:0] in,
//     input wire clk,
//     output reg [n-1:0] out
// );

//     always @(posedge clk) begin
//         out <= in;
//     end

// endmodule



// // 32位四级流水线加法器
// module pipeline_adder (
//     input clk,         // 时钟信号
//     input [31:0] a,    // 32位输入a
//     input [31:0] b,   // 32位输入b
//     input ci,          // 进位输入
//     output [31:0] s,  // 32位和输出
//     output co      // 进位输出
// );  
//     wire [64:0] latch1_out;
//     wire [56:0] latch2_out;
//     wire [48:0] latch3_out;
//     wire [40:0] latch4_out;
//     wire [32:0] latch5_out;

//     wire[7:0] s_wire1, s_wire2, s_wire3, s_wire4;
//     wire co_wire1, co_wire2, co_wire3, co_wire4;

//     //一级锁存器
//     latch_nbits #(
//         .n(65)
//     ) latch1 (
//         .in({a, b, ci}),
//         .clk(clk),
//         .out(latch1_out)
//     );

//     adder_8bits adder_8bits_inst1 (
//         .a(latch1_out[40:33]),
//         .b(latch1_out[8:1]),
//         .ci(latch1_out[0]),
//         .s(s_wire1),
//         .co(co_wire1)
//     );

//     //二级锁存器
//     latch_nbits #(
//         .n(57)
//     ) latch2 (
//         .in({latch1_out[64:41],latch1_out[32:9],co_wire1, s_wire1}),
//         .clk(clk),
//         .out(latch2_out)
//     );

//     adder_8bits adder_8bits_inst2 (
//         .a(latch2_out[40:33]),
//         .b(latch2_out[16:9]),
//         .ci(latch2_out[8]),
//         .s(s_wire2),
//         .co(co_wire2)
//     );

//     //三级锁存器
//     latch_nbits #(
//         .n(49)
//     ) latch3 (
//         .in({latch2_out[56:41],latch2_out[32:17],co_wire2, s_wire2, latch2_out[7:0]}),
//         .clk(clk),
//         .out(latch3_out)
//     );

//     adder_8bits adder_8bits_inst3 (
//         .a(latch3_out[40:33]),
//         .b(latch3_out[24:17]),
//         .ci(latch3_out[16]),
//         .s(s_wire3),
//         .co(co_wire3)
//     );

//     //四级锁存器
//     latch_nbits #(
//         .n(41)
//     ) latch4 (
//         .in({latch3_out[48:41],latch3_out[32:25],co_wire3, s_wire3, latch3_out[15:0]}),
//         .clk(clk),
//         .out(latch4_out)
//     );

//     adder_8bits adder_8bits_inst4 (
//         .a(latch4_out[40:33]),
//         .b(latch4_out[32:25]),
//         .ci(latch4_out[24]),
//         .s(s_wire4),
//         .co(co_wire4)
//     );

//     //五级锁存器
//     latch_nbits #(
//         .n(33)
//     ) latch5 (
//         .in({co_wire4, s_wire4, latch4_out[23:0]}),
//         .clk(clk),
//         .out(latch5_out)
//     );

//     // 输出结果 
//     assign s = latch5_out[31:0];
//     assign co = latch5_out[32];

// endmodule