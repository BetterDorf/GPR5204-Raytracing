#include "job_system.hpp"

#ifdef TRACY_ENABLE
#include "tracy/Tracy.hpp"
#endif

namespace jobs
{
void job_system::Execute(const job& job_to_perform)
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif

	// Get exclusive acces to this part of the code
	std::unique_lock<std::mutex> lock(_job_creation_mutex);

	// Wait until we are allowed to create a new thread
	_condition.wait(lock, [this] {return this->_current_thread_count < this->_max_size; });

	_current_thread_count += 1;

	std::thread thread([this, job_to_perform] {
		job_to_perform();
		_current_thread_count -= 1;
		_condition.notify_one();
		}
	);
	thread.detach();
}

void job_system::WaitUntilAllThreadsFinish()
{
	// Get exclusive acces to this part of the code
	std::unique_lock<std::mutex> lock(_job_creation_mutex);

	// Wait until we are allowed to create a new thread
	_condition.wait(lock, [this] {return this->_current_thread_count = 0; });
}
}
