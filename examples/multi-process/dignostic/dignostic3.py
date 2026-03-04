import multiprocessing.spawn as sp
import inspect
print("执行inspect.getsource(sp._fixup_main_from_name):")
print(inspect.getsource(sp._fixup_main_from_name))

import __main__
print("__main__.__file__:", getattr(__main__, '__file__', 'NONE'))
print("__main__.__spec__:", getattr(__main__, '__spec__', 'NONE'))
print("__main__.__spec__.origin:", getattr(getattr(__main__, '__spec__', None), 'origin', 'NONE'))