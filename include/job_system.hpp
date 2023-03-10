#pragma once

enum class job_completion
{
	standby,
	inProgress,
	finished
};

struct job
{
	job_completion status;

	void* function;

};