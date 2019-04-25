#include "Metrics.hpp"
#include <cmath>
#include <iostream>

#include <fftw3.h>


std::vector< double >
a_weighting( const std::vector< double > &freqs )
{
	const double c1 = 12194 * 12194;
	const double c2 = 20.6 * 20.6;
	const double c3 = 107.7 * 107.7;
	const double c4 = 737.9 * 737.9;

	std::vector< double > ret;

	ret.reserve( freqs.size() );

	// DC componentn is 2 dB
	ret.emplace_back( 2.0 );

	for ( std::size_t i = 1; i < freqs.size(); ++i )
	{
		const double f = freqs[i];
		const double f2 = f * f;
		const double f4 = f2 * f2;

		double ra = ( c1 * f4 ) / ( ( f2 + c2 ) * std::sqrt( ( f2 + c3 ) * ( f2 + c4 ) ) * ( f2 + c1 ) );

		// Convert to dB
		ret.emplace_back( 10.0 * std::log10( ra * ra ) + 2.0 );
	}

	return ret;
}

std::vector< double > fftfreq( int n, double d )
{
	double T = static_cast< double >( n ) * d;

	std::vector< double > ret;
	ret.reserve( n );

	// 0 is always first
	ret.emplace_back( 0.0 );

	if ( n % 2 )    // odd
	{
		// Positive values
		for ( int i = 1; i <= ( n - 1 ) / 2; ++i )
			ret.emplace_back( static_cast< double >( i ) / T );
		// Negative values
		for ( int i = -( n - 1 ) / 2; i <= -1; ++i )
			ret.emplace_back( static_cast< double >( i ) / T );
	} else { //even
		// Positive values
		for ( int i = 1; i <= n / 2 - 1; ++i )
			ret.emplace_back( static_cast< double >( i ) / T );
		// Negative values
		for ( int i = -n / 2; i <= -1; ++i )
			ret.emplace_back( static_cast< double >( i ) / T );
	}

	return ret;
}

double
a_noise_level( std::vector<double> signal, double rate )
{
	double delta = 1.0 / rate;

	std::vector< double > buff;
	buff.reserve( signal.size() );

	int n = static_cast<int>(signal.size());
	int out_size = n / 2 + 1;

	auto *input = ( double * )fftw_malloc( n * sizeof( double ) );
	auto *spec = ( fftw_complex * )fftw_malloc( out_size * sizeof( fftw_complex ) );

	// Get frequencies
	std::vector< double > freqs;
	freqs.reserve( static_cast< std::size_t >( out_size ) );
	for ( int i = 0; i < out_size; ++i )
		freqs.emplace_back( i / ( static_cast< double >( n ) * delta ) );

	// Copy data
	bool zeros = true;
	for ( int i = 0; i < signal.size(); ++i )
	{
		input[i] = signal[i];

		if ( input[i] != 0.0 )
			zeros = false;
	}

	if ( zeros )
	{
		fftw_free( input );
		fftw_free( spec );
		return 0.0;
	}

	// Adjust using a-weighting
	auto plan = fftw_plan_dft_r2c_1d( n, input, spec, FFTW_ESTIMATE );
	fftw_execute( plan );
	fftw_destroy_plan( plan );

	double norm_fac = 1.0 / static_cast< double >( n );

	static const double lower[] = { 22.0, 44.0, 88.0, 177.0, 354.0, 707.0,
	                                1414.0, 2828.0, 5656.0, 11312.0 };

	static const double upper[] = { 44.0, 88.0, 177.0, 354.0, 707.0,
	                                1414.0, 2828.0, 5656.0, 11312.0, 22624.0 };

	std::vector< double > center{ 31.5, 63.0, 125.0, 250.0, 500.0,
	                                 1000.0, 2000.0, 4000.0, 8000.0, 16000.0 };

	double bands[10];
	// Get per-band value
	for ( int i = 0; i < 10; ++i )
	{
		double bsum = 0.0;
		int j = 0;
		while( j < freqs.size() && ( freqs[j] < lower[i] ) )
		{
			j++;
		}

		while ( j < freqs.size() && ( freqs[j] < upper[i] ) )
		{
			double val = spec[j][0] * norm_fac;
			bsum = std::pow( 10.0, val * 0.1 );
			j++;
		}

		bands[i] = 10.0 * std::log10( bsum );
	}

	// Use DC component
	double sum = std::pow( 10.0, spec[0][0] * norm_fac * 0.1 );
	auto weighting = a_weighting( center );
	for ( int i = 0; i < 10; ++i )
	{
		sum += std::pow( 10.0, ( bands[i] + weighting.at( i ) ) * 0.1 );
	}

	// sum:
	//std::cout << "sum: " << sum << "\n";

	// Free memory
	fftw_free( input );
	fftw_free( spec );

	return 10.0 * std::log10( sum );
}

std::vector<double>
a_noise_bands( std::vector<double> signal, double rate )
{
	double delta = 1.0 / rate;
	
	std::vector< double > buff;
	buff.reserve( static_cast< std::size_t >( signal.size() ) );
	
	int n = static_cast<int>(signal.size());
	int out_size = n / 2 + 1;
	
	auto *input = ( double * )fftw_malloc( n * sizeof( double ) );
	auto *spec = ( fftw_complex * )fftw_malloc( out_size * sizeof( fftw_complex ) );
	
	// Get frequencies
	std::vector< double > freqs;
	freqs.reserve( static_cast< std::size_t >( out_size ) );
	for ( int i = 0; i < out_size; ++i )
		freqs.emplace_back( i / ( static_cast< double >( n ) * delta ) );
	
	// Copy data
	bool zeros = true;
	for ( int i = 0; i < signal.size(); ++i )
	{
		input[i] = signal[i];
		//std::cout << input[i] << "\n";
		
		if ( input[i] != 0.0 )
			zeros = false;
	}
	
	std::vector<double> ret(10, 0.0);
	if ( zeros )
	{
		fftw_free( input );
		fftw_free( spec );

		return ret;
	}
	
	// Adjust using a-weighting
	auto plan = fftw_plan_dft_r2c_1d( n, input, spec, FFTW_ESTIMATE );
	fftw_execute( plan );
	fftw_destroy_plan( plan );
	
	double norm_fac = 1.0 / static_cast< double >( n );
	
	static const double lower[] = { 22.0, 44.0, 88.0, 177.0, 354.0, 707.0,
	                                1414.0, 2828.0, 5656.0, 11312.0 };
	
	static const double upper[] = { 44.0, 88.0, 177.0, 354.0, 707.0,
	                                1414.0, 2828.0, 5656.0, 11312.0, 22624.0 };
	
	std::vector< double > center{ 31.5, 63.0, 125.0, 250.0, 500.0,
	                              1000.0, 2000.0, 4000.0, 8000.0, 16000.0 };
	
	double bands[10];
	// Get per-band value
	for ( int i = 0; i < 10; ++i )
	{
		double bsum = 0.0;
		int j = 0;
		while( j < freqs.size() && ( freqs[j] < lower[i] ) )
		{
			j++;
		}
		
		while ( j < freqs.size() && ( freqs[j] < upper[i] ) )
		{
			double val = spec[j][0] * norm_fac;
			bsum = std::pow( 10.0, val * 0.1 );
			j++;
		}
		
		bands[i] = 10.0 * std::log10( bsum );
		//std::cout << "band: " << bands[i] << "\n";
	}
	
	// Use DC component
	double dc = spec[0][0] * norm_fac;
	//std::cout << "dc component: " << dc << "\n";
	//auto weighting = a_weighting( center );
	for ( int i = 0; i < 10; ++i )
	{
		ret[i] = dc + bands[i];
	}
	
	// sum:
	//std::cout << "sum: " << sum << "\n";
	
	// Free memory
	fftw_free( input );
	fftw_free( spec );
	
	return ret;
}