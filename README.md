# Métodos Numéricos - Project Nº1

This project consists on an implementation of a program to compute the required isotherm of a [blast furnace](https://en.wikipedia.org/wiki/Blast_furnace), by knowing its wall's radius and temperature.

What's in here:

- C++ code for the algorithms that solves the main task.
- Makefile for the executable file.
- Python Notebook for creating 3 datasets.
	- `generar-instancias.ipynb`
- Python Notebook for running experiments.
	- `experimentos.ipynb`
- Python Notebook for plotting and analyzing results.
	- `analisis.ipynb`

Where :

```
TP
├─README.md 
├─.gitignore 
├─src 
│  ├─Makefile 
│  ├─main.cpp 
│  ├─auxiliares.h 
│  ├─auxiliares.cpp 
│  ├─algoritmos.h 
│  └─algoritmos.cpp 
│
└─python 
   ├─generar-instancias.ipynb 
   ├─experimentos.ipynb 
   └─analisis.ipynb 
```

## Usage

To compile everything, go to the `src` folder run the following command:
```
make
```

After that, you can already process an instance of the problem by indicating an input and output text file, and selecting an algorithm to use of the following:

- `0` : `Gauss`
- `1` : `LU`

With the intended format, run:
```
./executable input_file output_file 0
```
(`Gauss` is being used on the above example)

## Experiments

To replicate the experiments an analysis, it's recommended to install [Jupyter Lab or Jupyter Notebook](https://jupyter.org/).

With this, it's possible to run the python notebooks and replicate the process.
















