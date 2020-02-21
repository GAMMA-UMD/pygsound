//
// Created by zytang on 6/17/18.
//

#include "MicrophoneArrays.hpp"

//##########################################################################################
//***************************  Start Om Sound Base Namespace  ******************************
OM_SOUND_BASE_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################


MicrophoneArrays:: MicrophoneArrays()
        :	arrayType(CUSTOM),
             ChannelLayout()
{
}


MicrophoneArrays:: MicrophoneArrays( Size newNumChannels )
        :	arrayType(CUSTOM),
             ChannelLayout(newNumChannels)
{
}


MicrophoneArrays:: MicrophoneArrays( ArrayType newArrayType )
        :	arrayType(newArrayType),
             ChannelLayout(Type::CUSTOM)
{
    setArrayType(newArrayType);
}


MicrophoneArrays:: MicrophoneArrays( const MicrophoneArrays& other )
        :	ChannelLayout(other)
{
    // Copy the speaker angle data structure if it is valid.
    arrayType = other.arrayType;
}


//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################


MicrophoneArrays:: ~MicrophoneArrays()
{
//    this->ChannelLayout::~ChannelLayout();
}


//##########################################################################################
//##########################################################################################
//############
//############		Layout Type Accessor Method
//############
//##########################################################################################
//##########################################################################################

void MicrophoneArrays:: setArrayType( ArrayType newArrayType )
{
    arrayType = newArrayType;
    switch (arrayType)
    {
        case ArrayType::TENCENT:
            setChannelCount(6);
            setChannelPosition(0, polarXZ(0));
            setChannelPosition(1, polarXZ(60.0f));
            setChannelPosition(2, polarXZ(120.0f));
            setChannelPosition(3, polarXZ(180.0f));
            setChannelPosition(4, polarXZ(-120.0f));
            setChannelPosition(5, polarXZ(-60.0f));
            for ( Index c = 0; c < 6; c++ )
            {
                setChannelType( c, ChannelType::UNDEFINED);
            }
            break;
        case ArrayType::GOOGLE:
            setChannelCount(8);
            setChannelPosition(0, Vector3f( Float(1), Float(1), Float(1) ));
            setChannelPosition(1, Vector3f( Float(1), Float(-1), Float(1) ));
            setChannelPosition(2, Vector3f( Float(1), Float(1), Float(-1) ));
            setChannelPosition(3, Vector3f( Float(1), Float(-1), Float(-1) ));
            setChannelPosition(4, Vector3f( Float(-1), Float(1), Float(1) ));
            setChannelPosition(5, Vector3f( Float(-1), Float(-1), Float(1) ));
            setChannelPosition(6, Vector3f( Float(-1), Float(1), Float(-1) ));
            setChannelPosition(7, Vector3f( Float(-1), Float(-1), Float(-1) ));
            for ( Index c = 0; c < 8; c++ )
            {
                setChannelType( c, ChannelType::UNDEFINED);
            }
            break;
        default:
            setChannelCount(0);
    }
}

//##########################################################################################
//***************************  End Om Sound Base Namespace  ********************************
OM_SOUND_BASE_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
