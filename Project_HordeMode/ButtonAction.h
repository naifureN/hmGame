#pragma once
#include "includes.h"

class ButtonAction
{
public:
	virtual ~ButtonAction() = default;
	virtual void execute() = 0;

};

