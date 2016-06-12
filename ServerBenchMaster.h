/***************************************************************************
                          ServerBenchMaster.h  -  Simple server bench marking
                          program
                             -------------------
    begin                : Tue Jul 3 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/ 

#ifndef SERVERBENCHMASTER_H
#define SERVERBENCHMASTER_H

#include "PvmBenchmark.h"


namespace USGSBenchmark
{

/**
 *This is the class id tag for the ServerBenchMark class
 **/
#define SERVERBENCHMASTERTAG "ServerBenchMaster"

/**
 *This is the master part on a simple master server benchmarking program
 *where a bunch of nodes try to connect a set number of times to
 *the "server" and recieve data.  The server then keeps track of the
 *the number of transactions per time period for the entire system.
 *The server also spawns all the slaves.
 **/
class ServerBenchMaster : public PvmBenchmark
{
public: 

  /**
   *Main constructor for the class
   *initializes member vars
   **/
  ServerBenchMaster();

  /**
   *Main destructor for the class
   **/
  virtual ~ServerBenchMaster();

  /**
   *This is the entry point for each of the different
   *benchmarking classes. Overloaded it to create a new
   *custom benchmark.
   **/
  virtual bool run()
     throw(BenchmarkException);

  /**
   *This function must be overloaded by clients to supply
   *benchmark configuration
   **/
  virtual bool generateConfig()
    throw(BenchmarkException) = 0;

  /**
   *This function must be overloaded by clients to handle output.
   **/
  virtual bool outputResults()
    throw(BenchmarkException) = 0;  

protected:

  /**
   *Spawn the slaves.  The explict flag tells it whether to use
   *explicit names of hosts or to just let pvm select the
   *slaves.
   **/
  void spawn_slaves() throw (BenchmarkException);


  int * childtid;               //the childrens tid
  int num_runs;                 //the number of runs to do the sim
  bool explicit_names;          //whether to explict names
  int number_slaves;            //the number of slaves to spawn
  std::string slave_args;       //the slave arguments
  std::string slave_exe;        //the slave executable
  long int data_size;           //the size of a the send buffers
  float mavg;                   //the master average result
  std::vector<float> savg;      //the slave average result
  
};

}//namespace
#endif
