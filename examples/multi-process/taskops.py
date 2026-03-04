import multiprocessing
import sys
import time
import os

temp_path = os.path.join(os.getcwd(), "temp")

def run_task(x):
    os.makedirs(temp_path, exist_ok=True)
    with open(fr'{temp_path}\child_debug.txt', 'a') as f:
        f.write(f"=== child process x={x} ===\n")
        f.write(f"sys.executable = {sys.executable}\n")
        f.write(f"sys.argv = {sys.argv}\n")
        f.write(f"cwd = {os.getcwd()}\n")
        import __main__
        f.write(f"__main__.__file__ = {getattr(__main__, '__file__', 'NONE')}\n")
        f.write(f"__main__.__spec__ = {getattr(__main__, '__spec__', 'NONE')}\n")
        f.write("\n")
    time.sleep(1)
    return x * x

# 类方法保留也可以，但 Pool 里不要用
class TaskOps:
    @classmethod
    def run_task(cls, x):
        return run_task(x)