.globl sumSquare

.text
sumSquare:

    // YOUR CODE BEGIN
    // 保存上下文
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)
    sw s1, 4(sp)
    sw s2, 0(sp)
    // 检查 n 是否为正数
    blez a0, return_zero
    // 初始化累加器 sum 为 0
    li s0, 0
    // 初始化循环变量
    mv s1, a0
loop:
    // 将当前 n 的值传递给 square 函数
    mv a0, s1
    jal ra, square
    // 将平方值加到累加器 sum 中
    add s0, s0, a0
    // 减少 n 的值
    addi s1, s1, -1
    // 如果 n > 0，继续循环
    bgtz s1, loop
    // 将累加器 sum 的值放入 a0 寄存器中作为返回值
    mv a0, s0
    j end

return_zero:
    // 返回 0
    li a0, 0

end:
    // 恢复上下文
    lw ra, 12(sp)
    lw s0, 8(sp)
    lw s1, 4(sp)
    lw s2, 0(sp)
    addi sp, sp, 16

    // YOUR CODE END

    ret

.end
