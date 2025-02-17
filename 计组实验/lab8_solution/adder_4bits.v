module adder_4bits (
    input [3:0] a,    // 4位输入a
    input [3:0] b,    // 4位输入b
    input ci,         // 进位输入
    output [3:0] s,   // 4位和输出
    output co         // 进位输出
);

    wire [3:0] g;     // 生成进位
    wire [3:0] p;     // 传播进位
    wire [3:0] c;     // 内部进位

    // 生成和传播进位
    assign g = a & b;
    assign p = a ^ b;

    // 计算进位
    assign c[0] = ci;
    assign c[1] = g[0] | (p[0] & c[0]);
    assign c[2] = g[1] | (p[1] & c[1]);
    assign c[3] = g[2] | (p[2] & c[2]);
    assign co = g[3] | (p[3] & c[3]);

    // 计算和
    assign s = p ^ c;

endmodule