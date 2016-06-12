/***************************************************************************
                          ServerBenchSlave.h  -  definition for the server
                          bench mark slave which acts as a client to connect
                          to the server.
                             -------------------
    begin                : Thu Jul 12 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/ 

#ifndef SERVERBENCHSLAVE_H
#define SERVERBENCHSLAVE_H

#include "PvmBenchmark.h"

namespace USGSBenchmark
{

#define SERVERBENCHSLAVETAG "ServerBenchSlave"

/**
 *This is the server bench slave that is
 *used during the server benching marking program
 *in which multiple slaves connect to a single master and the
 *time per access is recorded on the master and the average
 *access time is recorded for each slave.
 **/
class ServerBenchSlave : public PvmBenchmark
{
public:
  /**
   *Main constructor for the server slave benchmarking class
   **/
  ServerBenchSlave();

  /**
   *Main destructor for the server slave benchmarking class
   **/
  virtual ~ServerBenchSlave();

  /**
   *This is the entry point for each of the different
   *benchmarking classes. Overloaded it to create a new
   *custom benchmark.
   **/
  virtual bool run()
     throw(BenchmarkException);

  /**
   *This function must be overloaded by clients to supply
   *benchmark configuration.
   *This is not need by this class.
   **/
  virtual bool generateConfig()
    throw(BenchmarkException)
    {return true;}

  /**
   *This function must be overloaded by clients to handle output.
   *This is not needed by this class
   **/
  virtual bool outputResults()
    throw(BenchmarkException)
    {return true;}

protected:
  int mastertid;     //the server tid
};

}//namespace
#endif
