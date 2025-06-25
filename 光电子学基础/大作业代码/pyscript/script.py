import os
import pandas as pd
import numpy as np
from scipy.interpolate import CubicSpline
import matplotlib.pyplot as plt

# 获取当前目录下的所有 xlsx 文件
current_dir = os.getcwd()
xlsx_files = [f for f in os.listdir(current_dir) if f.endswith('.xlsx')]

# 创建一个新的 Excel 文件用于保存均匀采样后的数据
output_file = 'uniform_sampled_accurate.xlsx'
writer = pd.ExcelWriter(output_file, engine='openpyxl')

# 遍历每个 xlsx 文件
for file in xlsx_files:
    # 读取文件中的所有表格
    excel_data = pd.ExcelFile(file)
    sheet_names = excel_data.sheet_names
    
    for sheet in sheet_names:
        # 读取当前表格
        df = excel_data.parse(sheet)
        
        # 提取波长和响应
        wavelength = df.iloc[1:, 0].astype(float).values  # 第一列为波长
        response = df.iloc[1:, 1].astype(float).values    # 第二列为响应
        
        # 绘制原始响应函数
        plt.plot(wavelength, response, label=f'{file} - {sheet}')
        
        # 进行均匀采样
        uniform_wavelength = np.arange(300, 1001, 1)  # 300nm到1000nm，间隔1nm
        cubic_spline = CubicSpline(wavelength, response)
        uniform_response = cubic_spline(uniform_wavelength)
        
        # 保存均匀采样后的数据到新的表格
        output_df = pd.DataFrame({
            'Wavelength (nm)': uniform_wavelength,
            'Response': uniform_response
        })
        output_df.to_excel(writer, sheet_name=f'{os.path.splitext(file)[0]}_{sheet}', index=False)

# 保存绘图
plt.xlabel('Wavelength (nm)')
plt.ylabel('Response')
plt.title('Response Functions')
plt.legend()
plt.grid()
plt.savefig('response_functions_accurate.png')
plt.show()

# 保存均匀采样后的 Excel 文件
writer._save()
print(f'均匀采样后的数据已保存到 {output_file}')