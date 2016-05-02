## Synopsis

This project is (or aims to be):

1. A solution to the [Problem B](http://cad-contest-2016.el.cycu.edu.tw/problem_B/default.html) of the ICCAD 2016 Contest.

2. A verilog netlist parser.

4. A verilog netlist nand-synthesizer and JIT compiler, for very fast simulation.

5. A ***ORBDD*** (Ordered Reduced Binary Decision Diagrams) creation and manipulation framework. ORBDD represent combinational circuits in canonical form, that is, the ORBDDs of equivalent predicates are equal. For this reason, ORBDDs are specialy useful to check equivalence of combinational circuits.


## Code Examples

Since the project is in its beginnings, there is not much in this section.


## Motivation

Testing circuits is not easy and we need more tools to do it.

## Usage

Nothing much can be done yet.

## API Reference

Since API is not yet defined, it is not wise yet to publish it.

## Installation

This projects aims to reduce the amount of depencies to the most. However it needs cmake and boost spirit 3.0.

To compile it, run the following commands:

```
cmake .
make
```


## Tests

Currently there is only a stub test. 
To run it, run the following command:

```
cd tests && ctest
```

## Contributors




## License

The current code is available under BSD3 licese. 

>
>Copyright (c) 2016, Emilio Wuerges
>
>All rights reserved.
>
>Redistribution and use in source and binary forms, with or without
>modification, are permitted provided that the following conditions are met:
>
>    * Redistributions of source code must retain the above copyright
>      notice, this list of conditions and the following disclaimer.
>
>    * Redistributions in binary form must reproduce the above
>      copyright notice, this list of conditions and the following
>      disclaimer in the documentation and/or other materials provided
>      with the distribution.
>
>    * Neither the name of Emilio Wuerges nor the names of other
>      contributors may be used to endorse or promote products derived
>      from this software without specific prior written permission.
>
>THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
>"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
>LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
>A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
>OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
>SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
>LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
>DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
>THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
>(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
>OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
>
