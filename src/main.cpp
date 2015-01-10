#include "Master.hpp"
#include "Slave.hpp"

int main(int argc, char* argv[])
{
		Master master;
		master.startBehavior();
		master.join();
		return 0;
}

