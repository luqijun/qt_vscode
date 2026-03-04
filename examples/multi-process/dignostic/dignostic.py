import multiprocessing.spawn as sp
print("get_executable:", sp.get_executable())
import multiprocessing as mp
mp.set_executable(r"D:\AppsDev\Python38\python.exe")
print("after set, get_executable:", sp.get_executable())

import multiprocessing.spawn as sp
import inspect
print(inspect.getsource(sp.get_executable))
print(inspect.getsource(sp.set_executable))