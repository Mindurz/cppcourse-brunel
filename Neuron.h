#include <vector>
#include <array>

#ifndef NEURON_H
#define NEURON_H

class Neuron 
{
	private :
	
	double potential_ ;
	unsigned int nb_spikes_ ;
	std::vector < int > spikes_time_ ;
	int local_step_ = 0;					//current step of the neuron
	static const int delay_step_ = 15 ;
	std::array <int, delay_step_+1> ring_buffer_ ;
	
	//constants FIND A BETTER WAY !! bad for user interface !!
	static constexpr double tau_ = 20 ;
	static constexpr double threshold_ = 20;
	static constexpr double c_ = 1 ;
	static constexpr int refractorytime_ = 20;
	
	public :
	
	Neuron () ;
	Neuron (double pot_, unsigned int nb_/*, vector < double > sp_times_*/) ;
	~Neuron () ;
	
	double GetPotential() ;
	unsigned int  GetNbspikes() ;
	std::vector< int > GetSpikestime() ;
	bool IsRefractory(int step) ;
	bool HasSpiked(int step) ;
	void SendSpike (Neuron& n) ;
	void StoreSpike() ;
	
	void UpdateState(double dt_, double I) ;
	
	void Printlocaltime() ;
};

#endif
