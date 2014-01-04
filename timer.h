/**
 * The header file for a simple timer.
 * @file timer.h
 * @author Vikrant Sarang
 * @date January 2014
 */

#ifndef TIMER_H
#define TIMER_H

#include "SDL/SDL.h"

class Timer {
	public:
		Timer();

		void start();
		void stop();
		void pause();
		void unpause();

		int get_ticks();
		bool is_started();
		bool is_paused();

	private:
		int startTicks;
		int pauseTicks;
		bool paused;
		bool started;
};

#endif
