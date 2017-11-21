/*
 * Task.h
 *
 *  Created on: Aug 15, 2014
 *      Author: arliones
 */

#ifndef TASK_H_
#define TASK_H_

#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

namespace BOOOS {

class Task {
	friend class Scheduler;
public:
	static int _count;
	static int count() {
		return _count;
	}
	enum State {
		READY, WAITING, RUNNING, FINISHING, SCHEDULER
	};
	static int __tid_counter;

	Task(void (*entry_point)(void*), int nargs, void * arg);
	Task(int prio, void (*entry_point)(void*), int nargs, void * arg);
	virtual ~Task();

	int tid() {
		return _tid;
	}
	State state() {
		return _state;
	}

	void pass_to(Task * t, State s = READY);

	void exit(int code);

	void yield();
	void nice(int priority);

	static Task * self() {
		return (Task*) __running;
	}
	static void init();

private:
	Task();
	static const int STACK_SIZE = 32768;

	static volatile Task * __running;
	static Task* __main;

	int _tid;
	ucontext_t _context;
	char* _stack;
protected:
	static std::queue<Task*> __ready;
	State _state;
};

} /* namespace BOOOS */

#endif /* TASK_H_ */
