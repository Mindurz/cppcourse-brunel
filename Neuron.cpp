#include "Neuron.h"

//-------------Constructors---------------------------------------------

/**
 * Everything is initialized at 0 but, since the buffer is an array 
 * it has random values initiated in each cases. Each time a neuron is 
 * built this make sure every cases is initialized at 0.
 */

Neuron::Neuron()														///< Base constructor of a neuron
:potential_(0.0), nb_spikes_(0), spikes_time_() 						
{																		
	for (size_t i = 0; i < (delay_step_+1) ; i++)									
	{																	
		ring_buffer_[i] = 0 ;											
	}																	
	
}

/**Same comment as the basic constructor here.
 * 
 * @param pot the starting membrane potential of the neuron
 */

Neuron::Neuron (double pot_)											//specific constructor to initialize
:potential_(pot_)														//the potential and the number of
{																		//spikes of a neuron. This was created for testing
	for (size_t i = 0; i < (delay_step_+1) ; i++)						//purpose.
	{																	
		ring_buffer_[i] = 0 ;											
	}																	
}

//------------Destructor------------------------------------------------

Neuron::~Neuron()
{}

//------------Getters---------------------------------------------------

double Neuron::GetPotential() 											
{																		
	return potential_ ;
}

unsigned int  Neuron::GetNbSpikes()										
{																		
	return nb_spikes_ ;
}

std::vector<int> Neuron::GetSpikestime() 								
{																		
	return spikes_time_ ;
}

std::array<int, 16> Neuron::GetBuffer()									///< gives the current state of the buffer Used only as a control medium
{																		
	return ring_buffer_;
}

//------------Utilitary-------------------------------------------------

/**
 * 		This function tells you if a neuron is still refractory at a certain
 * 		step it checks 2 things
 * 
 * a) It checks if the vector empty (avoid a segfault)
 * if it is empty the neuron is not refractory.
 * 
 * b) It checks if the difference between the current time 
 * (the argument) and the time of the last spike smaller than the 
 * refractory period of a neuron if this is true the neuron is 
 * refractory, else it is not and the potential can evolve.
 * 
 * @param step the step at which we want to know if the neuron is
 * refractory
 * 
 * @return true -> refracory
 * @return false -> not refractory
 */

bool Neuron::IsRefractory(int step)
{
	if (spikes_time_.empty())											// avoid a seg fault
	{
		return false ;
	}
	if(step - spikes_time_.back() < refractorytime_)					// condition of a refractory neuron
	{
		return true ;
	}
	return false ;
}

/**
 * 		Function to tell if a neuron has spiked at the time t
 * 
 * 1) It checks if the vector containing all the spikes is empty, 
 * to avoid a seg fault
 * 
 * 2) It checks if the argument is equal to the argument if it is then
 * it returns true else false.
 * 
 * To use this just put the actual time of the simulation as an argument
 * and it will tell you the neuron spiked at that time
 * 
 * @param step the step at which we want to check if the neuron 
 * has spiked.
 * 
 * @return return true if the neuron did spike at the step, else return
 * false
 */

bool Neuron::HasSpiked(int step)
{
	if (spikes_time_.empty())
	{
		return false ;
	}
	if(step == spikes_time_.back())
	{
		return true ;
	}
	return false ;
}

/*! \brief Send a spike to a post synaptic neuron with a delay
 * 
 * @param n a Neuron, the target
 * @param exc a boolean describing,the type of the neuron sending the 
 * spike excitatory = true, inhibitory = false.
 */

void Neuron::SendSpike(Neuron& n,bool exc)
{
	n.ReceiveSpike((local_step_)+delay_step_,exc) ;
}

/*! 
 * Store a spike in the buffer to use it in UpdateState after a certain 
 * delay.
 * @param step the step at which the spike was received
 * @param exc a boolean describing the type of the neuron sending the 
 * spike
 */

void Neuron::ReceiveSpike(int step,bool exc)
{
	if(exc)
	{
		ring_buffer_[(step) % (delay_step_+1)] += Se_ ;
	}else{
		ring_buffer_[(step) % (delay_step_+1)] += Si_ ;
	}
}

/*!
 * Update a neuron from the time t to the time t+dt depending of a
 * certain outside activity like a continuous external current or a 
 * number of spike happening around the neuron
 * @param I the current
 * @param spikes_ext_ the external activity
 */

void Neuron::UpdateState(double I,int spikes_ext_) 
{
	
//---------------Refractory check---------------------------------------
	
	if (IsRefractory(local_step_)) 										// if a neuron is refractory his pot = 0, 
	{																	
		
//---------------Potential update of a refractory neuron----------------
		
		potential_ = 0.0 ;
	}else{ 																// else we use the formula
		
//---------------Potential update of a non refractory neuron------------													//local_step_+1 because the neuron is going from t to t+dt 
																															//if we don't put the +1 we are checking one case before 
																															//the time we want, hence a wrong delay
		potential_ = C1_* potential_ + I * C2_ + (ring_buffer_[(local_step_+1) % (delay_step_+1)] + spikes_ext_)*J ; 
				
//---------------Checking if the neuron is spiking----------------------
				
		if (potential_ >= threshold_)
		{
			spikes_time_.push_back(local_step_ + 1) ;					//storing the spike in the vector of spikes
			nb_spikes_ += 1 ;											//incrementing th number of spikes
		};
		
//---------------Reseting the current slot of the buffer to 0-----------
		
		ring_buffer_[local_step_ % (delay_step_+1)] = 0 ;				// if we don't reset the value of the buffer it will stay during the whole program
		
	};
	
//---------------Incrementing the local time of the neuron--------------
	
local_step_ += 1 ;														
}
