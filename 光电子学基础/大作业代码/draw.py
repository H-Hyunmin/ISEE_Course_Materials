import os
import pandas as pd
import matplotlib.pyplot as plt

# 获取当前目录下的两个 xlsx 文件
current_dir = os.getcwd()
xlsx_files = [f for f in os.listdir(current_dir) if f.endswith('.xlsx')]

if len(xlsx_files) != 2:
    raise ValueError("当前目录下必须有且仅有两个 .xlsx 文件！")

# 遍历两个 xlsx 文件
for file in xlsx_files:
    # 读取文件中的所有表格
    excel_data = pd.ExcelFile(file)
    sheet_names = excel_data.sheet_names
    
    # 创建一个图像窗口
    plt.figure(figsize=(10, 6))
    
    for sheet in sheet_names:
        # 读取当前表格
        df = excel_data.parse(sheet)
        
        # 提取波长和响应
        wavelength = df.iloc[1:, 0].astype(float).values  # 第一列为波长
        response = df.iloc[1:, 1].astype(float).values    # 第二列为响应
        
        # 绘制响应函数
        plt.plot(wavelength, response, label=f'{sheet}')
    
    # 设置图像标题和标签
    plt.title(f'Response Functions - {file}')
    plt.xlabel('Wavelength (nm)')
    plt.ylabel('Response')
    plt.legend(loc='best', fontsize='small')
    plt.grid()
    
    # 保存图像
    output_image = f'{os.path.splitext(file)[0]}_response_functions.png'
    plt.savefig(output_image)
    plt.show()
    print(f'响应函数图像已保存为 {output_image}')