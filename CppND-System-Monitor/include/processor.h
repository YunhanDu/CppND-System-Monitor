#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  long CurTotal();
  long CurActive();
  long CurIdle();

  long PreTotal();
  long PreActive();
  long PreIdle();
  void Update(long idle, long active, long total);

  // TODO: Declare any necessary private members
 private:
  long m_idle;
  long m_active;
  long m_total;
};

#endif