/*
 * File: thread.cpp
 * Group: 14
 * Description: Implementation for the Thread abstract clas.
 */

#include "thread.h"
#include "util.h"

Thread::Thread() : _cancelFlag(false) {
	Log::info("Thread constructor()");
	pthread_mutex_init(&_cancelFlagMutex, NULL);
}

/*
 * Creates a new thread and starts execution in the new thread. Returns true if
 * the thread was started successfully.
 */
bool Thread::startThread() {
	pthread_attr_t attr;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
	bool result = (pthread_create(&_threadId, NULL, &executeRun, this) == 0);
	pthread_attr_destroy (&attr);
	
	return result;
}

/*
 * Sets the cacel flag to true;
 */
void Thread::stopThread() {
	Log::info("in stopThread()");
	
	pthread_mutex_lock(&_cancelFlagMutex);
	_cancelFlag = true;
	pthread_mutex_unlock(&_cancelFlagMutex);
}

bool Thread::isCancelFlagSet() {
	bool result = false;
	pthread_mutex_lock(&_cancelFlagMutex);
	result = _cancelFlag;
	pthread_mutex_unlock(&_cancelFlagMutex);
	
	return result;
}

/*
 * Wrapper for the run() function which gets implemented by the derived class.
 * Allows the new thread to still be able to access class members
 *
 * context: Pointer to the class calling start()
 */
void* Thread::executeRun(void* context) {
	((Thread*)context)->run();
	return NULL;
}
