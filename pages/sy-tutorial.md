---
layout: default
title: SY Tutorial
permalink: sy-tutorial
---

# The ForSyDe-SystemC Tutorial for the Synchronous MoC #

This tutorial is a walk-through for modeling systems in the Synchronous (SY) Model of Computation (MoC) using the ForSyDe-SystemC library. It assumes that the user has already installed the SystemC and the ForSyDe-SystemC libraries and knows how to build and run a model on her computer.

In ForSyDe-SystemC, a system is modeled as a hierarchical process network where processes communicate only via signals, making it similar to a data-flow style of modeling. We will introduce different elements of the SY MoC in ForSyDe-SystemC with examples.

In order to avoid confusion, note that although ForSyDe-SystemC uses the SystemC kernel to simulate the models, the modeling style is different from SystemC. Many elements of the SystemC language are not present[^1], and the ones which are used may appear in a different terminological context.

The running example for this tutorial is a simple multiply-accumulator system with two inputs and a single output. The main input is a ramp signal and the other input fixes the multiplication coefficient.


<p align="center">
<img width="800px" src="assets/svg/sy-tutorial.svg">
</p>


[^1]: or in better words, are not allowed to be used in ForSyDe-SystemC

## The Synchronous (SY) MoC ##

Before proceeding with implementing the first system using the SY MoC of ForSyDe-SystemC, a few basic concepts need to be clarified:

 * the SY MoC describes no quantitative notion of time thus it assumes that neither communication nor computation takes time.
 * two events may be either synchronous (they happen at the same moment) or one unambiguously precedes the other.
 * as other synchronous programming languages do ([Lustre](http://www-verimag.imag.fr/Synchrone), [Esterel](http://www.esterel.org/), [Argos](http://www-verimag.imag.fr/~maraninx/spip.php?article22), etc), [ForSyDe]({{ site.parent-url }}) describes events as ''present'' or ''absent''. A key property is that the order of these events is absolute and unambiguous.
 * two signals can be considered synchronous if all events in one signal are synchronous with the events from the other signal and vice-versa.
 * the SY MoC is widely used for modelling control systems or digital systems, where the design ignores (abstracts) timing details.

A good example to understand the SY MoC is the design style of sequential digital circuits in HDLs. The designer assumes of a master ''clock'' event which triggers the registers. By doing so, it enables the internal processes which are assumed to execute in ''delta delay'' (zero delay), thus abstracting the notion of physical time.

For more information about the concepts behind the SY MoC, the user is encouraged to consult the world wide web. A short and comprehensive introduction may be found [here](http://chess.eecs.berkeley.edu/design/2010/lectures/ee249-oct10-synchronous.pdf), and [here](http://web.it.kth.se/~axel/MESS-Book/Slides/lec-06-SyncMod.pdf).

## Signals ##
Signals are the only means with which the processes can communicate and synchronize together. Each signal must be bound to an input and an output port of a ForSyDe process. We will see how to do this later in this tutorial. Signals are typed. In the SY MoC, signals are defined using the class `ForSyDe::SY::SY2SY` (or `ForSyDe::SY::signal`) and the token carried by a signal can be either *absent*, denoting an absent event or a value of the signal type. There is a helper (template) class ´abst_ext<T>´ which is used to represent the absent-extended values, i.e., values which can be either absent, or present with a value of type `T`.  For example:

{% highlight cpp %}
ForSyDe::SY::SY2SY<double> my_sig;
{% endhighlight %}

defines a signal called `my_sig` which carries tokens of type `abst_ext<double>`. The `abst_ext` class has a set of accompanying methods and functions which can be used to check for and set presence of values.

## Processes ##
Processes are the basic computational elements in ForSyDe-SystemC. In the general case, a process receives its inputs via input signals, performs some calculations on them, and communicates the result as a single output. This single output may be an ''n''-tuple which can be later unzipped using another process. Hence, this does not affect the generality of the case. Each process belongs to a single MoC which governs how the data is communicated to, and out of a process. A process in a process network is either a ''leaf process'', which is a leaf of the process network hierarchy, or a ''composite process'', which is the result of interconnection of other basic and composite processes. In the SY MoC, leaf processes can be classified either as ''combinational'' if they do not include any internal state, or ''sequential'' if they do.

### Leaf Processes ###
Leaf processes are created using [process constructors](). Process constructors are templates provided by the library that are parameterized in order to create a process. The parameters to a process constructor can be initial values (e.g., initial states) or functions. From the C++ point of view, creating a leaf process out of a process constructor is equivalent to instantiating a C++ class and passing the required parameters to its constructor. Definition of functions used to pass to each process constructor usually happen in a separate header file for each process.

Consider the case of a simple process representing a multiplier which multiplies its two integer inputs, and has an integer output that represents the result. There is no internal state involved in this example which makes it a combinational leaf process. Looking at the [ForSyDe-SystemC library documentation]({{ site.api-doc }}), we can find that the process constructor for a two input combinational process is `ForSyDe::SY::scomb2`. It requires a function to be passed to it, which applied in each evaluation cycle to the inputs in order to produce the outputs. The function is defined in `mul.hpp` as:

{% highlight cpp %}
#ifndef MUL_HPP
#define MUL_HPP

#include <forsyde.hpp>

using namespace ForSyDe;

void mul_func(int& out1, const int& inp1, const int& inp2)
{
#pragma ForSyDe begin mul_func  
    out1 = inp1 * inp2;
#pragma ForSyDe end
}

#endif
{% endhighlight %}

The pragma statements enclose the core functionality of the function and can be used by external analysis/synthesis tools which later on operate on the intermediate representation generated by ForSyDe-SystemC models. They can be safely ignored for simulation-only purposes.

Now, in order to create the multiplier process (probably as a part of a composite process) we can have:

{% highlight cpp %}
...
SY::scomb2<int,int,int>* mul1 = SY::scomb2<int,int,int>("mul1", mul_func);
...
{% endhighlight %}

The types of inputs and the output are specified as template parameters of the class. The name of the process (SystemC module) together with the multiplication function are passed as parameters of the class constructor in the order defined in the [library API]({{ site.api-doc }}).

Processes are connected to signals using their ports. As for basic process constructors, port names are automatically chosen inside the library according to the following scheme. Output ports of the basic process constructors are called `oport1`, `oport2`, and so on. Similarly, input ports are also named as `iport1`, `iport2`, etc.

### Composite Processes ###
More complex processes, can be constructed by composing leaf or other composite processes. A composite process is simply the result of instantiation of other processes and wiring them together using signals. In order to have a valid ForSyDe model, a set of rules should be respected. Remember that if we ignore some of these rules, we may still be able to simulate our model using the SystemC kernel. But, the result will not benefit from the ForSyDe benefits such as formal analysis, composability, etc.

 * A composite process is in fact a SystemC module derived from the `sc_module` class.
 * A composite process is the result of instantiation and interconnection of other valid ForSyDe processes, no ad-hoc SystemC processes or modules are allowed.
 * Ports of all child processes in a composite process are connected together using signals of SystemC channel type `ForSyDe::SY::SY2SY` (`ForSyDe::SY::signal`).
 * A composite process in the includes zero or more inputs and output ports of types `ForSyDe::SY::in_port` and `ForSyDe::SY::out_port` (or `ForSyDe::SY::SY_in` and `ForSyDe::SY::SY_out` equivalently).
 * If an input port of a composite process should be connected to several child processes, an additional fanout process (i.e., `ForSyDe::SY::fanout`) is needed in between.

Let us put all these information into action and build a composite multiply-accumulator (MAC) process out of other basic processes.

{% highlight cpp %}
#ifndef MULACC_HPP
#define MULACC_HPP

#include <forsyde.hpp>
#include "mul.hpp"
#include "add.hpp"

using namespace ForSyDe;

SC_MODULE(mulacc)
{
    SY::in_port<int>  a, b;
    SY::out_port<int> result;
    
    SY::signal<int> addi1, addi2, acci;
    
    SC_CTOR(mulacc)
    {
        SY::scomb2<int,int,int> *mul1 = new SY::scomb2<int,int,int>("mul1", mul_func);
	mul1->iport1(a);
	mul1->iport2(b);
	mul1->oport1(addi1);

        SY::scomb2<int,int,int> *add1 = new SY::scomb2<int,int,int>("add1", add_func);
	add1->iport1(addi1);
	add1->iport2(addi2);
	add1->oport1(acci);
	add1->oport1(result);

	SY::sdelay<int> *accum = new SY::sdelay<int>("accum", 0);
	accum->iport1(acci);
	accum->oport1(addi2);
    }
};

#endif
{% endhighlight %}

Creating the adder and the delay processes is similar to creating the multiplier process shown above, but based on the `comb3` and `comb4` process constructors. The SystemC-provided macros `SC_MODULE` and `SC_CTOR` are used to create a SystemC module that plays the role of a ForSyDe composite process. Ports of appropriate types are specified at the beginning followed by declaration of interconnection signals. A signal of type `ForSyDe::SY::signals` is needed for each point-to-point connection with according type. In the constructor of the composite process (`SC_CTOR`), the child processes are created, followed by their interconnection using the intermediate signals.

### Reducing the Code Verbosity ###
Looking at the definition of the `mulacc` composite process presented above, we can identify a couple of points where we are repeating ourself which makes the code verbose. Among them:

 1. Class template type parameters for `comb2`- and `delay`-based processes are repeated twice. In fact these types can be inferred by the type of signals connected to their inputs and outputs.
 1. During process instantiation in each case, we bind all the input ports and the output port of the processes to at least one signal.

By exploiting the type-inference mechanism in the C++(11) language, ForSyDe-SystemC library provides helper functions to construct leaf processes and bind a channel to their inputs and outputs. The names of these helper functions are in the form `make_xyz`, where `xyz` is the name of the original process constructor. In addition to the parameters taken by the process constructor, these helper functions take one signal for each of the process inputs/outputs as arguments and binds it to the respective port. Additional signal binding can still be done using the port names. The `mulacc` composite process can be rewritten as:

{% highlight cpp %}
#ifndef MULACC_HPP
#define MULACC_HPP

#include <forsyde.hpp>
#include "mul.hpp"
#include "add.hpp"

using namespace ForSyDe;

SC_MODULE(mulacc)
{
    SY::in_port<int>  a, b;
    SY::out_port<int> result;
    
    SY::signal<int> addi1, addi2, acci;
    
    SC_CTOR(mulacc)
    {
        SY::make_scomb2("mul1", mul_func, addi1, a, b);

        auto add1 = SY::make_scomb2("add1", add_func, acci, addi1, addi2);
        add1->oport1(result);
        
        SY::make_sdelay("accum", 0, addi2, acci);
    }
};

#endif
{% endhighlight %}

The **auto** keyword is a newly-added feature of the latest [C++11 standard](https://en.wikipedia.org/wiki/C%2B%2B11) which in this case automatically infers the type of the `add1` process (pointer).

## Test-benches and Simulation ##
In order to test a process, one can simply look at the top level process as a design under test, provide inputs according to the semantics of the MoC that it belongs to and observe the outputs. There exist processes in each MoC that only produce/consume values and can be used for this purpose.

Consider the case of our MulAcc example. A very simple testbench can be constructed by providing a constant integer value 3 to the first input and a ramp signal starting from 1 to the second input. Below you can see how we can implement this simple testbench for our MulAcc example.

{% highlight cpp %}
#ifndef TOP_HPP
#define TOP_HPP

#include <forsyde.hpp>
#include "mulacc.hpp"
#include "siggen.hpp"
#include "report.hpp"

using namespace ForSyDe;

SC_MODULE(top)
{
    SY::signal<int> srca, srcb, result;
    
    SC_CTOR(top)
    {
        SY::make_sconstant("constant1", 3, 10, srca);
        
        SY::make_ssource("siggen1", siggen_func, 1, 10, srcb);
        
        auto mulacc1 = new mulacc("mulacc1");
        mulacc1->a(srca);
        mulacc1->b(srcb);
        mulacc1->result(result);
        
        SY::make_ssink("report1", report_func, result);
    }
};

#endif
{% endhighlight %}

The `constant` process constructor has only one output. It takes an initial value and the number of ticks to run (and no functions) as the parameter and produces a process which outputs this value in each cycle. The `source` process constructor is more powerful and also takes a function (shown below) additionally. It repeatedly applies the function to the initial value to generate the next output.

{% highlight cpp %}
#ifndef SIGGEN_HPP
#define SIGGEN_HPP

#include <forsyde.hpp>

void siggen_func(int& out1, const int& inp1)
{
#pragma ForSyDe begin siggen_func
    out1 # inp1 + 1;
#pragma ForSyDe end
}

#endif
{% endhighlight %}

The `sink` process constructor has only one input. It takes a function as its parameter and passes the received input to this function.

{% highlight cpp %}
#ifndef REPORT_HPP
#define REPORT_HPP

#include <forsyde.hpp>
#include <iostream>

using namespace ForSyDe;

void report_func(int inp1)
{
#pragma ForSyDe begin report_func
    std::cout << "output value: " << inp1 << std::endl;
#pragma ForSyDe end
}

#endif
{% endhighlight %}
To run the simulation, we need to instantiate the top level composite process and run the SystemC simulation kernel.

{% highlight cpp %}
#include "top.hpp"

int sc_main(int argc, char **argv)
{
    top top1("top1");

    sc_start();
        
    return 0;
}
{% endhighlight %}
Compilation of the project is just like any other C++ application. Depending on the way you have built your SystemC library (statically or dynamically), you compile the code and link against SystemC to yield a single executable file. By running the resulting executable file, we are in fact simulating our system. For example, in a Unix-based shell we could have:


    user@host:~/code/mulacc$ ./run
    
                 SystemC 2.3.0-ASI --- Oct 31 2012 09:07:54
            Copyright (c) 1996-2012 by all Contributors,
            ALL RIGHTS RESERVED
    output value: 3
    output value: 9
    output value: 18
    output value: 30
    output value: 45
    output value: 63
    output value: 84
    output value: 108
    output value: 135
    output value: 165
    user@host:~/code/mulacc$
    
## Introspection ##
A distinguishing feature of the !ForsyDe-SystemC library is the ability of the constructed models to exports their  own internal structure in a machine-readable intermediate representation. This enables the designers to easily pass the validated (by simulation) models to external analysis and synthesis tools, without the need for building a C++ front-end. By adding a SystemC hook to the start of the simulation phase of SystemC for the top model of the design, the executable models dump their internal process network structure in an XML format.

The top-level model of the mulacc design is modified below to export the model structure.

{% highlight cpp %}
#ifndef TOP_HPP
#define TOP_HPP

#include <forsyde.hpp>
#include "mulacc.hpp"
#include "siggen.hpp"
#include "report.hpp"

using namespace ForSyDe;

SC_MODULE(top)
{
    SY::signal<int> srca, srcb, result;
    
    SC_CTOR(top)
    {
        SY::make_sconstant("constant1", 3, 10, srca);
        
        SY::make_ssource("siggen1", siggen_func, 1, 10, srcb);
        
        auto mulacc1 # new mulacc("mulacc1");
        mulacc1->a(srca);
        mulacc1->b(srcb);
        mulacc1->result(result);
        
        SY::make_ssink("report1", report_func, result);
    }
#ifdef FORSYDE_INTROSPECTION
    void start_of_simulation()
    {
        ForSyDe::XMLExport dumper("gen/");
        dumper.traverse(this);
    }
#endif
};

#endif
{% endhighlight %}

Note that the library (and the above model) are enabled for introspection only when the `FORSYDE_INTROSPECTION` macro is defined, either in the source code or as a compiler switch (`-DFORSYDE_INTROSPECTION`). Assuming the existence of the `gen/` sub-folder, the executable model writes to XML files named `top.xml` and `mulacc.xml` to this folder, each representing one level of hierarchy.

As a simple usage, for visualization purpose, we have converted these XML files into [DOT](https://en.wikipedia.org/wiki/DOT_(graph_description_language)) format using the tool [f2dot]({{ site.parent-url }}/f2dot). The results are displayed below. These graphs are comparable withe the figure presented in the beginning of the tutorial.

<p align="center">
<img width="800px" src="assets/images/top.svg">
</p>
