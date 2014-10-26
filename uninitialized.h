/*
* uninitialized.h
*
*  Created on: Oct 26, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

namespace kmu
{
	struct Uninitialized final
	{
		Uninitialized() = delete;
		~Uninitialized() = delete;
		Uninitialized(const Uninitialized&) = delete;
		Uninitialized(Uninitialized&&) = delete;
		Uninitialized& operator=(const Uninitialized&) = delete;
		Uninitialized& operator=(Uninitialized&&) = delete;
	};

	struct null_t
	{
	};

} // namespace kmu
