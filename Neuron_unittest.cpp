#include <iostream>
#include "Neuron.h"
#include "gtest/gtest.h"
#include <cmath>

static constexpr double tau_ = 20 ;									
static constexpr double threshold_ = 20;							
static constexpr double c_ = 1 ; 									
static constexpr int refractorytime_ = 20; 							
	
static constexpr double C1_ = exp(-0.1/tau_) ;
static constexpr double C2_ = (tau_ / c_) * (1 - C1_) ;
/**
 * Testing if update does what it is supposed to do
 */
TEST (NeuronTest, MembranePotential)
{
	Neuron neuron ;
	neuron.UpdateState(1.0,0) ;
	EXPECT_EQ(neuron.GetPotential(),( 1.0 * C2_ )) ;
}

/**
 * Testing if the function HasSpiked works
 */
TEST (NeuronTest, HasSpiked)
{
	Neuron neuron ;
	neuron.UpdateState(300,0) ;
	
	EXPECT_EQ(neuron.HasSpiked(1), true) ;
}

/**
 * Testing if sending a excitatory spikes acts the way it should
 */
TEST (NeuronTest, SendSpikeExc)
{
	Neuron neuron1 ;
	Neuron neuron2 ;

	neuron1.SendSpike(neuron2, true) ;
	
	EXPECT_EQ(neuron2.GetBuffer()[15], 1) ;
}

/**
 * Testing if sending a inhibitory spikes acts the way it should
 */
TEST (NeuronTest, SendSpikeInh)
{
	Neuron neuron1 ;
	Neuron neuron2 ;
	
	for (int i = 1; i <9 ; i++)
	{
		neuron1.UpdateState(0.0,0) ;
		neuron2.UpdateState(0.0,0) ;
	}

	neuron1.SendSpike(neuron2, false) ;
	
	EXPECT_EQ(neuron2.GetBuffer()[7], -5) ;
}

/**
 * Testing if receiving a excitatory spikes acts the way it should
 */
TEST (NeuronTest, ReceiveSpikeExc)
{
	Neuron neuron ;
	neuron.ReceiveSpike(3, true) ;
	EXPECT_EQ(neuron.GetBuffer()[3], 1) ;
}

/**
 * Testing if receiving a inhibitory spikes acts the way it should
 */
TEST (NeuronTest, ReceiveSpikeInh)
{
	Neuron neuron ;
	neuron.ReceiveSpike(9, false) ;
	EXPECT_EQ(neuron.GetBuffer()[9], -5) ;
}

/**
 * Testing if the delay is working properly
 */
TEST (NeuronTest, Delay)
{
	Neuron neuron1 ;
	Neuron neuron2 ;
	
	for (int i = 0; i < 200; i++)
	{
		neuron1.SendSpike(neuron2, true);
	}
	
	int y (0) ;
	
	do
	{
		neuron2.UpdateState(0,0) ;
		++y ;
	}while(not neuron2.HasSpiked(y)) ;
	
	EXPECT_EQ(y, 15) ;
}

/**
 * Testing if a neuron doesn't spike with a current of 1.0
 */
TEST (NeuronTest, NoSpike)
{
	Neuron neuron;
    for(size_t i(0); i < 2000; ++i)
    {
        neuron.UpdateState(1.0, 0); 
    }
    EXPECT_EQ(neuron.GetNbSpikes(), 0); 
    EXPECT_NEAR(neuron.GetPotential(), 20, 0.01); 
}

/**
 * Testing if a neuron spikes with a current of 1.01
 */
TEST (NeuronTest, SpikeLimit)
{
	Neuron neuron;
    for(size_t i(0); i < 940; ++i)
    {
        neuron.UpdateState(1.01, 0);
    }
    EXPECT_EQ(neuron.GetNbSpikes(), 1) ;
}

int main (int argc, char **argv) 
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
