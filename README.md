# montecarlo-option-pricer

This is a small project to price one-fator call/put options using Monte Carlo

This project was started last week as part of my computational finance and C++ studies

It was designed to be modular and extensible
Currently it supports Plain, Lookback and average-rate Asian options
Stochastic differential equations are done using geometric brownian motion and the Euler method

The modules were written to be easy to implement Constant Elasticity of Variance and Milstein later on as well as new data input formats

The project make heavy use of templates, C++ 11 tuples and function types in the interfaces

All files were kept header-only for convenience

A more complete pricer is on the way, this one is just a baby
