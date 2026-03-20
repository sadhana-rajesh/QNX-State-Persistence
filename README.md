# QNX State Persistence and Recovery

## Project Overview
In embedded systems, unexpected power cuts or system crashes can lead to data loss. This project implements a recovery module in QNX RTOS to ensure system state is continuously saved and restored automatically after reboot.

## Technical Implementation
- Multithreading: Used POSIX threads (pthreads) with Worker and Logger threads running in parallel.
- Thread Safety: Implemented mutex locks (pthread_mutex_lock) to prevent race conditions.
- Fail-Safe Saving: Used atomic file write strategy with temporary file + fsync() + rename() to ensure data integrity.

## How it Works
- On startup, the system checks for an existing state file.
- If found, it loads the last saved value (e.g., Counter = 180%).
- Worker thread continues execution from that state.
- Logger thread saves progress every 2 seconds in the background.

## Environment and Tools
- OS: QNX Neutrino RTOS
- Language: C
- IDE: QNX Momentics IDE
- Libraries: pthreads, fcntl.h, unistd.h
