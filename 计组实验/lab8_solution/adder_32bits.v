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


//二选一多路器
module mux_2to1 (
    input sel,        // 选择信号
    input [3:0]d0,         // 输入0
    input [3:0]d1,         // 输入1
    output reg [3:0] y       // 输出
);

    always @(*) begin
        if (sel == 1'b0) begin
            y = d0;
        end else begin
            y = d1;
        end
    end

endmodule


//32bit超前进位加法器
module adder_32bits (
    input [31:0] a,   // 32位输入a
    input [31:0] b,   // 32位输入b
    input ci,         // 进位输入
    output [31:0] s,  // 32位和输出
    output co         // 进位输出
);

    wire C3,C7,C11,C15,C19,C23,C27,C31;//级间进位
    wire C7_0,C7_1,C11_0,C11_1,C15_0,C15_1,C19_0,C19_1,C23_0,C23_1,C27_0,C27_1,C31_0,C31_1;//内部进位
    wire [3:0]S3_0,S3_1,S7_0,S7_1,S11_0,S11_1,S15_0,S15_1,S19_0,S19_1,S23_0,S23_1,S27_0,S27_1,S31_0,S31_1;//内部和

    //adder_4bits_X_X表示第x级，ci为X的加法器模块
    //mux_2to1_X表示第x级的mux模块
    //第0级
    adder_4bits adder_4bits_0(.a(a[3:0]),.b(b[3:0]),.ci(ci),.s(s[3:0]),.co(C3));
    //第1级
    adder_4bits adder_4bits_1_1(.a(a[7:4]),.b(b[7:4]),.ci(1'b1),.s(S3_1),.co(C7_1));
    adder_4bits adder_4bits_1_0(.a(a[7:4]),.b(b[7:4]),.ci(1'b0),.s(S3_0),.co(C7_0));
    mux_2to1 mux_2to1_1(.sel(C3),.d0(S3_0),.d1(S3_1),.y(s[7:4]));
    assign C7 = C7_0 | (C7_1 & C3);
    //第2级
    adder_4bits adder_4bits_2_1(.a(a[11:8]),.b(b[11:8]),.ci(1'b1),.s(S7_1),.co(C11_1));
    adder_4bits adder_4bits_2_0(.a(a[11:8]),.b(b[11:8]),.ci(1'b0),.s(S7_0),.co(C11_0));
    mux_2to1 mux_2to1_2(.sel(C7),.d0(S7_0),.d1(S7_1),.y(s[11:8]));
    assign C11 = C11_0 | (C11_1 & C7);
    //第3级
    adder_4bits adder_4bits_3_1(.a(a[15:12]),.b(b[15:12]),.ci(1'b1),.s(S11_1),.co(C15_1));
    adder_4bits adder_4bits_3_0(.a(a[15:12]),.b(b[15:12]),.ci(1'b0),.s(S11_0),.co(C15_0));
    mux_2to1 mux_2to1_3(.sel(C11),.d0(S11_0),.d1(S11_1),.y(s[15:12]));
    assign C15 = C15_0 | (C15_1 & C11);
    //第4级
    adder_4bits adder_4bits_4_1(.a(a[19:16]),.b(b[19:16]),.ci(1'b1),.s(S15_1),.co(C19_1));
    adder_4bits adder_4bits_4_0(.a(a[19:16]),.b(b[19:16]),.ci(1'b0),.s(S15_0),.co(C19_0));
    mux_2to1 mux_2to1_4(.sel(C15),.d0(S15_0),.d1(S15_1),.y(s[19:16]));
    assign C19 = C19_0 | (C19_1 & C15);
    //第5级
    adder_4bits adder_4bits_5_1(.a(a[23:20]),.b(b[23:20]),.ci(1'b1),.s(S19_1),.co(C23_1));
    adder_4bits adder_4bits_5_0(.a(a[23:20]),.b(b[23:20]),.ci(1'b0),.s(S19_0),.co(C23_0));
    mux_2to1 mux_2to1_5(.sel(C19),.d0(S19_0),.d1(S19_1),.y(s[23:20]));
    assign C23 = C23_0 | (C23_1 & C19);
    //第6级
    adder_4bits adder_4bits_6_1(.a(a[27:24]),.b(b[27:24]),.ci(1'b1),.s(S23_1),.co(C27_1));
    adder_4bits adder_4bits_6_0(.a(a[27:24]),.b(b[27:24]),.ci(1'b0),.s(S23_0),.co(C27_0));
    mux_2to1 mux_2to1_6(.sel(C23),.d0(S23_0),.d1(S23_1),.y(s[27:24]));
    assign C27 = C27_0 | (C27_1 & C23);
    //第7级
    adder_4bits adder_4bits_7_1(.a(a[31:28]),.b(b[31:28]),.ci(1'b1),.s(S27_1),.co(C31_1));
    adder_4bits adder_4bits_7_0(.a(a[31:28]),.b(b[31:28]),.ci(1'b0),.s(S27_0),.co(C31_0));
    mux_2to1 mux_2to1_7(.sel(C27),.d0(S27_0),.d1(S27_1),.y(s[31:28]));
    assign C31 = C31_0 | (C31_1 & C27);
    assign co = C31;



endmodule