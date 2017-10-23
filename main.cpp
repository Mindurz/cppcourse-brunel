#include "Neuron.h"
#include <iostream>
#include <fstream>
#include <climits>

int main()
{
	//double current_t_ (0) ;
	int step_start_ (0) ;
	int step_stop_ (0) ;
	//double t_final_ (0);
	int current_step_(0) ;
	double I (0.0) ;
	double Iext (0.0);
	double h (0.1); // the interval of time in each step
	int nb_steps_ (400) ;
	
	//INITIALISATION OF I
	
	std::cout << "Enter a value for the current " ;
	
	//CONTROL OF THE VALUES (I > 0)
	
	while(Iext <= 0.0)
	{
		std::cin >> Iext ;
		if (std::cin.fail())
		{
			std::cout << "ERROR : enter a positive value" << std::endl ;
			std::cin.clear() ;
			std::cin.ignore(INT_MAX, '\n') ;
		}
	} ;
	
	//INITIALISATION OF [START ; STOP]
	
	std::cout << "Enter a time interval "<<std::endl ;
	std::cout << "When does the current start ? (number between 0 and "<< nb_steps_<<" ) " ;
	
	//CONTROL TSTART : 0=<TSTART<NBSTEPS
	
	do
	{
		std::cin >> step_start_ ;
		std::cout << std::endl ;
		if((std::cin.fail()) or (step_start_ < 0) or (step_start_ > nb_steps_))
		{
			std::cout << "Error : enter a positive integral number between 0 and "<< nb_steps_ << std::endl ;
			std::cin.clear() ;
			std::cin.ignore(INT_MAX, '\n') ;
			step_start_ = 0 ;
		}
	}while((step_start_ <= 0) or (step_start_ > nb_steps_)) ;
	
	std::cout << "When does the current stops ? (number of steps between starting time and "<<nb_steps_<<" ) ";
	
	//INITIALISATION TSTOP : TSTART<TSTOP<=NBSTEPS 
	
	do
	{
		std::cin >> step_stop_ ;
		std::cout << std::endl ;
		if((std::cin.fail()) or (step_stop_ < step_start_) or (step_stop_ > nb_steps_))
		{
			std::cout << "Error : enter a positive integral number between 0 and "<<nb_steps_<<" " << std::endl ;
			std::cin.clear() ;
			std::cin.ignore(INT_MAX, '\n') ;
			step_stop_ = 0 ;
		}
	}while((step_stop_ <= 0) or (step_stop_ > nb_steps_)) ;
	
	//INITIALISATION NEURONES
	
	std::vector <Neuron> Neurons_ ;
	Neuron mono ;
	Neurons_.push_back(mono) ;
	Neuron sono (10.0, 0 ) ;
	Neurons_.push_back(sono) ;
	
	//OPENING THE FILE TO WRITE POTENTIALS
	
	std::ofstream Project_Neuron ;
	Project_Neuron.open("Potentials.txt") ;
	Project_Neuron << "Neuron 1" << '\t' << "Neuron 2" << std::endl ;
	
	//SIMULATION START
	
	while ( current_step_<= nb_steps_)
	{	
		//CHECKING WHEN THE CURRENT START
		
		if((current_step_ < step_start_) or (current_step_ > step_stop_))
		{
			I = 0.0 ;
		}else{
			I = Iext ;
		}
		
		//UPDATING EACH NEURON
		
		for (size_t i = 0; i < Neurons_.size() ; i++)
		{
			Neurons_[i].UpdateState(h, I) ;
			Project_Neuron << Neurons_[i].GetPotential()<<'\t' << '\t' ;
			
			//sortir cette lepre de la boucle
			
			if (i == 0)
			{
				if(Neurons_[i].HasSpiked(current_step_))
				{
					Neurons_[i].SendSpike(Neurons_[i+1]) ;
				} ;
			} ;
			
			Neurons_[i].Printlocaltime() ;
			std::cout<<current_step_<<std::endl;
		} ;
		
		Project_Neuron << std::endl ;
		current_step_ += 1 ;
		

	} ; 
	
	Project_Neuron.close() ;
	
	return 0 ;
}
