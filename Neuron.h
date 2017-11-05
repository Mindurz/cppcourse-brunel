#include <vector>
#include <array>
#include <cmath>

/**@brief The class Neuron to emulate one active neuron
 * 
 * 
 */

#ifndef NEURON_H
#define NEURON_H

class Neuron 
{
	private :
	
//------------Attributes------------------------------------------------
	
	double potential_ ; 												///< The membrane potential
	unsigned int nb_spikes_ ; 											///< The number of spikes the neuron did
	std::vector < int > spikes_time_ ;									///< Vector storing times at which the neurons spiked
	int local_step_ = 0;												///< Current step of the neuron
	static const int delay_step_ = 15 ;									///< Delay between when the spike is send and when it is received
	std::array <int, delay_step_+1> ring_buffer_ ; 						///< Buffer to implement the delay
	static constexpr double J = 0.1 ; 									///< Amplitude
	static constexpr int Se_ = 1 ;										///< Output of an excitatory spike
	static constexpr int Si_ = -5 ;										///< Output of an inhibitory spike	
	
//------------Constants-------------------------------------------------
	
	static constexpr double tau_ = 20 ;									///< Tau
	static constexpr double threshold_ = 20;							///< Threshold
	static constexpr double c_ = 1 ; 									///< capacitance
	static constexpr int refractorytime_ = 20; 							///< Time after a spike inwhich a neuron is inactive
	
	static constexpr double C1_ = exp(-0.1/tau_) ;						
	static constexpr double C2_ = (tau_ / c_) * (1 - C1_) ;
	
	
//------------Constructors and destructors------------------------------
	
	public :
	
	Neuron () ;															///< Basic constructor
	Neuron (double pot_) ;												///< Specific constructor
	~Neuron () ;														///< Base destructor
	
//------------Getters---------------------------------------------------
	
	double GetPotential() ;												///< Gives the potential of the neuron at the current time
	unsigned int  GetNbSpikes() ;										///< Gives the number of spikes the neuron did up to the current time
	std::vector< int > GetSpikestime() ;								///< Gives every times at which the neuron spiked
	std::array<int, delay_step_+1> GetBuffer() ;
	
//------------Utilitary-------------------------------------------------
	
	bool IsRefractory(int step) ;										///< Check if the neuron is still in a refractory period
	bool HasSpiked(int step) ;											///< Check if the neuron has spiked at a certain time
	void SendSpike (Neuron& n,bool exc) ;								///< Send the spikes to a post synaptic neuron with a delay
	void ReceiveSpike(int step,bool exc) ;								///< Receive a spike in the buffer
	
//------------Update----------------------------------------------------
	
	void UpdateState(double I,int spike_ext_) ;							///< Update the neuron from t to t+dt
	
};

#endif
