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

        6068509337      cache-references                                            
         750750521      cache-misses              #   12.371 % of all cache refs    
       81540705818      cycles                                                      
        9797467689      instructions              #    0.12  insn per cycle         
   <not supported>      branches                                                    
             32913      faults                                                      
                 0      migrations                                                  

      27.203115072 seconds time elapsed

      27.085055000 seconds user
       0.099944000 seconds sys
```

## Lazy

```
Performance counter stats for './a.out lazy':

        6645977811      cache-references                                            
         750824008      cache-misses              #   11.297 % of all cache refs    
       87335085998      cycles                                                      
       11182745035      instructions              #    0.13  insn per cycle         
   <not supported>      branches                                                    
             32915      faults                                                      
                 0      migrations                                                  

      29.135343833 seconds time elapsed

      29.017391000 seconds user
       0.099949000 seconds sys
```

## Eager prefetch

```
Performance counter stats for './a.out prefetch':

        4832982224      cache-references                                            
         562468244      cache-misses              #   11.638 % of all cache refs    
       46866037671      cycles                                                      
       12711150924      instructions              #    0.27  insn per cycle         
   <not supported>      branches                                                    
             33112      faults                                                      
                 0      migrations                                                  

      15.643485487 seconds time elapsed

      15.536831000 seconds user
       0.095955000 seconds sys
```

## Lazy prefetch

```
Performance counter stats for './a.out prefetch lazy':

        4584468659      cache-references                                            
         361312504      cache-misses              #    7.881 % of all cache refs    
       38257454491      cycles                                                      
       13200558389      instructions              #    0.35  insn per cycle         
   <not supported>      branches                                                    
             33116      faults                                                      
                 0      migrations                                                  

      12.772190360 seconds time elapsed

      12.731075000 seconds user
       0.031977000 seconds sys
```
