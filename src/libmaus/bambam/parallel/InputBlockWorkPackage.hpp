/*
    libmaus
    Copyright (C) 2009-2014 German Tischler
    Copyright (C) 2011-2014 Genome Research Limited

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
#if ! defined(LIBMAUS_BAMBAM_PARALLEL_INPUTBLOCKWORKPACKAGE_HPP)
#define LIBMAUS_BAMBAM_PARALLEL_INPUTBLOCKWORKPACKAGE_HPP

#include <libmaus/bambam/parallel/ControlInputInfo.hpp>
#include <libmaus/parallel/SimpleThreadWorkPackage.hpp>

namespace libmaus
{
	namespace bambam
	{		
		namespace parallel
		{
			/**
			 * work package for reading blocks from a stream
			 **/
			struct InputBlockWorkPackage : public libmaus::parallel::SimpleThreadWorkPackage
			{
				typedef InputBlockWorkPackage this_type;
				typedef libmaus::util::unique_ptr<this_type>::type unique_ptr_type;
				typedef libmaus::util::shared_ptr<this_type>::type shared_ptr_type;
			
				//! information required for reading
				ControlInputInfo * inputinfo;
	
				InputBlockWorkPackage()
				: 
					libmaus::parallel::SimpleThreadWorkPackage(), 
					inputinfo(0)
				{
				}
				
				InputBlockWorkPackage(
					uint64_t const rpriority, ControlInputInfo * rinputinfo,
					uint64_t const rreadDispatcherId
				)
				: libmaus::parallel::SimpleThreadWorkPackage(rpriority,rreadDispatcherId), inputinfo(rinputinfo)
				{
				}
			
				char const * getPackageName() const
				{
					return "InputBlockWorkPackage";
				}
			};
		}
	}
}
#endif