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
#include <libmaus/lz/PlainOrGzipStream.hpp>
#include <libmaus/lz/BufferedGzipStream.hpp>
// #include <libmaus/lz/PlainOrGzipStreamBufferWrapper.hpp>

int main()
{
	try
	{
		// ::libmaus::lz::BufferedGzipStream BGS(std::cin);
		libmaus::lz::PlainOrGzipStream BGS(std::cin);
		::libmaus::autoarray::AutoArray<char> B(64*1024);
		
		while ( BGS )
		{
			BGS.read(B.begin(),B.size());
			int64_t const n = BGS.gcount();
			if ( n > 0 )
				std::cout.write(B.begin(),n);
		}
		
		std::cout.flush();
		
		return EXIT_SUCCESS;
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
