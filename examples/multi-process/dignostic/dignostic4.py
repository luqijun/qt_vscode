import os
import multiprocessing as mp
print("当前工作目录:", os.getcwd())

mp.set_executable(r"D:\AppsDev\Python38\python.exe")
os.chdir(r'D:\AppsDev\Python38')

# ⭐ 覆盖原始目录
import multiprocessing.process as mpp
mpp.ORIGINAL_DIR = r'D:\AppsDev\Python38'

# 验证 preparation_data 是否干净
import multiprocessing.spawn as sp
data = sp.get_preparation_data('verify')
print("dir:", data.get('dir'))
print("orig_dir:", data.get('orig_dir'))
