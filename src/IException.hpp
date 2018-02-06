#pragma once

#include <exception>
#include <string>

class IException : public std::exception
{
public:
	IException(const std::string message = "Unknown exception", const std::string type = "Exception");
	virtual ~IException() {}
	const char* what() const noexcept override;

private:
	std::string type_;
	std::string stack_;
	std::string message_;
	std::string what_;
	void calculateStack_() noexcept;
};