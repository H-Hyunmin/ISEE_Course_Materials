import os
import pandas as pd
import matplotlib.pyplot as plt

def list_xlsx_files():
    """列出当前目录下的所有xlsx文件"""
    files = [f for f in os.listdir('.') if f.endswith('.xlsx')]
    return files

def select_file(files):
    """命令行交互选择一个xlsx文件"""
    print("请选择一个xlsx文件：")
    for i, file in enumerate(files):
        print(f"{i + 1}: {file}")
    while True:
        try:
            choice = int(input("输入文件编号：")) - 1
            if 0 <= choice < len(files):
                return files[choice]
            else:
                print("编号超出范围，请重新输入。")
        except ValueError:
            print("输入无效，请输入数字编号。")

def plot_responses(file):
    """读取xlsx文件并绘制25条响应函数"""
    print(f"正在读取文件：{file}")
    try:
        for sheet_index in range(25):
            # 读取每个表格
            df = pd.read_excel(file, sheet_name=sheet_index, header=0)
            # 提取波长和响应
            wavelength = df.iloc[1:, 0].astype(float)  # 第一列为波长
            response = df.iloc[1:, 1].astype(float)   # 第二列为响应
            # 绘制响应曲线
            plt.plot(wavelength, response, linewidth=1)
    except Exception as e:
        print(f"读取文件时出错：{e}")
        return

    # 设置图像参数
    plt.xlabel("Wavelength")
    plt.ylabel("Response")
    plt.title("Response Functions")
    plt.grid(True)
    plt.show()

if __name__ == "__main__":
    # 列出xlsx文件
    xlsx_files = list_xlsx_files()
    if not xlsx_files:
        print("当前目录下没有找到xlsx文件。")
    else:
        # 选择文件
        selected_file = select_file(xlsx_files)
        # 绘制响应函数
        plot_responses(selected_file)