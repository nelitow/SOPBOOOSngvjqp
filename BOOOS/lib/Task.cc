#include "Task.h"
#include "Scheduler.h"
#include <queue>

namespace BOOOS {

volatile Task * Task::__running;
Task * Task::__main;
int Task::__tid_counter;
int Task::_count = 0;
const int Task::STACK_SIZE;
std::queue<Task*> Task::__ready;

Task::Task(void (*entry_point)(void*), int nargs, void * arg) {
	getcontext(&this->_context);
	_stack = (char*) malloc(STACK_SIZE);
	if (_stack) {
		this->_context.uc_stack.ss_sp = _stack;
		this->_context.uc_stack.ss_size = STACK_SIZE;
		this->_context.uc_stack.ss_flags = 0;
		this->_context.uc_link = (ucontext*) -1;
	}
	makecontext(&this->_context, (void (*)()) entry_point, nargs, arg);
	this->_tid = __tid_counter++;
	this->_state = READY;
	__ready.push(this);
	_count++;
}

Task::Task(int priority, void (*entry_point)(void*), int nargs, void * arg) :
		Task(entry_point, nargs, arg) {
	_priority = priority;
}

void Task::nice(int priority) {
	this->_priority = this->_priority - priority;

	if (_priority < -20) {
		_priority = -20;
	}

	if (_priority > 19) {
		_priority = 19;

	}
	queue<Task*> aux;

	while (_priority_queue.size()) {
		Task * aTask = _priority_queue.top();
		aux.push(aTask);
		_priority_queue.pop();
	}

	while (aux.size()) {
		Task * aTask = aux.front();
		_priority_queue.push(aTask);
		aux.pop();

	}
	self()->yield();

}

Task::Task() {
	this->_stack = 0;
	getcontext(&_context);
	this->_tid = __tid_counter++;
	_state = READY;
	_count++;
}

Task::~Task() {
	free(this->_stack);
	__ready.pop();
}

void Task::init() {
	__tid_counter = 0;
	__main = new Task();
	__main->_state = RUNNING;
	__running = __main;
}

void Task::pass_to(Task * t, State s) {
	this->_state = s;
	__running = t;
	if (s == READY)
		__ready.push(this);
	swapcontext(&this->_context, &t->_context);
}

void Task::yield() {
	pass_to((Task*) Scheduler::self(), READY);
}

void Task::exit(int code) {
	__tid_counter--;
	_count--;
	this->_state = FINISHING;
	swapcontext(&this->_context, &this->__main->_context);
}
} /* namespace BOOOS */
