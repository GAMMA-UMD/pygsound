//
// Created by zytang on 6/17/18.
//

#ifndef PYSOUND_MICROPHONEARRAYS_H
#define PYSOUND_MICROPHONEARRAYS_H

#include <om/sound/base/omSoundChannelLayout.h>
#include <om/sound/omSoundBase.h>
#include <om/sound/base/omSoundBaseConfig.h>

//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################

class MicrophoneArrays : public ChannelLayout
{
    public:
            /// An enum that is used to specify various pre-defined types of channel layouts.
            enum ArrayType
            {
                /// An enum value describing a ChannelLayout for Tencent ASR microphone array system.
                        TENCENT,

                /// An enum value describing a ChannelLayout for Google ASR microphone array system.
                        GOOGLE,

                /// An enum value indicating a custom channel layout.
                        CUSTOM,

                /// An enum value indicating an undefined type of channel layout.
                        UNDEFINED
            };
            //********************************************************************************
            //******	Constructors


            /// Create a default channel layout with no channels.
            MicrophoneArrays();


            /// Create a new channel layout with the specified number of channels.
            /**
              * All channels in the layout are initialized to have the ChannelType::UNDEFINED
              * type and are non-positional.
              */
            MicrophoneArrays( Size newNumChannels );


            /// Create a ChannelLayout object that has the specified predefined layout type.
            /**
              * The predefined layout type is used to determine the type and number of
              * channels that should be in the layout.
              */
            MicrophoneArrays( ArrayType newArrayType );


            /// Create a copy of the specified ChannelLayout object.
            MicrophoneArrays( const MicrophoneArrays& other );


            //********************************************************************************
            //******	Destructor


            /// Destroy all state used by a ChannelLayout object.
            ~MicrophoneArrays();

            //********************************************************************************
            //******	Layout Type Accessor Methods


            /// Return the semantic kind of channel layout this layout corresponds to.
            OM_INLINE ArrayType getArrayType() const
            {
                return arrayType;
            }


            /// Set the semantic kind of channel layout this layout corresponds to.
            /**
              * If the number of channels for the specified layout type is different
              * than the current number of channels, the channel layout is enlarged
              * for the new number of channels.
              *
              * If the specified type is CUSTOM or UNDEFINED, the number and type of channels is
              * unchanged.
              */
            void setArrayType( ArrayType newArrayType );

    private:

            //********************************************************************************
            //******	Private Helper Methods


            /// Return the 3D normalized vector to a speaker located at the specified rotation relative to front center.
            OM_FORCE_INLINE static Vector3f polarXZ( Float degrees )
            {
                const Float radians = math::degreesToRadians( degrees );
                return Vector3f( math::sin(radians), 0, -math::cos(radians) );
            }

            //********************************************************************************
            //******	Private Data Members


            /// An enum value that stores the semantic type of this channel layout.
            ArrayType arrayType;

};

//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################

#endif //PYSOUND_MICROPHONEARRAYS_H
