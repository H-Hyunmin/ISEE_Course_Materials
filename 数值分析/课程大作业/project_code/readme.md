<!--
 * @Author: ZJUHuangXM hxm15726861286@gmail.com
 * @Date: 2023-11-18 22:06:26
 * @LastEditors: ZJUHuangXM hxm15726861286@gmail.com
 * @LastEditTime: 2023-11-06 22:08:01
 * @FilePath: \project_code\readme.md
-->
⽂件结构： 
    ├————project_code 
        ├———README.md 
        ├———Main.m
        ├———Power_Method.m
        ├———QR_Algorithm.m
        ├———gram_schmidt_qr.m        
        ├———Arnoldi_Algorithm.m
        ├———IRAM_Algorithm.m
        ├———IRAM_Arnoldi_iter.m
        ├———Arnoldi_Implicitly_Restart.m
运行环境：matlab环境
运行方式：用matlab打开Main.m脚本，直接运行
文件功能：
Main.m                          项目主体脚本
Power_Method.m                  幂法求特征值函数
QR_Algorithm.m                  QR算法函数
gram_schmidt_qr.m               施密特正交进行QR分解函数
Arnoldi_Algorithm.m             Arnoldi算法函数
IRAM_Algorithm.m                IRAM算法函数
IRAM_Arnoldi_iter.m             初始Arnoldi迭代函数，用于IRAM算法
Arnoldi_Implicitly_Restart.m    Arnoldi重启迭代函数，用于IRAM算法


程序题目介绍
数值分析课程大作业：
用Power Method算法，QR 算法和Arnoldi 迭代算法求解稀疏矩阵特征值
题⽬： 
1. ⽣成随机矩阵 
 （1）⽣成⼀个 10x10 维度的随机矩阵。 
（2）⽣成⼀个 10000x10000 维度且密度为 0.001 的随机稀疏矩阵，并统计矩阵中⾮零
元素数量。 
（3）利⽤库函数计算（1）和（2）中矩阵的特征值。 
2. 给出 Power Method 的伪代码并⽤代码实现，能够输出绝对值最⼤的特征值。 
 （1）利⽤ Power Method 计算题⽬ 1（1）中矩阵的绝对值最⼤的特征值。 
 （2）利⽤ Power Method 计算题⽬ 1（2）中稀疏矩阵的绝对值最⼤的特征值。 
3. 给出 QR 算法的伪代码并⽤代码实现，并能够实现输出前 k 个绝对值最⼤的特征值，其中
k 为⾃定义参数。 
（1）利⽤ QR 算法计算题⽬ 1（1）中矩阵的前 4 个绝对值最⼤的特征值。 
 （2）利⽤ QR 算法计算题⽬ 1（2）中稀疏矩阵的前 5 个绝对值最⼤的特征值。 
4. ⽤代码实现 Arnoldi 迭代算法，并能够实现输出前 k 个绝对值最⼤的特征值，其中 k 为⾃
定义参数。 
（1）利⽤ Arnoldi 迭代算法计算题⽬ 1（1）中矩阵的前 6 个绝对值最⼤的特征值。 
 （2）利⽤ Arnoldi 迭代算法计算题⽬ 1（2）中稀疏矩阵的前 7 个绝对值最⼤的特征值。 
5.（bonus，⾮必做）给出 ERAM 或 IRAM 算法的伪代码并⽤代码实现，并能够实现输出前
k 个绝对值最⼤的特征值，其中 k 为⾃定义参数。 
（1）利⽤ ERAM 或 IRAM 算法计算题⽬ 1（2）中稀疏矩阵的前 8 个绝对值最⼤的特征
值。 
（2）利⽤ ERAM 或 IRAM 算法计算题⽬ 1（2）中稀疏矩阵的前 20 个绝对值最⼤的特
征值。

