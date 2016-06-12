/***************************************************************************
                          MpiBenchmark.h  -  base class for mpi benchmarks
                             -------------------
    begin                : Tue Jul 17 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/ 

#ifndef PVMBENCHMARK_H
#define PVMBENCHMARK_H

#include "Benchmark.h"
#include "MessageTags.h"
#include <vector>
#include "mpi.h"

namespace USGSBenchmark
{

/**
 *PVM base class for network benchmarking
 **/
class MpiBenchmark : public Benchmark
{
public: 
  /**
   *Main constructor to class
   **/
  MpiBenchmark(const std::string & inid);

  /**
   *Class destructor
   **/
  virtual ~MpiBenchmark();

  /**
   *This is the entry point for each of the different
   *benchmarking classes. Overloaded it to create a new
   *custom benchmark.
   **/
  virtual bool run()
     throw(BenchmarkException)=0;

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
   *This allows this program to send data to a particular node
   *and store the time it takes to send the data (enter, pack,and leave
   *the mpi send functions.
   **/
  void send(std::list<Time_stat> & time_list, int tid, char* data,
          long int data_size, int msgtag) throw(BenchmarkException);

  /**
   *This function allows a bench mark to send data to a node and
   *recvs (probably from the same node) and times the entire round
   *trip time. (if the quit tag is recieved then function returns false
   * else it returns true)
   **/
  bool send_recv(std::list<Time_stat> & time_list, int tid, int msgtag,
        char* data, long int data_size, char* rdata, long int rdata_size )
            throw(BenchmarkException);

  int ctid;                            //class pvm task id number
  std::vector<std::string> name_table; //host name table

};

}//namespace
#endif
