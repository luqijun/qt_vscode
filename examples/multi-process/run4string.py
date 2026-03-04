import multiprocessing as mp
import time
import sys
import os

temp_path = os.path.join(os.getcwd(), "temp")
dir = r"E:\MyDocs\Code\Qt\qt_vscode_multiprocess\examples\multi-process"
sys.path.append(dir)
print(sys.path)

def hide_subprocess_win():
    
    # 隐藏子进程窗口
    import _winapi
    import multiprocessing.popen_spawn_win32 as pw

    _original_CreateProcess = _winapi.CreateProcess

    def _silent_CreateProcess(app, cmd, proc_attrs, thread_attrs, inherit, flags, env, cwd, startupinfo):
        # CREATE_NO_WINDOW = 0x08000000
        flags |= 0x08000000
        return _original_CreateProcess(app, cmd, proc_attrs, thread_attrs, inherit, flags, env, cwd, startupinfo)

    _winapi.CreateProcess = _silent_CreateProcess

def set_multiprocess_env():
    import sys
    
    # 设置sys.executable
    # sys.executable = r'D:\AppsDev\Python38\python.exe'
    # sys._base_executable = r'D:\AppsDev\Python38\python.exe'  # 关键 
    
    import multiprocessing as mp
    mp.set_executable(r'D:\AppsDev\Python38\python.exe')
    
    print(f"sys.executable = {sys.executable}")
    print(f"sys._base_executable = {sys._base_executable}")


def main():
    
    hide_subprocess_win()
    set_multiprocess_env()

    sys.path.append(dir)
    from taskops import run_task
    with mp.Pool(processes=5) as pool:
        start_time = time.time()
        results = pool.map_async(run_task, range(10)).get()
        end_time = time.time()
        print(f"Results: {results}")
        print(f"Time taken: {end_time - start_time:.2f} seconds")


# ⭐ 嵌入场景下 __name__ == '__main__' 是成立的（因为我们用了 importlib 加载）
# 所以这里会正确执行 main()
if __name__ == "__main__":
    main()
