#include "Neuron.h"
#include <cmath>
#include <iostream>
#include <vector>

using namespace std ;

Neuron::Neuron()
:potential_(0.0), nb_spikes_(0), spikes_time_() 
{
	for (size_t i = 0; i < 16 ; i++)
	{
		ring_buffer_[i] = 0 ;
	}
	
}

Neuron::Neuron (double pot_, unsigned int nb_/*, vector<double> sp_times_*/)
:potential_(pot_), nb_spikes_(nb_)/*, spikes_time_(sp_times_) */
{
	for (size_t i = 0; i < 16 ; i++)
	{
		ring_buffer_[i] = 0 ;
	}
}

/*
Neuron::Neuron(Neuron n)
:potential_(n.GerPotential()), nb_spikes_(n.GetNbspikes()), spikes_time_(n.GetSpikestime()) 
{}
*/

Neuron::~Neuron()
{}

double Neuron::GetPotential() 
{
	return potential_ ;
}

unsigned int  Neuron::GetNbspikes() 
{
	return nb_spikes_ ;
}

vector<int> Neuron::GetSpikestime() 
{
	return spikes_time_ ;
}

bool Neuron::IsRefractory(int step)
{
	if (spikes_time_.empty())
	{
		return false ;
	}
	if(step - spikes_time_.back() < refractorytime_)
	{
		return true ;
	}
	return false ;
}

bool Neuron::HasSpiked(int step)
{
	if (spikes_time_.empty())
	{
		return false ;
	}
	if(step == spikes_time_.back())
	{
		cout<<"spike at "<<spikes_time_.back()<<"  and the step is "<<step<<endl ;
		return true ;
	}
	return false ;
}

void Neuron::SendSpike(Neuron& n)
{
	n.StoreSpike() ;
	cout<<"HALP  " << local_step_+delay_step_<<" MEH "<< (local_step_+delay_step_)%(delay_step_+1)<<"  PLIZ  "<<local_step_%(delay_step_+1)<<endl;
	for (size_t i = 0; i < 16 ; i++)
	{
		cout<<n.ring_buffer_[i] ;
	}
	cout<<endl;
}

void Neuron::StoreSpike()
{
	ring_buffer_[(local_step_+delay_step_) % (delay_step_+1)] += 1 ;
	
	cout<<"modulo = "<<((local_step_+delay_step_) % (delay_step_+1))<< "  et le step est  "<<local_step_<<endl;
	
	for (size_t i = 0; i < 16 ; i++)
	{
		cout<<i<<"    "<<ring_buffer_[i]<<endl;
	}
}

void Neuron::UpdateState(double dt_, double I) 
{
	if (IsRefractory(local_step_)) //neuron refractory-> pot = 0 sinon on applique la formule
	{
		potential_ = 0.0 ;
	}else{ 
		potential_ = exp(-dt_/tau_) * potential_ + I * (tau_ / c_) * (1 - exp(-dt_/tau_)) + ring_buffer_[local_step_ % (delay_step_+1)]*threshold_ ; 
		
		//checking if the neuron is spiking
		
		if (potential_ >= threshold_)
		{
			spikes_time_.push_back(local_step_ + 1) ;
			nb_spikes_ += 1 ;
		}
		
		//reseting the slot of the buffer to 0
		
		ring_buffer_[local_step_ % (delay_step_+1)] = 0 ;
		
	}
local_step_ += 1 ;
}

void Neuron::Printlocaltime()
{
	cout<<"local time is "<<local_step_<<endl;
}
