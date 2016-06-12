/***************************************************************************
                          BenchmarkException.h  -  exception class for the
                          benchmark suite.
                             -------------------
    begin                : Fri Jun 29 2001
    copyright            : (C) 2001 by 
    email                : cbilder@usgs.gov
    author               : Chris Bilderback
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/ 

#ifndef BENCHMARKEXCEPTION_H
#define BENCHMARKEXCEPTION_H


#include <string>

namespace USGSBenchmark
{

#define BENCH_UNKOWN_ERR  "USGSBench: Unkown Error"

/**
 *An exception class for use with
 *the benchmarking suite
 **/
class BenchmarkException
{
public: 
  /**
   *Default constructor which creates a
   *"Unkown Error" message
   **/
  BenchmarkException();

  /**
   *Secondary constructor takes a string message
   *as an error.
   **/
  BenchmarkException(const std::string & inErrorMesg);

  /**
   *Secondary constructor takes a c-string argument
   **/
  BenchmarkException(const char * inErrorMesg);

  /**
   *Default destructor for the class
   **/
  virtual ~BenchmarkException();

  /**
   *Returns the error message for this class
   **/
  std::string getError() const throw();

  /**
   *Returns the error message as a c string
   **/
  const char * getcstrError() const throw();

  /**
   *Allows the user to set a message
   **/
  void setError(std::string inErrorMessage) throw();

protected:
  std::string err;    //the error message
};

}//namespace
#endif
