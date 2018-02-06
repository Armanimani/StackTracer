#include "IException.hpp"

#include "stackTrace.hpp"

IException::IException(const std::string message, const std::string type) : message_(std::move(message)), type_(std::move(type))
{
	calculateStack_();
	what_ = std::string("\t***** " + type_ + " *****\n\t[info]: " + message_ + '\n' + stack_);
}

const char* IException::what() const noexcept
{
	return what_.c_str();
}

void IException::calculateStack_() noexcept
{
	auto trace = stackTrace();

	for (decltype(trace.size()) i = 0; i != trace.size(); ++i)
	{
		stack_.append("\t[" + std::to_string(i) + "] " + trace[i]);
		if (i != trace.size() - 1)
		{
			stack_.append("\n");
		}
	}
}