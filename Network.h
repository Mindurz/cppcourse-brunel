#include "Neuron.h"

/**@brief The class Network to emulate a group of active 
 * neurons connected to form a network
 * 
 * This can run a simulation for a certain number of neurons and a
 * certain interval of time.
 */

#ifndef NETWORK_H
#define NETWORK_H

class Network
{
	private:
	
///-------------Attributes----------------------------------------------
	
	int step_start_ = 0 ;												///< Starting time of the external activity
	int step_stop_ = 0 ;												///< Stopping time of the external activity
	int current_step_ = 0 ;												///< The current time of the simulation
	double I = 0.0 ;													///< A variable needed to store the current
	double Iext = 0.0;													///< The external current
	double h = 0.1; 													///< The interval of time in each step
	int nb_steps_ = 10000 ;												///< The number of steps of the simulation
	const unsigned int nb_neurons_ = 12500 ; 						///< The number of neurons
	const int nb_connections_ = nb_neurons_/10 ;						///< The number of connections between every neurons
	const double lambda_ = 2 ;											///< The variable used in teh poisson distribution
	
	std::vector <Neuron> Neurons_ ;										///< A vector representing all the neurons
	std::vector <std::vector<int> > Targets_ ;							///< A matrix representing the connections between the neurons
	
	public :
	
	Network() ;															///< Basic constructor
	void InitializeExtActivity() ;										///< Initialize the current and time interval
	void Run() ;														///< Runs the simulation
	
};

#endif
