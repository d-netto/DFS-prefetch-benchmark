# DFS-prefetch-benchmark

Simple benchmark to test effectiveness of prefetching in the mark-phase of garbage collectors.

## lscpu

```
Architecture:                    x86_64
CPU op-mode(s):                  32-bit, 64-bit
Byte Order:                      Little Endian
Address sizes:                   46 bits physical, 48 bits virtual
CPU(s):                          36
On-line CPU(s) list:             0-35
Thread(s) per core:              2
Core(s) per socket:              18
Socket(s):                       1
NUMA node(s):                    1
Vendor ID:                       GenuineIntel
CPU family:                      6
Model:                           85
Model name:                      Intel(R) Xeon(R) Platinum 8124M CPU @ 3.00GHz
Stepping:                        4
CPU MHz:                         1505.787
BogoMIPS:                        5999.99
Hypervisor vendor:               KVM
Virtualization type:             full
L1d cache:                       576 KiB
L1i cache:                       576 KiB
L2 cache:                        18 MiB
L3 cache:                        24.8 MiB
NUMA node0 CPU(s):               0-35
Vulnerability Itlb multihit:     KVM: Vulnerable
Vulnerability L1tf:              Mitigation; PTE Inversion
Vulnerability Mds:               Vulnerable: Clear CPU buffers attempted, no microcode; SMT Host state unknown
Vulnerability Meltdown:          Mitigation; PTI
Vulnerability Spec store bypass: Vulnerable
Vulnerability Spectre v1:        Mitigation; usercopy/swapgs barriers and __user pointer sanitization
Vulnerability Spectre v2:        Mitigation; Full generic retpoline, STIBP disabled, RSB filling
Vulnerability Srbds:             Not affected
Vulnerability Tsx async abort:   Not affected
Flags:                           fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ss ht syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon rep_good nopl xtopolog
                                 y nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq monitor ssse3 fma cx16 pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand hypervisor lah
                                 f_lm abm 3dnowprefetch invpcid_single pti fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx avx512f avx512dq rdseed adx smap clflushopt clwb avx512cd avx512bw avx512vl xsaveo
                                 pt xsavec xgetbv1 xsaves ida arat pku ospke
```

## Eager (object enqueueing)

```
 Performance counter stats for './a.out':

         939148124      cache-references                                            
         715013232      cache-misses              #   76.134 % of all cache refs    
      103334874546      cycles                                                      
       11349140492      instructions              #    0.11  insn per cycle         
        2686988208      branches                                                    
             32922      faults                                                      
                 0      migrations                                                  

      29.921045550 seconds time elapsed

      29.859988000 seconds user
       0.060080000 seconds sys
```

## Lazy (edge enqueueing)

```
Performance counter stats for './a.out lazy':

         968644071      cache-references                                            
         738536116      cache-misses              #   76.244 % of all cache refs    
      128893216274      cycles                                                      
       12052674761      instructions              #    0.09  insn per cycle         
        2586785613      branches                                                    
             32921      faults                                                      
                 0      migrations                                                  

      37.301091162 seconds time elapsed

      37.215494000 seconds user
       0.084071000 seconds sys
```

## Eager prefetch (object enqueueing + prefetch)

```
Performance counter stats for './a.out prefetch':

         758815118      cache-references                                            
         581584323      cache-misses              #   76.644 % of all cache refs    
       75534973743      cycles                                                      
       33427897695      instructions              #    0.44  insn per cycle         
        6778701607      branches                                                    
             33122      faults                                                      
                 0      migrations                                                  

      21.878402555 seconds time elapsed

      21.801422000 seconds user
       0.076117000 seconds sys
```

## Lazy prefetch (edge enqueueing + prefetch)

```
Performance counter stats for './a.out prefetch lazy':

         766396384      cache-references                                            
         585606035      cache-misses              #   76.410 % of all cache refs    
       53974221073      cycles                                                      
       33423314496      instructions              #    0.62  insn per cycle         
        6778155652      branches                                                    
             33021      faults                                                      
                 0      migrations                                                  

      15.644569534 seconds time elapsed

      15.523668000 seconds user
       0.120214000 seconds sys
```
