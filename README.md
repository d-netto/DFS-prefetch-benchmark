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

       29550871450      cache-references                                            
         750297750      cache-misses              #    2.539 % of all cache refs    
      156502653804      cycles                                                      
       36655683224      instructions              #    0.23  insn per cycle         
   <not supported>      branches                                                    
             32913      faults                                                      
                 2      migrations                                                  

      52.232894828 seconds time elapsed

      52.125446000 seconds user
       0.067960000 seconds sys
```

## Lazy

```
Performance counter stats for './a.out lazy':

       29913972501      cache-references                                            
         750437068      cache-misses              #    2.509 % of all cache refs    
      161009044446      cycles                                                      
       35272892298      instructions              #    0.22  insn per cycle         
   <not supported>      branches                                                    
             32914      faults                                                      
                 0      migrations                                                  

      53.713442143 seconds time elapsed

      53.604904000 seconds user
       0.071952000 seconds sys
```

## Eager prefetch

```
Performance counter stats for './a.out prefetch':

       31499359157      cache-references                                            
         609613723      cache-misses              #    1.935 % of all cache refs    
      138173538393      cycles                                                      
       42951408099      instructions              #    0.31  insn per cycle         
   <not supported>      branches                                                    
             33013      faults                                                      
                 0      migrations                                                  

      46.098657143 seconds time elapsed

      45.988090000 seconds user
       0.075947000 seconds sys
```

## Lazy prefetch

```
Performance counter stats for './a.out prefetch lazy':

       29991722986      cache-references                                            
         438678779      cache-misses              #    1.463 % of all cache refs    
       99406304430      cycles                                                      
       41624984035      instructions              #    0.42  insn per cycle         
   <not supported>      branches                                                    
             33114      faults                                                      
                 0      migrations                                                  

      33.168372392 seconds time elapsed

      33.076813000 seconds user
       0.067952000 seconds sys
```
