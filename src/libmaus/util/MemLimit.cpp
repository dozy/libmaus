/*
    libmaus
    Copyright (C) 2009-2013 German Tischler
    Copyright (C) 2011-2013 Genome Research Limited

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <libmaus/util/MemLimit.hpp>
#include <libmaus/LibMausConfig.hpp>

#if defined(LIBMAUS_HAVE_SETRLIMIT)
#include <sys/time.h>
#include <sys/resource.h>
#endif
#include <cerrno>
#include <cstring>

#include <libmaus/types/types.hpp>
#include <libmaus/exception/LibMausException.hpp>

void libmaus::util::MemLimit::setLimits(uint64_t const maxmem)
{
	setAddressSpaceLimit(maxmem);
	setDataLimit(maxmem);
	setResidentSetSizeLimit(maxmem);
}

void libmaus::util::MemLimit::setAddressSpaceLimit(uint64_t const maxmem)
{
	#if defined(LIBMAUS_HAVE_SETRLIMIT)
	rlimit const lim = { 
		static_cast<rlim_t>(maxmem), 
		static_cast<rlim_t>(maxmem) };
	if ( setrlimit(RLIMIT_AS,&lim) != 0 )
	{
		::libmaus::exception::LibMausException se;
		se.getStream() << "setrlimit(RLIMIT_AS," << maxmem << ") failed: " << strerror(errno) << std::endl;
		se.finish();
		throw se;					
	}   
	#endif
}

void libmaus::util::MemLimit::setDataLimit(uint64_t const maxmem)
{
	#if defined(LIBMAUS_HAVE_SETRLIMIT)
	rlimit const lim = { 
		static_cast<rlim_t>(maxmem), 
		static_cast<rlim_t>(maxmem) };
	if ( setrlimit(RLIMIT_DATA,&lim) != 0 )
	{
		::libmaus::exception::LibMausException se;
		se.getStream() << "setrlimit(RLIMIT_DATA," << maxmem << ") failed: " << strerror(errno) << std::endl;
		se.finish();
		throw se;					
	}   
	#endif
}

void libmaus::util::MemLimit::setResidentSetSizeLimit(uint64_t const maxmem)
{
	#if defined(LIBMAUS_HAVE_SETRLIMIT)
	rlimit const lim = { 
		static_cast<rlim_t>(maxmem), 
		static_cast<rlim_t>(maxmem) };
	if ( setrlimit(RLIMIT_RSS,&lim) != 0 )
	{
		::libmaus::exception::LibMausException se;
		se.getStream() << "setrlimit(RLIMIT_RSS," << maxmem << ") failed: " << strerror(errno) << std::endl;
		se.finish();
		throw se;					
	}   
	#endif
}
