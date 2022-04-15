/*
 * StateMachine.cc
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#include "StateMachine.h"

std::stack<std::shared_ptr<ProgramState>> StateMachine::s_states;

std::weak_ptr<ProgramState> StateMachine::current_state()
{
	if (s_states.empty()) return std::shared_ptr<ProgramState>(nullptr);
	return s_states.top();
}

void StateMachine::enter_state(const std::shared_ptr<ProgramState> &st)
{
	s_states.push(st);
	st->initialize();
	st->enter();
}

void StateMachine::replace_state(const std::shared_ptr<ProgramState> &st)
{
	leave_state();
	enter_state(st);
}

StateMachine::~StateMachine()
{
	while (not s_states.empty())
		leave_state();
}

void StateMachine::leave_state()
{
	if (auto st = current_state().lock()) {
		st->leave();
		st->finalize();
	}
	if (not s_states.empty()) s_states.pop();
}
