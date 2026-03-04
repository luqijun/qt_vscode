import multiprocessing as mp
import time
import sys
import os

sys.path.append("/mnt/e/MyDocs/Code/Qt/qt_vscode/examples/multi-process/")

def main():

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
