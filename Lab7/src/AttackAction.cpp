#include "AttackAction.h"
#include <iostream>

AttackAction::AttackAction()
{
	m_name = "Attack Action";
}

AttackAction::~AttackAction() = default;

void AttackAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
}