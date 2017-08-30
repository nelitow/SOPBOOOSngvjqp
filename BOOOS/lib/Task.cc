/*
 * Task.cc
 *
 *  Created on: Feb 27, 2014
 *      Author: arliones
 */

#include "Task.h"

namespace BOOOS {

volatile Task * Task::__running;
Task * Task::__main;
int Task::__tid_counter;


Task::Task(void (*entry_point)(void*), int nargs, void * arg) {
	getcontext(&this->_context);
	this->_stack = (char*) malloc(STACK_SIZE);
	if (_stack) {
		_context.uc_stack.ss_sp = _stack;
		_context.uc_stack.ss_size = STACK_SIZE;
		_context.uc_stack.ss_flags = 0;
		_context.uc_link = 0;
	} else {
		perror("Erro na criação da pilha: ");
		exit(1);
	}
	makecontext(&this->_context, (void (*)())&entry_point, nargs, arg);
}

Task::Task() {
	//this->_stack = (char*) malloc(STACK_SIZE);
	getcontext(&_context);
	_tid = __tid_counter;
	_state = READY;
}

Task::~Task() {
	free(this->_stack);
}

void Task::init() {
	__main = new Task();
	__main->_state = RUNNING;
	__running = __main;
	//__tid_counter;

	//__ready;

	//_tid;
	//_context;
	//_state;
}

void Task::pass_to(Task * t, State s) {
	this->_state = s;
	swapcontext(&this->_context, &t->_context);
}

void Task::exit(int code) {
	swapcontext(&this->_context, &this->__main->_context);
}
} /* namespace BOOOS */
