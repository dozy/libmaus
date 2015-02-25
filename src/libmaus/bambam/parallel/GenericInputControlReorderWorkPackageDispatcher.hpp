/*
    libmaus
    Copyright (C) 2009-2015 German Tischler
    Copyright (C) 2011-2015 Genome Research Limited

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
#if ! defined(LIBMAUS_BAMBAM_PARALLEL_GENERICINPUTCONTROLREORDERWORKPACKAGEDISPATCHER_HPP)
#define LIBMAUS_BAMBAM_PARALLEL_GENERICINPUTCONTROLREORDERWORKPACKAGEDISPATCHER_HPP

#include <libmaus/bambam/parallel/GenericInputControlReorderWorkPackageFinishedInterface.hpp>
#include <libmaus/bambam/parallel/GenericInputControlReorderWorkPackageReturnInterface.hpp>
#include <libmaus/bambam/parallel/GenericInputControlReorderWorkPackage.hpp>
#include <libmaus/parallel/SimpleThreadWorkPackageDispatcher.hpp>

namespace libmaus
{
	namespace bambam
	{
		namespace parallel
		{
			struct GenericInputControlReorderWorkPackageDispatcher : public libmaus::parallel::SimpleThreadWorkPackageDispatcher
			{
				typedef GenericInputControlReorderWorkPackage this_type;
				typedef libmaus::util::unique_ptr<this_type>::type unique_ptr_type;
				typedef libmaus::util::shared_ptr<this_type>::type shared_ptr_type;
				
				GenericInputControlReorderWorkPackageReturnInterface & packageReturnInterface;
				GenericInputControlReorderWorkPackageFinishedInterface & finishedInterface;
				libmaus::bambam::BamAuxFilterVector filter;
				libmaus::bitio::BitVector & BV;
			
				GenericInputControlReorderWorkPackageDispatcher(
					GenericInputControlReorderWorkPackageReturnInterface & rpackageReturnInterface,
					GenericInputControlReorderWorkPackageFinishedInterface & rfinishedInterface,
					libmaus::bitio::BitVector & rBV
				)
				: packageReturnInterface(rpackageReturnInterface), finishedInterface(rfinishedInterface), BV(rBV)
				{
					filter.set('Z','R');		
				}
			
				void dispatch(libmaus::parallel::SimpleThreadWorkPackage * P, libmaus::parallel::SimpleThreadPoolInterfaceEnqueTermInterface & /* tpi */)
				{
					assert ( dynamic_cast<GenericInputControlReorderWorkPackage *>(P) != 0 );
					GenericInputControlReorderWorkPackage * BP = dynamic_cast<GenericInputControlReorderWorkPackage *>(P);
					
					libmaus::bambam::parallel::AlignmentBuffer::shared_ptr_type & in = BP->in;
					libmaus::bambam::parallel::FragmentAlignmentBuffer::shared_ptr_type & out = BP->out;
					std::pair<uint64_t,uint64_t> const I = BP->I;
					uint64_t const index = BP->index;
					libmaus::bambam::parallel::FragmentAlignmentBufferFragment & frag = *((*out)[index]);
					uint32_t const dupflag = libmaus::bambam::BamFlagBase::LIBMAUS_BAMBAM_FDUP;
					uint32_t const dupmask = ~dupflag;
					
					for ( uint64_t i = I.first; i < I.second; ++i )
					{
						// get alignment block data
						std::pair<uint8_t const *,uint64_t> P = in->at(i);
						
						// get next offset in buffer
						uint64_t const o = frag.getOffset();						
						// put alignment
						frag.pushAlignmentBlock(P.first,P.second);

						// output data pointer						
						uint8_t * p = frag.getPointer(o);
						
						// get rank
						int64_t const rank = libmaus::bambam::BamAlignmentDecoderBase::getRank(p+sizeof(uint32_t),P.second);

						libmaus::bambam::BamAlignmentEncoderBase::putFlags(p+sizeof(uint32_t),libmaus::bambam::BamAlignmentDecoderBase::getFlags(p+sizeof(uint32_t)) & dupmask);

						// mark as duplicate if in bit vector
						if ( rank >= 0 && BV.get(rank) )
							libmaus::bambam::BamAlignmentEncoderBase::putFlags(
								p+sizeof(uint32_t),libmaus::bambam::BamAlignmentDecoderBase::getFlags(p+sizeof(uint32_t)) | dupflag
							);

						// filter out ZR tag
						uint32_t const fl = libmaus::bambam::BamAlignmentDecoderBase::filterOutAux(p+sizeof(uint32_t),P.second,filter);
						// replace length
						frag.replaceLength(o,fl);
						
						assert ( P.second >= fl );
						
						frag.pullBack(P.second-fl);
					}
					
					finishedInterface.genericInputControlReorderWorkPackageFinished(BP->in,BP->out);
					packageReturnInterface.genericInputControlReorderWorkPackageReturn(BP);
				}
			};
		}
	}
}
#endif