#include "MoveToLOSAction.h"
#include <iostream>



MoveToLOSAction::MoveToLOSAction()
{
	m_name = "Move To LOS Action";
}

MoveToLOSAction::~MoveToLOSAction() = default;

void MoveToLOSAction::Action()
{
	std::cout << "Performing " << m_name << std::endl;
}