#include "WaitBehindCoverAction.h"
#include <iostream>

WaitBehindCoverAction::WaitBehindCoverAction()
{
	m_name = "Wait Behind Cover Action";
}

WaitBehindCoverAction::~WaitBehindCoverAction()
= default;

void WaitBehindCoverAction::Action()
{
	if (getAgent()->getActionState() != WAIT_BEHIND_COVER)
		std::cout << "Performing " << m_name << std::endl;
	getAgent()->WaitBehindCover();
}
