The final project.

To make the program with Cmake put the following files in a folder
Neuron.h,
Neuron.cpp,
Network.h,
Network.cpp,
main.cpp,
Neuron_unittest.cpp,
Cmakelists.txt,
Doxyfile.in,
gtest (folder)

Then use the commands "cmake ." and then "make" this will create 2 executables Neuron, the simulation and Neuron_unittest the tests of the functions.
To run the simulation use the command "./Neuron" and then enter the parameters, and wait :-), it takes around 50 seconds to run.
To run the tests use the command "./Neuron_unittest"

The repository also contains 5 "GraphPython.txt" one is a standard simulation (the one with no number). The other wher use to make the BrunnelPlots.pdf and they are linked like this:
1 -> A,2 -> B, 3 -> C, 4 -> D

