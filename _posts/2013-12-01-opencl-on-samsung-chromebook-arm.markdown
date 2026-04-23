---
layout: post
title: "OpenCL on the Samsung Chromebook ARM - Benchmarks"
date: 2013-12-01 19:00:00+08:00
categories: linux blogspot
---

This is a second post on using OpenCL on the Chromebook ARM. The previous one gives instructions to [install OpenCL drivers and SDK on the Samsung Chromebook ARM](/blog/2013/11/23/opencl-on-samsung-chromebook-arm-under/), without requiring to boot a separate Ubuntu, by using [crouton](https://github.com/dnschneid/crouton). This post compares OpenMP and OpenCL performance of the Chromebook ARM with a 4-year old laptop.

### Test setup

In these tests, I compare my 4-year old Dell laptop with the Samsung ARM Chromebook. It's obviously not a very fair comparison: The laptop is quite obsolete now (and will actually be replaced soon). On the other hand, the Samsung ARM is a budget device, with a ridiculously low power consumption.

<table align="center" border="1" style="text-align: center;">
<tbody>
<tr><td></td><td><b>Dell Latitude E6400</b></td><td><b>Samsung Chromebook ARM</b></td></tr>
<tr><td><b>CPU</b></td><td>Intel Core2 Duo T9950 @ 2.66Ghz<br />(no hyperthreading)</td><td>Samsung Exynos 5 Dual (5250)<br />(Cortex A15; 1.7GHz dual core cpu)</td></tr>
<tr><td><b>RAM</b></td><td>4 GB</td><td>2 GB</td></tr>
<tr><td><b>GPU</b></td><td>NVIDIA Corporation G98M<br />(Quadro NVS 160M)<br />256MB dedicated RAM</td><td>ARM Mali T604<br />(quad core)</td></tr>
<tr><td><b>OS</b></td><td>Archlinux<br />(gcc 4.8.2)</td><td>Ubuntu 12.04 in crouton<br />(gcc 4.6.3)</td></tr>
</tbody></table>

As a benchmark test suite, we use [Rodinia](https://www.cs.virginia.edu/~skadron/wiki/rodinia/index.php/Main_Page), a CUDA/OpenMP/OpenCL test suite from the University of Virginia.

The test suite does not compile unmodified, and for some OpenCL tests, the number of threads need to be reduced to fit in the limited memory of both computers. Complete instructions and patches can be found in [my github repository](https://github.com/drinkcat/rodinia-benchmark).

### Test results - OpenMP

First we show comparisons using OpenMP, that only makes use of the CPU. We expect the Intel laptop to be far superior, and this is what we get:

{% include img.html src="/images/opencl-on-samsung-chromebook-arm/results-1.png" alt="OpenMP benchmark results" %}

The ARM CPU alone is between 1.75 and 4.5 times slower (average: 2.87 times slower). Not a big surprise considered the lower frequency and simpler architecture. On the other hand, the Chromebook stays cool (no fan!), while the Dell laptop blows out hot air on the side.

### Test results - OpenCL

We can then compare the GPUs, using OpenCL tests in Rodinia:

{% include img.html src="/images/opencl-on-samsung-chromebook-arm/results-2.png" alt="OpenCL benchmark results" %}

And this is where we get a nice surprise: the ARM GPU is very close in performance in most tests. Excluding ParticleFilter, which is 8 times slower, the GPU is, in the worst case, 1.94 times slower, and it is even 1.8 times faster in the Kmeans test (average: 1.25 times slower).

For some reason, I could not get the OpenCL code to compile on the Samsung ARM for LavaMD (it fails with CL_INVALID_KERNEL_ARGS), but I didn't try very hard. Let me know if you find a way! It would also be interesting to figure out why ParticleFilter is so slow.

### Test results correctness

The benchmark timings need to be taken with a bit a precaution, in case the results are garbled. Some tests do not produce any output, so it's hard to tell if the computation is correct. On the other hand, hotspot produce some output that can be plotted:

{% include img.html src="/images/opencl-on-samsung-chromebook-arm/hotspot.png" alt="Hotspot output comparison" %}

As you can see, the results look identical in all cases.

To analyse the differences more precisely, we can measure some average numerical error between data results x and y of 2 different implementations, as follows:

<img src="/images/opencl-on-samsung-chromebook-arm/error-formula.png" alt="Error formula" style="display: block; margin: auto;" />

And a summary of these errors, for 2 of the tests, where some output is created:

<table align="center" border="1" style="text-align: center;">
<tbody>
<tr><td><b>Test</b></td><td><b>Variable</b></td><td colspan="3"><b>Error</b></td></tr>
<tr><td></td><td></td><td>x86 OpenMP<br />vs<br />x86 OpenCL</td><td>x86 OpenMP<br />vs<br />ARM OpenMP</td><td>x86 OpenCL<br />vs<br />ARM OpenCL</td></tr>
<tr><td><b>HotSpot</b></td><td>output</td><td>1.132563e-06</td><td>0</td><td>3.017828e-09</td></tr>
<tr><td rowspan="3"><b>CFD</b></td><td>density</td><td><span style="background-color: red;">249862.2</span></td><td>0</td><td>1.143825e-07</td></tr>
<tr><td>density_energy</td><td><span style="background-color: red;">253706.2</span></td><td>0</td><td>1.060202e-07</td></tr>
<tr><td>momentum[1]</td><td><span style="background-color: red;">322420.1</span></td><td>0</td><td>1.065893e-07</td></tr>
</tbody></table>

As you can see, running the OpenMP code on ARM and x86 gives identical results. The OpenCL results are also very close. That basically means the comparisons above between the 2 laptops is fair.

When it comes to differences between OpenMP and OpenCL code, the HotSpot test shows a good agreement between the 2 versions. On the other hand, the CFD test outputs very different results between the 2 implementations. This is worrisome as CFD is one of the tests that shows most improvement using OpenCL compared to OpenMP...

### Test results - overall

The next graph shows all the results aggregated.

{% include img.html src="/images/opencl-on-samsung-chromebook-arm/results-0.png" alt="Overall benchmark results" %}

Assuming OpenCL and OpenMP implementations give similar results (which is actually doubtful in some cases, see the previous section), running OpenCL code on the Samsung Chromebook ARM can help a lot in terms of performance: On the Dell laptop, using OpenCL improves performance by a factor 2.25 on average. On the Chromebook ARM, the ratio is 4.1! And this is without any attempt at optimizing the code for the Mali architecture, which is quite different from a normal GPU (in particular, it has no local memory, so data does not need to be copied back and forth).

I'd also like to try some real applications, my next project is to get [darktable](http://www.darktable.org/) running (a RAW photo developer). Do let me know if you have some real applications using OpenCL! I'll follow up with another post if I get them to work.
