import multiprocessing.spawn as sp
import multiprocessing as mp
import inspect

# 查看 get_preparation_data 返回什么
data = sp.get_preparation_data('test')
print("preparation data:")
for k, v in data.items():
    print(f"  {k} = {v}")

print("inspect.getsource(spawn.prepare):")
print(inspect.getsource(sp.prepare))

import multiprocessing.popen_spawn_win32 as pw
import inspect
print("inspect.getsource(pw.Popen.__init__):")
print(inspect.getsource(pw.Popen.__init__))