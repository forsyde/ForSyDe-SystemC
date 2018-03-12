---
layout: default
title: SDF Tutorial
permalink: sdf-tutorial
---


# The ForSyDe-SystemC Tutorial for the Synchronous Data Flow MoC

This tutorial is a walk-through for modeling systems in the the Synchronous Data Flow (SDF) Model of Computation (MoC) using the [ForSyDe-SystemC]() library. It assumes that the user has already installed the SystemC and the [ForSyDe-SystemC]() libraries and knows how to build and run a model on her computer. Also, it is assumed that the user has gone through the [tutorial for the SY MoC](sy-tutorial), thus we will focus only on the specific features of the SDF MoC.

In ForSyDe-SystemC, a system is modeled as a hierarchical process network where processes communicate only via signals, making it similar to a data-flow style of modeling. Although SystemC elements are present in this framework, the user is advised to adopt only the ForSyDe terminology in designing systems in order to fully exploit the advantages of formalism provided by the ForSyDe methodology.

This tutorial will present a toy example for understanding the concepts of designing an SDF system. As shown in the picture below, this system consists of an up-sampler, a running averager and a down-sampler.

<p align="center">
<img width="700px" src="assets/svg/sdf-example.svg">
</p>


## The Synchronous Data Flow (SDF) MoC 

Before proceeding with implementing the first system using the SDF MoC of [ForSyDe-SystemC](), a few basic assumptions derived from the SDF semantics need to be clarified:

 * just as the SY MoC, the SDF MoC is _untimed_, thus there is no notion of physical time described in model.
 * processes communicate via messages called _tokens_.
 * each process has fixed production _P_ and consumption _C_ rates.
 * a SDF process is triggered once it inputs _C_ tokens (unlike a SY process which is triggered by a master event). Once it is triggered, it consumes the _C_ tokens and produces _P_ tokens.

To understand the usage of the SDF MoC, the user might consider that a main class of applications modelled as SDF systems are telecommunications applications where processes communicate in different clock domains through specific protocols. Also, streaming applications are targeted since their main target is the control of the data flow.

For more information about the concepts behind the SDF MoC, the user is encouraged to consult the world wide web. A good introduction may be found [here](http://www.artist-embedded.org/docs/Events/2006/MoCC_Zurich/geilen-MoCC06.pdf).

## Signal

As seen in the [SY tutorial](sy-tutorial#signals), signals are the only means with which the processes can communicate and synchronize. In the SDF MoC, signals are carriers for the data containers associated with tokens. A signal is typed and it is instantiated with the class `ForSyDe::SDF::signal` using the template:

{% highlight cpp %}
ForSyDe::SDF::signal<double> my_sig;
{% endhighlight %}

or the template.

{% highlight cpp %}
ForSyDe::SDF::SDF2SDF<double> my_sig;
{% endhighlight %}

Both instantiations are equivalent and create a signal `my_sig` which carries tokens encapsulating data of type `double`.

## Processes

The processes in ForSyDe are described [here](ForSyDe), and have been presented in the [SY tutorial](sy-tutorial#processes). Their usage in the SDF MoC of [ForSyDe-SystemC]() is similar to the one in SY MoC. The main differences will be presented in the following paragraphs.

### Leaf processes 
The process constructors for the leaf processes are found in the [ForSyDe::SDF](api/namespace_for_sy_de_1_1_s_d_f.html) namespace. Also, as described in their [API documentation](api/namespace_for_sy_de_1_1_s_d_f.html), to instantiate a leaf process, apart from the process-specific parameters, the user must pass the number of tokens for each input port and for the output port. An example of process instantiation for the `avg1` process in the figure above is:

{% highlight cpp %}
        comb2<double,double,double> *avg1 = new SDF::comb2<double,double,double>("avg1", avg_func, 2, 3, 2);
	avg1->iport1(iport);
	avg1->iport2(dout);
	avg1->oport1(oport);
	avg1->oport1(din);
{% endhighlight %}

The same result is obtain by using the `make_comb2` helper function to reduce code verbosity (see the [SY tutorial](sy-tutorial#processes)):

{% highlight cpp %}
        auto avg1 = SDF::make_comb2("avg1", averager_func, 2,3,2, oport, iport, dout);
        avg1->oport1(din);
{% endhighlight %}

The helper function constructs a standard `comb2` ForSyDe process which has only one output. The line `avg1->oport1(din)` specifies that the same output port `oport1` of `avg1` is used as input for the signal `din`. Also, as suggested in the picture above, the number of tokens are specified for `avg1->oport1` (2),  `avg1->iport1` (3) and `avg1->iport2` (2).

The function passed as a parameter for the `avg1` process (of type `avg1`) called `avg_func` describes the desired functionality:

{% highlight cpp %}
void avg_func(std::vector<double>& out1,
                   const std::vector<double>& inp1,
                   const std::vector<double>& inp2)
{
#pragma ForSyDe begin avg_func
    out1[0] = (inp1[0]+inp1[1]+inp2[0])/3;
    out1[1] = (inp1[1]+inp1[2]+inp2[1])/3;
#pragma ForSyDe end
};
{% endhighlight %}

The ForSyDe-SystemC convention for passing the process ports as function parameters is still respected. The convention imposes that the list of parameters should start with the output port, followed by the input ports in ascending order of their indexes.

The process ports are passed to the function as vectors and, unlike in the [SY MoC](sy-tutorial), decapsulation is not needed outside the `#pragma` preprocessors. Each token is represented by one element of the vector, thus accessing the tokens is like accessing the vector elements (for example `inp1[0]`).

Inside the preprocessors `#pragma ForSyDe begin` and `#pragma ForSyDe end` the user is advised to write [standard C code](http://users.ece.cmu.edu/~eno/coding/CCodingStandard.html), since most of the design flows associated with [ForSyDe-SystemC]() directly map that code on C-based backends.

### Composite processes 
The composite processes are the same as in the [SY tutorial](sy-tutorial#processes). The only difference is that, since now the signals which pass through them are SDF signals, the input an output ports now derive from `SDF::in_port`, respectively from `SDF::out_port`. Below you can find an example for designing the `compAvg1` composite process, as suggested in the picture.

{% highlight cpp %}
#include <forsyde.hpp>
#include "averager.hpp"

using namespace ForSyDe;

SC_MODULE(compAvg)
{
    SDF::in_port<double>  iport;
    SDF::out_port<double> oport;
    
    SDF::signal<double> din, dout;
    
    SC_CTOR(compAvg)
    {
        auto averager1 = SDF::make_comb2("avg1", avg_func, 2,3,2, oport, iport, dout);
        avg1->oport1(din);
        
        SDF::make_delayn("avginit1",0.0,2, dout, din);
    }
};

{% endhighlight %}


## Simulation

How to test a ForSyDe-SystemC design has been presented in the [SY tutorial](sy-tutorial#processes). The user needs to provide inputs and observe the outputs with the help of procuder/consumer processes specific to (in our case) the SDF MoC. For example, the top module for our toy example:

{% highlight cpp %}
#include "compAvg.hpp"
#include "upSampler.hpp"
#include "downSampler.hpp"
#include "report.hpp"
#include "stimuli.hpp"
#include <iostream>

using namespace ForSyDe;

SC_MODULE(top)
{
    SDF::signal<double> src, upsrc, res, downres;
    
    SC_CTOR(top)
    {
        SDF::make_source("stimuli1", stimuli_func, 0.0, 10, src);
      
        SDF::make_comb("us1", us_func, 2, 1, upsrc, src);

        auto compAvg1 = new compAvg("compAvg1");
        compAvg1->iport1(upsrc);
        compAvg1->oport1(res);

        SDF::make_comb("ds1", ds_func, 2, 3, downres, res);
        
        SDF::make_sink("report1", report_func, downres);
    }
};
{% endhighlight %}

As shown in the [SY tutorial](sy-tutorial#processes), two new processes have been instantiated: a [source process](api/classForSyDe_1_1SDF_1_1source.html) called `stimuli1` which produces a stream of 10 tokens generated by the function `stimuli_func` and a [sink process](api/classForSyDe_1_1SDF_1_1sink.html) called `report1` which consumes the tokens generated by `ds1` and prints them out through its function `report_func`. The code for `stimuli_func` and `report_func` is:

{% highlight cpp %}
void stimuli_func(double& out1, const double& inp1)
{
#pragma ForSyDe begin stimuli_func
    out1 = inp1 + 1;
#pragma ForSyDe end
}
{% endhighlight %}

{% highlight cpp %}
void report_func(double inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}
{% endhighlight %}

To verify the design we have to compile the code in a set-up environment suggested in the [installation instructions](setup). Running the resulting binary, we get the following output:

            SystemC 2.3.0-ASI --- Dec 11 2013 15:10:22
            Copyright (c) 1996-2012 by all Contributors,
            ALL RIGHTS RESERVED
    
    output value: 0
    output value: 0.333333
    output value: 1.44444
    output value: 2.33333
    output value: 3.77778
    output value: 4.2716
    output value: 5.7572
    output value: 6.75309
    
which is the expected output for our toy system since we know that the source has produced the stream `[1 2 3 4 5 6 7 8 9 10]`.

The full code can be checked out from the [ForSyDe-SystemC repository]({{ site.github.url }}), and compiled and run from the user's machine.

## Introspection

The notion of _Introspection_ has been presented in the [SY tutorial](sy-tutorial#introspection). This feature can be activated by adding the following lines after declaring the processes inside the module that needs to be parsed (in our case the top module in `top.hpp`):

{% highlight cpp %}
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
{% endhighlight %}

Note that the library (and the above model) are enabled for introspection only when the `FORSYDE_INTROSPECTION` macro is defined, either in the source code or as a compiler switch (`-DFORSYDE_INTROSPECTION`). Assuming the existence of the `gen/` sub-folder, the executable model writes to XML files named `top.xml` and `compAvg.xml` to this folder, each representing one level of hierarchy. The XML files synthesize the structural characteristics of the system which can be extracted by other tools in the design flow. For example, the tool [[f2dot]] is able to generate the following representation:


<p align="center">
<img width="700px" src="assets/images/top_FullStruct_vert.svg">
</p>
