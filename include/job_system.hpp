#pragma once
#include <functional>
#include <mutex>
#include <thread>

namespace jobs
{
using job = std::function<void()>;

//enum class job_status
//{
//	standby,
//	inProgress,
//	finished
//};

//struct job
//{
//	std::function<void()> Function;
//	job_status Status = job_status::standby;
//
//	void operator()() const
//	{
//		Function();
//	}
//};

class job_system
{
public:
	job_system() = delete;
	explicit job_system(const size_t size) : _max_size(size) {}

	/**
	 * \brief Execute a job on an available thread and a return a pointer to it's execution status.
	 * \param job_to_perform The function to turn into a job
	 */
	void Execute(const job& job_to_perform);
	void WaitUntilAllThreadsFinish();

private:
	std::mutex _job_creation_mutex;
	std::condition_variable _condition;

	// Track the number of current threads executing that were started by this job system
	size_t _current_thread_count = 0;
	// Max number of threads this job system can concurrently create
	size_t _max_size;
};}