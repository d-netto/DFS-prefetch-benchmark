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

        6092138503      cache-references                                            
         749982873      cache-misses              #   12.311 % of all cache refs    
       86722305432      cycles                                                      
        9872795961      instructions              #    0.11  insn per cycle         
   <not supported>      branches                                                    
             32914      faults                                                      
                 0      migrations                                                  

      28.930620362 seconds time elapsed

      28.812618000 seconds user
       0.099946000 seconds sys
```

## Lazy

```
Performance counter stats for './a.out lazy':

        6697437058      cache-references                                            
         750922963      cache-misses              #   11.212 % of all cache refs    
       85172904405      cycles                                                      
       11155840542      instructions              #    0.13  insn per cycle         
   <not supported>      branches                                                    
             32913      faults                                                      
                 1      migrations                                                  

      28.422230156 seconds time elapsed

      28.335784000 seconds user
       0.067961000 seconds sys
```

## Eager prefetch

```
Performance counter stats for './a.out prefetch':

        5845992879      cache-references                                            
         568421004      cache-misses              #    9.723 % of all cache refs    
       57749757391      cycles                                                      
       18850824198      instructions              #    0.33  insn per cycle         
   <not supported>      branches                                                    
             33114      faults                                                      
                 0      migrations                                                  

      19.272160803 seconds time elapsed

      19.183661000 seconds user
       0.075951000 seconds sys
```

## Lazy prefetch

```
Performance counter stats for './a.out prefetch lazy':

        5564683815      cache-references                                            
         356464232      cache-misses              #    6.406 % of all cache refs    
       42827151680      cycles                                                      
       19212503328      instructions              #    0.45  insn per cycle         
   <not supported>      branches                                                    
             33115      faults                                                      
                 0      migrations                                                  

      14.292633884 seconds time elapsed

      14.198681000 seconds user
       0.083944000 seconds sys
```
