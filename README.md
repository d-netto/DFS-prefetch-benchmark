# DFS-prefetch-benchmark

## lscpu

```
Architecture:                    aarch64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
CPU(s):                          80
On-line CPU(s) list:             0-79
Thread(s) per core:              1
Core(s) per socket:              80
Socket(s):                       1
NUMA node(s):                    1
Vendor ID:                       ARM
Model:                           1
Model name:                      Neoverse-N1
Stepping:                        r3p1
CPU max MHz:                     3000.0000
CPU min MHz:                     1000.0000
BogoMIPS:                        50.00
L1d cache:                       5 MiB
L1i cache:                       5 MiB
L2 cache:                        80 MiB
NUMA node0 CPU(s):               0-79
Vulnerability Itlb multihit:     Not affected
Vulnerability L1tf:              Not affected
Vulnerability Mds:               Not affected
Vulnerability Meltdown:          Not affected
Vulnerability Mmio stale data:   Not affected
Vulnerability Spec store bypass: Mitigation; Speculative Store Bypass disabled via prctl
Vulnerability Spectre v1:        Mitigation; __user pointer sanitization
Vulnerability Spectre v2:        Mitigation; CSV2, BHB
Vulnerability Srbds:             Not affected
Vulnerability Tsx async abort:   Not affected
Flags:                           fp asimd evtstrm aes pmull sha1 sha2 crc32 atomics fphp asimdhp cpuid asimdrdm lrcpc dcpop asimddp ssbs
```

## Eager

```
Performance counter stats for './a.out':

        6087589578      cache-references                                            
         750515754      cache-misses              #   12.329 % of all cache refs    
       86803786040      cycles                                                      
        9831207412      instructions              #    0.11  insn per cycle         
   <not supported>      branches                                                    
             32914      faults                                                      
                 0      migrations                                                  

      28.957133552 seconds time elapsed

      28.861534000 seconds user
       0.079959000 seconds sys
```

## Lazy

```
 Performance counter stats for './a.out lazy':

        6688349374      cache-references                                            
         750524560      cache-misses              #   11.221 % of all cache refs    
       88309364576      cycles                                                      
       11139266198      instructions              #    0.13  insn per cycle         
   <not supported>      branches                                                    
             32915      faults                                                      
                 0      migrations                                                  

      29.464217607 seconds time elapsed

      29.367708000 seconds user
       0.075957000 seconds sys
```

## Eager prefetch

```
Performance counter stats for './a.out prefetch':

        4749648173      cache-references                                            
         550868293      cache-misses              #   11.598 % of all cache refs    
       51445355078      cycles                                                      
       15424030958      instructions              #    0.30  insn per cycle         
   <not supported>      branches                                                    
             33115      faults                                                      
                 0      migrations                                                  

      17.168312209 seconds time elapsed

      17.059916000 seconds user
       0.095932000 seconds sys
```

## Lazy prefetch

```
Performance counter stats for './a.out prefetch lazy':

        4445878996      cache-references                                            
         347132716      cache-misses              #    7.808 % of all cache refs    
       38901113198      cycles                                                      
       14869874711      instructions              #    0.38  insn per cycle         
   <not supported>      branches                                                    
             33014      faults                                                      
                 0      migrations                                                  

      12.984511470 seconds time elapsed

      12.882227000 seconds user
       0.091930000 seconds sys
```
