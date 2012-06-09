/*
 * File: thread.h
 * Group: 14
 * Description: Header file for Thread abstract class. This abstract class
 * provides threading capbilities. The run() function must be implemented by
 * the derived class. Calling startThread() will start the thread.
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>

class Thread {
	public:
		Thread() { /* Empty */ }
		virtual ~Thread() { /* Empty */ }

		bool startThread();
		bool stopThread();
		void waitForThread();

	protected:
		/*
		 * This function must be implemented by the derived class with the code
		 * that is to be run in the new thread
		 */
		virtual void run() = 0;

	private:
		static void* executeRun(void*);
		pthread_t _threadId;
};

#endif /* _THREAD_H_ */
