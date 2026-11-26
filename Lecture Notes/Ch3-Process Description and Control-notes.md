# Chapter 3 Process Description and Control

## 1- Process Concept [slide 3-5]
Program: a passive executable stored on (disk) \
Process: an active instance/entity in the (memory) \
OS must:
- Interleave processes (run at same time)
- Allocate Resource & avoid deadlock
- Support IPC (InterProcess Communication)
- Process Creation

---
## 2- Process Trace & Dispatcher [slide 6-8]
A Process trace lists the sequece of instrcutions a process executes. \
Dispatcher: interleaves multiple processes on the CPU
- time-out (quantum exceed)
- I/O request (scanf, network/database request)

---
## 3- Process Creation & Termination [slide 9-13]
processes are created by:
- Batch jobs
- User logons
- OS services
- Parent Processes.

Processes are terminated due to:
- Normal completion
- User exit
- Runtime/memory/ IO errors
- OS/Parent Intervention تدخل

---
## 4- Two-State PRocess Model & Its Flaw [slide 14-17]
Processes are either running or not-running 

[❌Not Running] --dispatch--> [⚡️Running] --interrupt (quantum or IO) -> [❌Not Running] \

spliting (not running) to (ready) & (Blocked) \

[✅Ready] --dispatch-> [⚡️Run]

[⚡️Run] --(Quantum expired)-> [✅Ready]

[⚡️Run] --(interrupt IO)-> [🚫Blocked] 

[🚫Blocked] --(IO Completed)-> [✅Ready]


> ⚠️ Flaw: using a single Queue (FIFO) is inadequate, the dispatcher cannot tell whether a waiting process is ready to run or blocked on I/O 

---
## 5- Five-State Process Model [slide 18-25]
Expands the lifecycle into:
- New           (created process, not yet in memory)
- Ready         (waiting to be assigned to the processor)
- Running       (currently being executed)
- Blocked       (waiting for event ex: IO)
- Exit          (finished or aborted)

Running -> Ready:
1. quantum expired
2. OS assigned different priority (process move from blocked to running directly)

Running -> Blocked:
1. IO Request (scan, print, network access, file request)
2. IPC: a process waiting for another process

Blocked -> Ready:
1. when the event is done (IO Completed)

Running -> Exit:
1. when parent terminate the child
2. when parent itself is terminated (all childs exit)

Ready -> Running:
1. Process Dispatched

---
## 6- Queueing Models [slide 26-30]

seperate Blocked Queue to prevent moving all processes to ready state.

example: if a queue contain [Net,Net,Disk,Net,Disk] \
and Network IO is finished, then all processes will be moved to ready

but, if we have multiple queues, we will only move the desired processes to ready.

> No need to scan the entire queue !

---
## 7- Swapping [slide 31-43]

CPU is faster than IO ! (nano seconds vs milli seconds), CPU is IDLE (free) most of the time \
⚠️ CPU move the blocked processes to the memory, so the memory get full ! \
✅ Swapping: Move some of the blocked processes from the memory to the disk (based on priority). 

We add state (blocked/suspended) \
Blocked -> blocked/suspended: processes swapped from memory to disk \
blocked/suspended -> Ready: (activation), when I/O is finished


We add state (ready/suspended), to save memory: \
ready -> ready/suspended: processes swapped from memory to disk \
ready/suspended -> ready: activate (memory is available) ⚡️ \
blocked/suspended -> ready/suspended: I/O finished \
blocked/suspended -> blocked: activate (memory is available) ⚡️ \
New -> ready/suspended: when new, and no enough memory \
Running -> Ready: when quantum expired and no enough memory

---






