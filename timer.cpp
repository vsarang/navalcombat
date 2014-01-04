#include "timer.h"

Timer::Timer() {
	startTicks = 0;
	pauseTicks = 0;
	paused = false;
	started = false;
}

void Timer::start() {
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}

void Timer::stop() {
	started = false;
	paused = false;
}

void Timer::pause() {
	if (started && !paused) {
		paused = true;
		pauseTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause() {
	if (paused) {
		paused = false;
		startTicks = SDL_GetTicks() - pauseTicks;
		pauseTicks = 0;
	}
}

int Timer::get_ticks() {
	if (started) {
		if (paused) {
			return pauseTicks;
		} else {
			return SDL_GetTicks() - startTicks;
		}
	} else {
		// timer isn't running
		return 0;
	}
}

bool Timer::is_started() {
	return started;
}

bool Timer::is_paused() {
	return paused;
}
