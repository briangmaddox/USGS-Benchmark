/***************************************************************************
                          PvmBenchmark.cpp  -  description
                             -------------------
    begin                : Tue Jul 17 2001
    copyright            : (C) 2001 by 
    email                : cbilder@mcpc1159
 ***************************************************************************/

/***************************************************************************
 * This software is free under the terms of the MPL (Mozilla Public License*
 * See www.mozilla.org for details.                                        *
 ***************************************************************************/

#include "PvmBenchmark.h"

using namespace USGSBenchmark;

//***************************************************************************
PvmBenchmark::PvmBenchmark(const std::string & inid) : Benchmark(inid)
{
  //enroll in pvm
  ctid = pvm_mytid();
  //check to see that pvm exits
  if (ctid < 0)
    throw BenchmarkException("PvmBenchmark::PvmBenchmark-no pvm! Shazbot!");


}

//***************************************************************************
PvmBenchmark::~PvmBenchmark()
{
  //exit pvm
  pvm_exit();
}

//***************************************************************************
void PvmBenchmark::send(std::list<Time_stat> & time_list, int tid, char* data,
  long int data_size, int msgtag) throw(BenchmarkException)
{
  int sendcode(0);
  struct Time_stat tm_stat;

  // get the start time
  if (!timer.start())
    throw BenchmarkException
      ("Benchmark::send-Unable to use timer");

  pvm_initsend(PvmDataDefault);

  if (data_size)
    pvm_pkbyte(data, data_size, 1);

  sendcode=pvm_send(tid, msgtag);

  if(sendcode < 0)
  {
    throw BenchmarkException("Benchmark::send-Unable to send message");
  }
 
  // get the finish time
  if (!timer.stop(tm_stat.lapse_tm))
    throw BenchmarkException("Benchmark::send-Unable to get stop time");
  
  //put it in the time_list
  time_list.push_back(tm_stat);

}

//***************************************************************************
bool
PvmBenchmark::send_recv(std::list<Time_stat> & time_list, int tid, int msgtag,
char* data, long int data_size, char* rdata, long int rdata_size )
throw(BenchmarkException)
{
  int sendcode(0);
  int  bufferid(0), len(0), tag(0),    //pvm tags
  temptid(0);
  struct Time_stat tm_stat;

  // get the start time
  if(!timer.start())
    throw BenchmarkException
      ("Benchmark::send_recv-Unable to use timer");

  pvm_initsend(PvmDataDefault);
  if (data_size)
    pvm_pkbyte(data, data_size, 1);
  sendcode=pvm_send(tid, msgtag);

  if(sendcode < 0)
    throw BenchmarkException("Benchmark::send_recv-Unable to send message");

  bufferid = pvm_recv(-1, -1);
  pvm_bufinfo(bufferid, &len, &tag, &temptid);

  if (tag == BENCH_QUIT || tag == BENCH_ERR)
  {
    return false;   //we are not timing any more
  }

  //check for return data
  if (rdata_size && rdata)
  {
    pvm_upkbyte(rdata, rdata_size, 1);
  }

  //get the finishing time
  if(!timer.stop(tm_stat.lapse_tm))
    throw BenchmarkException
      ("Benchmark::send_recv-Unable to get stop time");

  //put it in the time_list
  time_list.push_back(tm_stat);

  return true; //got a valid timing
}
