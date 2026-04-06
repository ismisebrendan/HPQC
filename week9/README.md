# Week 9

## Files in this directory

 - ```quantum_comp.py``` - Following the tutorial from the [link](https://enccs.se/tutorials/2024/02/quantum-computing-for-beginners-part-3-gpus/) provided, building on the tutorials from weeks 7 and 8.

I was unable to run this as it appears that Cheetah does not have an NVIDIA GPU and/or the correct driver installed, when I run ```quantum_comp.py``` I get an error message:

```
Traceback (most recent call last):
  File "quantum_comp.py", line 443, in <module>
    workspace = pt.tensor([[1.]],device=pt.device('cuda'), dtype=pt.float32)
  File "/home/ug/wattersb2/.local/lib/python3.8/site-packages/torch/cuda/__init__.py", line 314, in _lazy_init
    torch._C._cuda_init()
RuntimeError: Found no NVIDIA driver on your system. Please check that you have an NVIDIA GPU and installed a driver from http://www.nvidia.com/Download/index.aspx
```

However I did copy this from the tutorial so I believe it should be correct.
