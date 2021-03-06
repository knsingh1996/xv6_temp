#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_myexit(void)
{
  int _stat;

  if(argint(0, &_stat) < 0)
    return -1;

  myexit(_stat);
  return 0;
}

int
sys_wait(void)
{
  return wait();
}

int
sys_mywait(void)
{
  int* _stat;

  if(argptr(0, (char **) &_stat, sizeof(int*)) < 0)
    return -1;

  return mywait(_stat);
}

int
sys_waitpid(void)
{
  int* _stat;
  int _pid, _options;

  if(argint(0, &_pid) < 0)
    return -1;

  if(argptr(1, (char **) &_stat, sizeof(int*)) < 0)
    return -1;

  if(argint(2, &_options) < 0)
    return -1;

  return waitpid(_pid, _stat, _options);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_setpriority(void)
{
  int _priority;
  if(argint(0, &_priority) < 0)
    return -1;
  return setpriority(_priority);
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
