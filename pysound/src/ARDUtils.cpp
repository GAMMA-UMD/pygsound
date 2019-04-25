#include "ARDUtils.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

std::vector<float> read_field_ir( std::vector<int> size, std::vector<float> listener,
            const std::string &prefix, std::size_t nsteps )
{
	int nx = size[0];
	int ny = size[1];
	int nz = size[2];

	//std::cout << "nx: " << nx << ", ny: " << ny << ", nz: " << nz << "\n";

	int px = static_cast<int>( listener[0] * nx );
	int py = static_cast<int>( listener[1] * ny );
	int pz = static_cast<int>( listener[2] * nz );

	//std::cout << "px: " << px << ", py: " << py << ", pz: " << pz << "\n";

	std::vector<float> ir(nsteps, 0.0);
	auto *frame = new double[nx * ny * nz];

//	std::ofstream tempout( "temp_extract.ir" );

	std::ifstream ifs;
	for ( std::size_t i = 0; i < nsteps; ++i )
	{
		std::stringstream ss;
		ss << prefix << i + 1 << ".raw";
		ifs.open( ss.str().c_str(), std::ios_base::binary );

		if ( !ifs )
		{
			throw std::runtime_error( "unable to open file!" );
		}

		//std::cout << "index: " << px + py * nx + pz * nx * ny << " / " << nx * ny * nz << "\n";
		ifs.read( reinterpret_cast< char * >( frame ), nx * ny * nz * sizeof( double ) );
		// X varies the fastest
		double val = frame[px + py * nx + pz * nx * ny];
		//tempout << val << "\n";
		ir[i] = static_cast<float>(val);

		ifs.close();
	}

//	tempout.close();

	delete[] frame;

	return ir;
}

std::vector< std::vector<float> > read_field_irs( std::vector<int> size, std::vector< std::vector<float> > listeners,
                        const std::string &prefix, std::size_t nsteps )
{
	int nx = size[0];
	int ny = size[1];
	int nz = size[2];
	
	//std::cout << "nx: " << nx << ", ny: " << ny << ", nz: " << nz << "\n";
	
	auto nlisteners = listeners.size();

	std::vector< std::vector<float> > irs(nlisteners, std::vector<float>(nsteps, 0.0));
	
	//std::cout << "px: " << px << ", py: " << py << ", pz: " << pz << "\n";
	
	auto *frame = new double[nx * ny * nz];
	
	std::ifstream ifs;
	for ( std::size_t i = 0; i < nsteps; ++i )
	{
		std::stringstream ss;
		ss << prefix << i + 1 << ".raw";
		ifs.open( ss.str().c_str(), std::ios_base::binary );
		
		if ( !ifs )
		{
			throw std::runtime_error( "unable to open file!" );
		}
		
		ifs.read( reinterpret_cast< char * >( frame ), nx * ny * nz * sizeof( double ) );
		
		for ( int j = 0; j < nlisteners; ++j )
		{
			auto lpos = listeners[j];
			
			int px = static_cast<int>( lpos[0] * nx );
			int py = static_cast<int>( lpos[1] * ny );
			int pz = static_cast<int>( lpos[2] * nz );
			
			// X varies the fastest
			double val = frame[px + py * nx + pz * nx * ny];

			irs[j][i] = static_cast<float>(val);

		}
		
		ifs.close();
	}
	
	
	delete[] frame;
	
	return irs;
}
