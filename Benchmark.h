/***************************************************************************
                          Benchmark.h  -  The base class for our beowulf
                                          benchmarking class.
                             -------------------
    begin                : Thu Jun 28 11:49:19 CDT 2001
    copyright            : (C) 2001 by
    email                : athmer@usgs.gov
    author               : Keith Athmer and Chris Bilderback
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <list>
#include <string>
#include "BenchmarkException.h"
#include "MiscUtils/MicroTimer.h"

namespace USGSBenchmark
{

/**
 *A glorified struct for taking
 *time values
 **/
class Time_stat
{
public:
  /**
   *Default constructor
   **/
  Time_stat() : start_tm(0), end_tm(0),
                lapse_tm(0)
   {}

  //public data
  unsigned long start_tm;
  unsigned long end_tm;
  unsigned long lapse_tm;
};

/**
 *The base class for particular pvm based benchmarks
 **/
class Benchmark
{
public:

  /**
   *Default constructor for the benchmarking base class
   *sets the id for the class
   **/
  Benchmark(const std::string & inid);

  /**
   *Default destructor for benchmarking class
   **/
  virtual ~Benchmark();

  /**
   *This returns the class id for that particular class
   **/
  std::string getId() const throw();

  /**
   *This is the entry point for each of the different
   *benchmarking classes. Overloaded it to create a new
   *custom benchmark.
   **/
  virtual bool run() throw(BenchmarkException)=0;

  /**
   *This function must be overloaded by clients to supply
   *benchmark configuration
   **/
  virtual bool generateConfig() throw(BenchmarkException) = 0;

  /**
   *This function must be overloaded by clients to handle output.
   **/
  virtual bool outputResults() throw(BenchmarkException) = 0;

protected:

  /**
   *This function simply computes the average of a list of time files
   *and return it.
   **/
  float average(const std::list<Time_stat> & time_list)
            const throw(BenchmarkException);


  std::string id;               //this the benchmark id
  MiscUtils::MicroTimer timer;  //used by all benchmarks to get times

};


}//namespace

#endif



