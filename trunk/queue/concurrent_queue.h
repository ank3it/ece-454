/*
 * File: concurrent_queue.h
 * Date: May, 19/2012
 * Group: 14
 * Description: Header and implementation for a concurrent queue. This queue 
 * allows for synchronized access from different threads. Since this class uses
 * templates, both the declaration and definition are included in this header.
 */

#ifndef _CONCURRENT_QUEUE_H_
#define _CONCURRENT_QUEUE_H_

#include <pthread.h>
#include <queue>

/*
 * Provides a thread-safe generic implementation of a queue using pthread 
 * mutexes, condition variables and the STL queue
 */
template <typename Object>
class ConcurrentQueue {
	public:
		ConcurrentQueue();

		void push(Object const&);
		bool isEmpty() const;
		void pop(Object&);
		bool tryPop(Object&);
	
	private:
		std::queue<Object> _queue;
		pthread_mutex_t _queue_mutex;
		pthread_cond_t _queue_empty_cv;
};

/*
 * ConcurrentQueue constructor. Initializes the mutex and condition variable
 */
template <typename Object>
ConcurrentQueue<Object>::ConcurrentQueue() {
	pthread_mutex_init(&_queue_mutex, NULL);
	pthread_cond_init(&_queue_empty_cv, NULL);
}

/*
 * Returns true if the queue is empty
 */
template <typename Object>
bool ConcurrentQueue<Object>::isEmpty() const {
	return _queue.empty();
}

/*
 * Pushes the given object onto the queue. Signals the queue condition variable.
 *
 * obj: The object to be inserted into the queue
 */
template <typename Object>
void ConcurrentQueue<Object>::push(Object const& obj) {
	pthread_mutex_lock(&_queue_mutex);

	_queue.push(obj);

	pthread_cond_signal(&_queue_empty_cv);
	pthread_mutex_unlock(&_queue_mutex);
}

/*
 * Pops an element off the queue and returns it. If the queue is empty, the
 * thread blocks until signalled on the condition variable that it is no longer
 * empty
 *
 * result: A reference to an object in which the popped element will be returned
 */
template <typename Object>
void ConcurrentQueue<Object>::pop(Object& result) {
	pthread_mutex_lock(&_queue_mutex);

	// Check if queue is empty, wait on conditon variable if it is
	while (_queue.empty()) {
		pthread_cond_wait(&_queue_empty_cv, &_queue_mutex);
	}

	// Pop the oldest element from the queue
	result = _queue.front();
	_queue.pop();

	pthread_mutex_unlock(&_queue_mutex);
}

/*
 * Tries to pop an element off the queue. If able to pop off the queue then 
 * returns true.
 *
 * result: A reference to an object in which the popped element will be returned
 */
template <typename Object>
bool ConcurrentQueue<Object>::tryPop(Object& result) {
	pthread_mutex_lock(&_queue_mutex);

	if (_queue.empty()) 
		return false;

	result = _queue.front();
	_queue.pop();
	
	pthread_mutex_unlock(&_queue_mutex);

	return true;
}

#endif /* _CONCURRENT_QUEUE_H_ */
