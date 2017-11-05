#include <iostream>
#include <fstream>
#include <climits>
#include <random>
#include "Network.h"

/*
 *  01234--i--->nb_neurons
 * 1                |
 * 2                |
 * 3    TARGETS     |
 * |                |
 * v                |
 * nb_connections___|
 */

/**
 * The neuron are stored in a vector of size nb_neurons.
 * 
 * The connections are represented with a matrix containing the
 * address of all the neuron connected.
 * 
 * To connect the two, we say that the neuron i is sending his spikes
 * to all the targets in the column i.
 * 
 * The constructor of Network, it generates an uniform distribution
 * to create the connections between the neurons. It will store the spikes
 * in a file to later draw a plot with the data.
 */
 
Network::Network()
:Neurons_ (nb_neurons_), Targets_ (nb_neurons_,std::vector<int>(nb_connections_))
{
	
	const long int nb_connections_ (nb_neurons_/10) ;
	
	std::cout<<"Generating simulation environment" << std::endl ;
	
	std::random_device rd_n_ ;
	std::mt19937 gen_n_(rd_n_()) ;
	std::uniform_int_distribution<> dis(0 , (nb_neurons_-1)) ;
	
	for (size_t i = 0; i < nb_neurons_ ; i++)							//loop to fill the vector of targets
	{
		for (int j = 0; j < nb_connections_ ; j++)
		{	
			Targets_[i][j] = dis(gen_n_) ;
		}
	}
	
	std::cout<<"Done" << std::endl ;
}	


/**
 * @brief The function to initialize the current and the time interval.
 * 
 * It asks the user to enter the time interval and the current while
 * checking that the user doesn't enter unusable values
 */

void Network::InitializeExtActivity()
{
//-------------Initialisation of I--------------------------------------

	std::cout << "Enter a value for the current " ;
	
	//CONTROL OF THE VALUES (I > 0)
	
	do
	{
		std::cin >> Iext ;
		if (std::cin.fail())
		{
			std::cout << "ERROR : enter a positive value" << std::endl ;
			std::cin.clear() ;
			std::cin.ignore(INT_MAX, '\n') ;
			Iext = -1 ;													//setting this value to be sure that the loop is still going on
		}
	}while(Iext < 0.0) ;
	
//-------------Initialisation [start;stop]------------------------------
	
	std::cout << "Enter a time interval "<<std::endl ;
	std::cout << "When does the current start ? (number between 0 and "<< nb_steps_<<" ) " ;
	
//-------------Control Tstart : 0=<Tstart<Nbsteps-----------------------
	
	do
	{
		std::cin >> step_start_ ;
		std::cout << std::endl ;
		if((std::cin.fail()) or (step_start_ < 0) or (step_start_ > nb_steps_))
		{
			std::cout << "Error : enter a positive integral number between 0 and "<< nb_steps_ << std::endl ;
			std::cin.clear() ;
			std::cin.ignore(INT_MAX, '\n') ;
			step_start_ = -1 ;											//setting this value to be sure that the loop is still going on
		}
	}while((step_start_ < 0) or (step_start_ > nb_steps_)) ;
	
	std::cout << "When does the current stops ? (number of steps between starting time and "<<nb_steps_<<" ) ";
	
//-------------Initialisation Tstop : Tstart<Tstop<=Nbsteps------------- 
	
	do
	{
		std::cin >> step_stop_ ;
		std::cout << std::endl ;
		if((std::cin.fail()) or (step_stop_ < step_start_) or (step_stop_ > nb_steps_))
		{
			std::cout << "Error : enter a positive integral number between 0 and "<<nb_steps_<<" " << std::endl ;
			std::cin.clear() ;
			std::cin.ignore(INT_MAX, '\n') ;
			step_stop_ = -1 ;											//setting this value to be sure that the loop is still going on
		}
	}while((step_stop_ <= 0) or (step_stop_ > nb_steps_)) ;
}
	
/** 
 * It begins by creating a poisson distribution to simulate the external
 * spikes. Then it updates every Neuron and each time a neuron spikes
 * the address and the step are written in a document.
 * 
 * Note 1: The neurons 1 to 10000 are excitatory and the 10001 to 12500 
 * are inhibitory.
 * 
 * Note 2: This used to write potentials of all the neurons in a file, but
 * since it was not used anymore and slowed the program it was set as 
 * commentary.
 * 
 */ 
	
void Network::Run()
{

/*-------------Opening potentials file----------------------------------

	std::ofstream Potentials_ ;
	Potentials_.open("Potentials.txt") ;
*/
//-------------Opening python file--------------------------------------

	std::ofstream Python_ ;
	Python_.open("GraphPython4.txt") ;

//-------------Generation of poisson distribution-----------------------
	
	std::random_device rd_p_ ;
	std::mt19937 gen_p_(rd_p_()) ;
	std::poisson_distribution<> poisson_(lambda_) ;

//-------------Simulation start-----------------------------------------
	
	std::cout<<"Simulation start" << std::endl ;
	
	while ( current_step_<= nb_steps_)									//loop to run the simulation 
	{	
//-------------Checking when the external activity starts---------------
		
		for (size_t i = 0; i < Neurons_.size() ; i++)					//loop to update the neurons
		{
			if((current_step_ < step_start_) or (current_step_ > step_stop_))
			{
			
//-------------Update without external activity-------------------------
			
				Neurons_[i].UpdateState(0, 0) ;
			
			}else{
		
//-------------Update with external activity----------------------------
		
				Neurons_[i].UpdateState(I, poisson_(gen_p_)) ;
				
			}
			
			//Potentials_<<Neurons_[i].GetPotential()<< '\t' ;			//used to write the potentials in a file
			
//-------------Checking if the neuron spiked----------------------------
			
			if(Neurons_[i].HasSpiked(current_step_) )
			{
				
				Python_ << current_step_ << '\t' << i << std::endl ;
					
				if (i < ((4*nb_neurons_/5)))	
				{
						
//-------------Sending spike to all post synaptic neurons---------------
						
					for (int j = 0; j <nb_connections_ ; j++)			//loop to send the spike to all the post synaptics neurons
					{
							
						Neurons_[i].SendSpike(Neurons_[Targets_[i][j]],true) ;
							
					}
					
				}else{
					
					for (int k = 0;k < nb_connections_ ; k++)
					{
							
						Neurons_[i].SendSpike(Neurons_[Targets_[i][k]],false) ;
							
					}
				}
			}
		};																//end of the update loop
	//Potentials_<< std::endl ;
	
	
//-------------Incrementing the simulation time-------------------------

		current_step_ += 1 ;
		
	}; 																	//end of the simulation loop
	
//-------------Closing the file------------------------------------------
	
	//Potentials_.close() ;
	Python_.close() ;
	
	std::cout<<"Simulation terminated" << std::endl;
}
