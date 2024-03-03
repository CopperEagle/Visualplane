
#include "FrameRateEnforcer.h"
#include <GLFW/glfw3.h>
#include <unistd.h>


FrameRateEnforcer::FrameRateEnforcer(double fps): wait_period(1.0/fps){}


void FrameRateEnforcer::startloop()
{
	timestamp = glfwGetTime();
}


void FrameRateEnforcer::wait()
{
	while(glfwGetTime() - timestamp < wait_period){usleep(50);};
	timestamp += wait_period;
}

/*

FrameRateEnforcer::FrameRateEnforcer(double fps): wait_period_us(long(1.0/fps * 1000000000)){}


void FrameRateEnforcer::startloop()
{
	timestamp = std::chrono::steady_clock::now();
}

void FrameRateEnforcer::wait()
{
	while(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - timestamp).count() < wait_period_us){usleep(50);};
	timestamp += std::chrono::nanoseconds(wait_period_us);
}*/