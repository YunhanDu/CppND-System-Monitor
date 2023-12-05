#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
long Processor::CurTotal() {
  return LinuxParser::Jiffies();
}
long Processor::CurActive() {
  return LinuxParser::ActiveJiffies();
}
long Processor::CurIdle() {
  return LinuxParser::IdleJiffies();
}

long Processor::PreTotal() {
  return m_total;
}
long Processor::PreActive() {
  return m_active;
}
long Processor::PreIdle() {
  return m_idle;
}
void Processor::Update(long idle, long active, long total) {
  m_idle = idle;
  m_active = active;
  m_total = total;    
}
float Processor::Utilization() {
  long curTotal = CurTotal();
  long preTotal = PreTotal();

  long curIdle = CurIdle();
  long preIdle = PreIdle();
  Update(curIdle, CurActive(), curTotal);
  float deltaTotal = float(curTotal - preTotal);
  float deltaIdle = float(curIdle - preIdle);
  return (deltaTotal - deltaIdle) / deltaTotal;
}