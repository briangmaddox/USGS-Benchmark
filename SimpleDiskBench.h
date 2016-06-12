/***************************************************************************
                          SimpleDiskBench.h  -  Your classic read/write
                          disk I/0 benchmark.
                             -------------------
    begin                : Mon Jul 16 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/ 

#ifndef SIMPLEDISKBENCH_H
#define SIMPLEDISKBENCH_H


#include "Benchmark.h"

namespace USGSBenchmark
{

/**
 *This is the ID tag for the simple disk benchmark
 **/
#define SIMPLEDISKBENCHTAG "SimpleDiskBench"

/**
 *A classice write and then read disk I/O benchmark
 **/
class SimpleDiskBench : public Benchmark
{
public:
  /**
   *Main constructor for class initilizes data.
   **/
  SimpleDiskBench();

  /**
   *Main destructor for the class
   **/
  virtual ~SimpleDiskBench();

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
   *Read input reads class setup from the console input
   **/
  void readInput() throw(BenchmarkException);

  /**
   *Read config reads class setup from a configuration file
   **/
  void readConfig(const std::string & configfile) throw (BenchmarkException);


  std::string outputfile;              //output file name
  long int buffer_size;                //size of each write and read
  long int number_times;               //number of times to write and read
};

}//namespace
#endif
