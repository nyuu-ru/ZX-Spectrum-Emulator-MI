/*
 * StateMachine.h
 *
 *  Created on: 15 апр. 2022 г.
 *      Author: unyuu
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include <stack>
#include <memory>
#include "ProgramState.h"

class StateMachine
{
private:
	static std::stack<std::shared_ptr<ProgramState>> s_states;
public:
	~StateMachine();
	static std::weak_ptr<ProgramState> current_state();

	static void enter_state(const std::shared_ptr<ProgramState> &st);
	static void replace_state(const std::shared_ptr<ProgramState> &st);
	static void leave_state();

};

#endif /* STATEMACHINE_H_ */
