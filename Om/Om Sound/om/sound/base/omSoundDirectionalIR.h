/*
 * Project:     Om Software
 * Version:     1.0.0
 * Website:     http://www.carlschissler.com/om
 * Author(s):   Carl Schissler
 * 
 * Copyright (c) 2016, Carl Schissler
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	1. Redistributions of source code must retain the above copyright
 * 	   notice, this list of conditions and the following disclaimer.
 * 	2. Redistributions in binary form must reproduce the above copyright
 * 	   notice, this list of conditions and the following disclaimer in the
 * 	   documentation and/or other materials provided with the distribution.
 * 	3. Neither the name of the copyright holder nor the
 * 	   names of its contributors may be used to endorse or promote products
 * 	   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INCLUDE_OM_SOUND_DIRECTIONAL_RESPONSE_H
#define INCLUDE_OM_SOUND_DIRECTIONAL_RESPONSE_H


#include "omSoundBaseConfig.h"


#include "omSoundFrequencyData.h"
#include "omSoundChannelInfo.h"


//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################




//********************************************************************************
/// A class that stores a directional transfer function for a linear acoustic system.
/**
  * Multiple representations and filter domains are supported.
  * The most common is sampled data, where the response is known at discrete direction samples.
  * The data may also be stored in the spherical harmonic basis for more efficient
  * interpolation and less memory usage.
  */
class DirectionalIR
{
	public:
		
		//********************************************************************************
		//******	Representation Enum Declaration
			
			/// An enum type that specifies the various representations that a directional IR can have.
			enum Representation
			{
				/// The response is represented by data that is sampled for various discrete directions.
				/**
				  * This is the raw format for a directional IR, consisting of transfer function
				  * measurements for a set of discrete directions. The response in other directions is
				  * interpolated from the nearest direction samples.
				  *
				  * This format is generally less efficient for interpolation and filter reconstruction,
				  * and requires more memory.
				  */
				SAMPLED = 0,
				
				/// The response is represented in the spherical harmonic orthornormal function basis.
				/**
				  * This representation is more efficient for runtime interpolation and filter
				  * reconstruction and generally uses much less memory. However, the spherical harmonic
				  * basis cannot easily represent sharp directivities, so some error may be
				  * introduced in the resulting filter versus the original sampled data.
				  */
				SPHERICAL_HARMONIC = 1
			};
			
			
		//********************************************************************************
		//******	Domain Enum Declaration
			
			
			/// An enum type that specifies the domain that a directional IR is stored in.
			enum Domain
			{
				/// The response is stored as a sequence of time-domain pressure samples.
				/**
				  * This is the usual format for measured directional IR data and it
				  * is efficient for filter reconstruction, but is less suited to interpolation
				  * because it can introduce comb-filtering artifacts.
				  */
				TIME_DOMAIN = 0,
				
				/// The response is stored as complex (real+imaginary) frequency-domain samples.
				/**
				  * This format has better interpolation quality than time-domain data, but
				  * can still introduce some artifacts if the adjacent filters are very different
				  * in the frequency domain.
				  */
				FREQUENCY_DOMAIN = 1,
				
				/// The response is stored as magnitude only in the frequency domain.
				/**
				  * This domain type tends to be the best for interpolation of HRTF data
				  * where the small-scale phase is less important.
				  * The phase is neglected and reconstructed after the magnitude response
				  * is interpolated. This reduces interpolation artifacts.
				  * A min-phase or linear-phase reconstruction is used to determine the phase.
				  */
				FREQUENCY_DOMAIN_MAGNITUDE = 2,
				
				/// The response is stored as magnitude and phase in the frequency domain.
				/**
				  * This format does not have very good interpolation results, probably because the phase
				  * at different frequencies does not stay coherent between adjacent samples.
				  */
				FREQUENCY_DOMAIN_MAGNITUDE_PHASE = 3
			};
			
			
		//********************************************************************************
		//******	Filter Phase Enum Declaration
			
			
			/// An enum type that specifies the phase type of a filter reconstructed from magnitude-only data.
			enum Phase
			{
				/// When the filter is reconstructed from frequency domain data, the original phase data is used (if it exists).
				/**
				  * If there is no phase data for the directional IR, a minimum phase filter is computed instead.
				  */
				ORIGINAL_PHASE = 0,
				
				/// When the filter is reconstructed from frequency domain data, a linear-phase filter is created.
				/**
				  * This filter type has a latency of half the length of the directional IR in time domain.
				  *
				  * To construct the filter, all imaginary components of the frequency-domain data are set to
				  * zero and an inverse FFT is performed. The first and last halves of the time-domain result
				  * are swapped to produce a causal linear-phase filter.
				  * While this filter type introduces more latency, the phase response is linear.
				  */
				LINEAR_PHASE = 1,
				
				/// When the filter is reconstructed from frequency domain data, a minimum-phase filter is created.
				/**
				  * This filter type has the minimum possible latency for a causal filter at all frequencies.
				  *
				  * To construct the filter, a Hilbert transform is performed on magnitude-only data to
				  * reconstruct the minimum phase for each frequency. An inverse FFT is performed to produce
				  * the time-domain result.
				  */
				MIN_PHASE = 2
			};
			
			
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new default sampled time-domain directional IR of length 256 with no channels.
			DirectionalIR();
			
			
			/// Create a new sampled directional IR with the specified format.
			DirectionalIR( Size numChannels, Size length, SampleRate sampleRate = SampleRate(44100), Domain domain = TIME_DOMAIN );
			
			
			/// Create a copy of the specified directional IR.
			DirectionalIR( const DirectionalIR& other );
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a directional IR and release its internal data.
			~DirectionalIR();
			
			
		//********************************************************************************
		//******	Assignment Operator
			
			
			/// Assign the state of another directional IR to this one.
			DirectionalIR& operator = ( const DirectionalIR& other );
			
			
		//********************************************************************************
		//******	Representation Accessor Methods
			
			
			/// Return the representation type for this directional IR.
			OM_INLINE Representation getRepresentation() const
			{
				return representation;
			}
			
			
		//********************************************************************************
		//******	Domain Accessor Methods
			
			
			/// Return the domain type for this directional IR.
			OM_INLINE Domain getDomain() const
			{
				return domain;
			}
			
			
			/// Change the domain type for this directional IR.
			/**
			  * The data in ths directional IR is converted to the specified domain.
			  * If the method fails because the domain cannot be converted (e.g. if it is in a
			  * spherical harmonic representation), the method returns FALSE.
			  */
			Bool setDomain( Domain newDomain );
			
			
		//********************************************************************************
		//******	Length Accessor Methods
			
			
			/// Return the length in samples of this directional IR's time-domain impulse response.
			OM_INLINE Size getLength() const
			{
				return length;
			}
			
			
			/// Set the length in samples of this directional IR's time-domain impulse response.
			/**
			  * The data stored in the response is converted so that it has the specified output length.
			  * Extra data is truncated or the response resampled if the length changes.
			  */
			void setLength( Size newLength );
			
			
		//********************************************************************************
		//******	Channel Accessor Methods
			
			
			/// Return the number of channels there are in this directional IR.
			OM_INLINE Size getChannelCount() const
			{
				return channels.getSize();
			}
			
			
			/// Return an object containing information about the channel at the specified index in this directional IR.
			OM_INLINE ChannelInfo& getChannelInfo( Index channelIndex )
			{
				return channels[channelIndex].info;
			}
			
			
			/// Return an object containing information about the channel at the specified index in this directional IR.
			OM_INLINE const ChannelInfo& getChannelInfo( Index channelIndex ) const
			{
				return channels[channelIndex].info;
			}
			
			
			/// Set information about the channel at the specified index in this directional IR.
			OM_INLINE void setChannelInfo( Index channelIndex, const ChannelInfo& newInfo )
			{
				if ( channelIndex >= channels.getSize() )
					return;
				
				channels[channelIndex].info = newInfo;
			}
			
			
			/// Set the number of channels that are in this directional IR.
			/**
			  * If the number of channels increases, the new channels are empty.
			  * If the number of channels decreases, the last channels are discarded.
			  */
			void setChannelCount( Size newChannelCount );
			
			
			/// Remove all channels and samples from this directional IR.
			void clearChannels();
			
			
		//********************************************************************************
		//******	Sample Accessor Methods
			
			
			/// Return the number of samples there are for the channel at the specified index.
			OM_INLINE Size getSampleCount( Index channelIndex ) const
			{
				if ( channelIndex >= channels.getSize() )
					return 0;
				
				return channels[channelIndex].samples.getSize();
			}
			
			
			/// Return the 3D cartesian vector for the specified sample.
			OM_INLINE const Vector3f& getSampleDirection( Index channelIndex, Index sampleIndex ) const
			{
				return channels[channelIndex].samples[sampleIndex].direction;
			}
			
			
			/// Return the delay time in seconds for the specified sample.
			OM_INLINE Float getSampleDelay( Index channelIndex, Index sampleIndex ) const
			{
				return channels[channelIndex].samples[sampleIndex].delay;
			}
			
			
			/// Return the data for the specified sample.
			OM_INLINE const Float32* getSampleData( Index channelIndex, Index sampleIndex ) const
			{
				return channels[channelIndex].samples[sampleIndex].data.getPointer();
			}
			
			
			/// Add a new sample of frequency-domain maginitude-only data to this directional IR.
			/**
			  * The method returns whether or not the sample data was able to be added.
			  */
			Bool addSample( Index channelIndex, const Vector3f& direction, Float delay, const FrequencyData& magnitude );
			
			
			/// Add a new sample to this directional IR.
			/**
			  * The sample data must be in the same domain as the directional IR and must
			  * be at least getFilterLength() floating-point numbers long.
			  *
			  * The method returns whether or not the sample data was able to be added.
			  */
			Bool addSample( Index channelIndex, const Vector3f& direction, Float delay, const Float32* sampleData );
			
			
			/// Clear all of the samples from this directional IR.
			void clearSamples();
			
			
		//********************************************************************************
		//******	Sample Rate Accessor Methods
			
			
			/// Return the sample rate that this directional IR is stored in.
			OM_INLINE SampleRate getSampleRate() const
			{
				return sampleRate;
			}
			
			
			/// Set the sample rate that this directional IR is stored in.
			/**
			  * This method does not modify the directional IR data, just the sample rate
			  * that the data is interpreted in.
			  */
			OM_INLINE void setSampleRate( SampleRate newSampleRate )
			{
				sampleRate = newSampleRate;
			}
			
			
		//********************************************************************************
		//******	Orientation Accessor Methods
			
			
			/// Return a 3x3 orthonormal rotation matrix indicating the local coordinate basis of this directional IR.
			/**
			  * This matrix transforms the directions in the directional IR before the filter
			  * is interpolated. This allows the HRTF to be rotated to match a given coordinate
			  * system convention.
			  */
			OM_INLINE const Matrix3f& getOrientation() const
			{
				return orientation;
			}
			
			
			/// Set a 3x3 orthonormal rotation matrix indicating the local coordinate basis of this directional IR.
			/**
			  * This matrix transforms the directions in the directional IR before the filter
			  * is interpolated. This allows the HRTF to be rotated to match a given coordinate
			  * system convention.
			  */
			OM_INLINE void setOrientation( const Matrix3f& newOrientation )
			{
				orientation = newOrientation;
			}
			
			
		//********************************************************************************
		//******	Name String Accessor Method
			
			
			/// Return a string containing the name of the acoustic system this directional IR was measured from.
			OM_INLINE const UTF8String& getName() const
			{
				return name;
			}
			
			
			/// Set a string containing the name of the acoustic system this directional IR was measured from.
			OM_INLINE void setName( const UTF8String& newName )
			{
				name = newName;
			}
			
			
		//********************************************************************************
		//******	Description String Accessor Method
			
			
			/// Return a string containing a description of this directional IR.
			OM_INLINE const UTF8String& getDescription() const
			{
				return description;
			}
			
			
			/// Set a string containing a description of this directional IR.
			OM_INLINE void setDescription( const UTF8String& newDescription )
			{
				description = newDescription;
			}
			
			
		//********************************************************************************
		//******	Spherical Harmonic Order Accessor Methods
			
			
			/// Return the spherical harmonic order that is used to represent this directional IR.
			/**
			  * If the representation type is SPHERICAL_HARMONIC, the return value indicates
			  * the spherical harmonic order. Otherwise, the return value is 0.
			  */
			OM_INLINE Size getSHOrder() const
			{
				return shOrder;
			}
			
			
		//********************************************************************************
		//******	Filter Interpolation Methods
			
			
			/// Return the length of the internal filter data in the current domain type.
			OM_INLINE Size getFilterLength() const
			{
				return this->getFilterLength( domain );
			}
			
			
			/// Return the length of the output filter data in the specified domain type.
			/**
			  * The return value indicates the number of valid samples that will be written
			  * to the buffer. NOTE: The required storage for the output filter data is
			  * retured by getFilterDataLength().
			  */
			Size getFilterLength( Domain domain ) const;
			
			
			/// Return the required floating-point storage for the output filter data.
			/**
			  * This value may be more than the return value of getFilterLength() because extra space is required
			  * for intermediate data.
			  */
			OM_INLINE Size getFilterDataLength() const
			{
				return fftSize + 2;
			}
			
			
			/// Get the interpolated filter for a 3D cartesian direction for the channel with the specified index.
			/**
			  * The filter data buffer must have enough space to hold the number of values returned
			  * by getFilterDataLength().
			  *
			  * The method returns the number of output samples that were written to the filter data buffer.
			  * A return value of 0 indicates that an error occurred.
			  */
			Size getFilter( Index channel, Domain domain, Phase phase,
							math::Interpolation interpolation, const Vector3f& xyz, Float* filterData ) const;
			
			
			/// Get the filter for a spherical harmonic basis for the channel with the specified index.
			/**
			  * This method should only be used for directional IRs that have the SPHERICAL_HARMONIC
			  * representation type.
			  *
			  * NOTE: the SH basis must be specified in the response's local
			  * coordinate system if the orientation calibration matrix is not the identity matrix.
			  * Therefore, the orientation calbration must be applied externally when building the basis
			  * coefficients.
			  *
			  * The filter data buffer must have enough space to hold the number of values returned
			  * by getFilterDataLength().
			  *
			  * The method returns the number of output samples that were written to the filter data buffer.
			  * A return value of 0 indicates that an error occurred.
			  */
			Size getFilterSH( Index channel, Domain domain, Phase phase,
							Size shOrder, const Float* shBasis, Float* filterData ) const;
			
			
		//********************************************************************************
		//******	Conversion Methods
			
			
			/// Convert the data for this directional IR to the spherical harmonic representation using the specified parameters.
			Bool convertToSH( const math::SHProjection& projection, DirectionalIR& result ) const;
			
			
			/// Convert the data for this directional IR to the specified domain type.
			/**
			  * The output directional IR is initialized with the converted domain data
			  * from this response.
			  */
			Bool convertDomain( Domain domain, Phase phase, DirectionalIR& result ) const;
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a directional IR.
			static const om::resources::ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Class Declaration
			
			
			/// The type to use for direction sample data.
			typedef PODArray< Float32, 1, Size, AlignedAllocator<16> > SampleData;
			
			
			/// A 32-bit floating point complex number.
			typedef math::Complex<Float32> Complex32f;
			
			
			/// An enum which contains the classification for a convex hull vertex.
			enum HullClass
			{
				UNKNOWN_VERTEX = 0,
				INTERNAL_VERTEX = 1,
				HULL_VERTEX = 2
			};
			
			
			/// A class that represents a triangle on the surface of the convex hull of a sampled directional IR.
			class Triangle
			{
				public:
					
					OM_INLINE Triangle( Index newV1, Index newV2, Index newV3, const math::Plane3f& newPlane )
						:	v1( newV1 ),
							v2( newV2 ),
							v3( newV3 ),
							plane( newPlane )
					{
					}
					
					Index v1;
					Index v2;
					Index v3;
					
					// The plane of this triangle.
					math::Plane3f plane;
					
			};
			
			
			class Edge
			{
				public:
					
					OM_INLINE Edge( Index newV1, Index newV2, Index newWinding )
						:	v1( newV1 ),
							v2( newV2 ),
							winding( newWinding )
					{
					}
					
					
					Index v1;
					Index v2;
					
					/// The index of the odd vertex out in the edge's original triangle.
					Index winding;
					
			};
			
			
			/// A class that stores the data for a sample in a directional IR.
			class Sample
			{
				public:
					
					OM_INLINE Sample( const Vector3f& newDirection, Float newDelay )
						:	direction( newDirection ),
							delay( newDelay )
					{
					}
					
					
					/// The normalized 3D cartesian direction for this HRTF sample.
					Vector3f direction;
					
					
					/// The broadband delay in seconds that this sample's response should be delayed by.
					Float delay;
					
					
					/// The data for this sample.
					SampleData data;
					
			};
			
			
			/// A class that stores the data for a channel in a directional IR.
			class Channel
			{
				public:
					
					
					/// A list of the samples that are part of this channel's data.
					/**
					  * For spherical harmonic data, the samples represent the basis function coefficients
					  * in cannonical order.
					  */
					ArrayList<Sample> samples;
					
					
					/// An object that contains information about this channel.
					ChannelInfo info;
					
			};
			
			
			/// Make the resource transcoder a friend so that is can deserialize the SH order.
			friend class om::sound::resources::DirectionalIRResourceTranscoder;
			
			
		//********************************************************************************
		//******	Private Helper Methods
			
			
			Bool findTriangle( Index channelIndex, const Vector3f& direction, Index& triangle, Vector3f& bary ) const;
			
			
			void convertSampleDomain( Domain inputDomain, const Float32* dataIn, Domain outputDomain, Float32* dataOut, Phase phase ) const;
			
			void timeToFrequencyComplex( const Float32* dataIn, Float32* dataOut ) const;
			void frequencyComplexToTime( const Float32* dataIn, Float32* dataOut ) const;
			OM_FORCE_INLINE static void frequencyComplexToMagnitude( const Float32* dataIn, Float32* dataOut, Size fftSize );
			OM_FORCE_INLINE static void frequencyComplexToMagnitudePhase( const Float32* dataIn, Float32* dataOut, Size fftSize );
			void frequencyMagnitudeToMagnitude( const FrequencyData& dataIn, Float32* dataOut, Size fftSize ) const;
			void frequencyMagnitudeToComplex( const FrequencyData& dataIn, Float32* dataOut, Size fftSize, Phase phase ) const;
			void frequencyMagnitudeToComplex( const Float32* dataIn, Float32* dataOut, Size fftSize, Phase phase ) const;
			OM_FORCE_INLINE static void frequencyMagnitudePhaseToComplex( const Float32* dataIn, Float32* dataOut, Size fftSize );
			OM_FORCE_INLINE static void frequencyMagnitudePhaseToMagnitude( const Float32* dataIn, Float32* dataOut, Size fftSize );
			
			
			static void buildHull( const Sample* vertices, Size numVertices, ArrayList<Triangle>& triangles );
			
			
			/// Return a uniformly distributed random unit vector direction.
			OM_FORCE_INLINE static Vector3f getRandomDirection( math::Random<Float>& variable )
			{
				const Float u1 = variable.sample( Float(-1), Float(1) );
				const Float u2 = variable.sample( Float(0), Float(1) );
				const Float r = math::sqrt( Float(1) - u1*u1 );
				const Float theta = Float(2)*math::pi<Float>()*u2;
				
				return Vector3f( r*math::cos( theta ), r*math::sin( theta ), u1 );
			}
			
			
			
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the channels in this directional IR.
			ShortArray<Channel,2> channels;
			
			
			/// The length in samples of this directional IR's time-domain impulse response.
			Size length;
			
			
			/// The length in samples of this directional IR's frequency-domain impulse response.
			Size fftSize;
			
			
			/// The spherical harmonic order of the data stored in this directional IR.
			Size shOrder;
			
			
			/// The internal representation type that is used for this directional IR.
			Representation representation;
			
			
			/// The internal domain type that is used for this directional IR.
			Domain domain;
			
			
			/// The sample rate that this directional IR is stored in.
			SampleRate sampleRate;
			
			
			/// A 3x3 orthonormal rotation matrix indicating the local coordinate basis of this directional IR.
			/**
			  * This matrix transforms the directions in the directional IR before the filter
			  * is interpolated. This allows the HRTF to be rotated to match a given coordinate
			  * system convention.
			  */
			Matrix3f orientation;
			
			
			/// A string containing a the name of this directional IR.
			UTF8String name;
			
			
			/// A string containing a description of this directional IR.
			UTF8String description;
			
			
			/// A pointer to an object that is computing real FFTs for this directional IR.
			mutable FFTReal<Float32>* fftReal;
			
			
			/// A pointer to an object that is computing complex FFTs for this directional IR.
			mutable FFTComplex<Float32>* fftComplex;
			
			
			/// A buffer that contains a temporary SH basis for a 3D direction vector.
			mutable SampleData tempSHBasis;
			
			
			/// A buffer that contains temporary storage for intermediate sample data.
			mutable SampleData tempBuffer;
			
			
			/// A list of the triangles that form this directional IR's convex hull.
			mutable ArrayList<Triangle> triangles;
			
			
			/// A boolean value that indicates whether or not the current convex hull is valid.
			mutable Bool validHull;
			
			
			
};




/// A class that stores a directional transfer function for a linear acoustic system.
typedef DirectionalIR DTF;


/// A class that stores a head-related transfer function.
typedef DirectionalIR HRTF;




//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::sound::base::DirectionalIR, om::sound::base::DirectionalIR::RESOURCE_TYPE );


#endif // INCLUDE_OM_SOUND_DIRECTIONAL_RESPONSE_H
