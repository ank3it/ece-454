/*
 * File: thread.cpp
 * Group: 14
 * Description: Implementation for the Thread abstract clas.
 */

#include "thread.h"

/*
 * Creates a new thread and starts execution in the new thread
 */
bool Thread::start() {
	return (pthread_create(&_threadId, NULL, &executeRun, this) == 0);
}

/*
 * Causes the caller to wait until the thread has completed execution
 */
void Thread::waitForThread() {
	pthread_join(_threadId, NULL);
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
