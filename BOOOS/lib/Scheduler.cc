#include "Scheduler.h"
#include <iostream>
#include "Task.h"

namespace BOOOS {

Scheduler * Scheduler::__dispatcher;

Scheduler::Scheduler() :
		Task(dispatcher, 0, 0) {
	Task::_state = SCHEDULER;
}

Scheduler::~Scheduler() {
	free(__dispatcher);
}

void Scheduler::init() {
	if (!__dispatcher)
		__dispatcher = new Scheduler();
}

void Scheduler::dispatcher(void*) {
	Task * next;
	while (__ready.size() != 0) {
		next = self()->choose_next();
		if (next) {
			self()->pass_to(next, WAITING); // transfere controle para a tarefa "next"
		}
	}
	self()->pass_to(Task::__main, Task::FINISHING);
}

Task * Scheduler::choose_next() {
	Task * next = __ready.front();
	__ready.pop();
	return next;
}

} /* namespace BOOOS */
