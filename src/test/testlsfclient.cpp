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

#include <libmaus/network/Socket.hpp>
#include <libmaus/network/GetHostName.hpp>
#include <libmaus/util/ArgInfo.hpp>

int main(int argc, char * argv[])
{
	try
	{
		::libmaus::util::ArgInfo const arginfo(argc,argv);
		int const controlfd = arginfo.getValue<int>("controlfd",-1);
		uint64_t const mem = arginfo.getValue<int>("mem",0);
		::libmaus::network::SocketBase controlsock(controlfd);
		
		std::cout << "Hello world." << std::endl;
		std::cout << "controlfd=" << controlfd << std::endl;
		std::cout << "mem=" << mem << std::endl;
		
		controlsock.writeString(::libmaus::network::GetHostName::getHostName());
		controlsock.barrierRw();
	}
	catch(std::exception const & ex)
	{
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}
