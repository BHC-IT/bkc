#include "bfc/exit.hpp"
#include "bfc/masterThread.hpp"

bool bfc::_exit = false;

void bfc::exit()
{
	bfc::_exit = true;
	bfc::masterThread::kill();
}