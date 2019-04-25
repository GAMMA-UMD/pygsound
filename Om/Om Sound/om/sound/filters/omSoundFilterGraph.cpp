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

#include "omSoundFilterGraph.h"


//##########################################################################################
//*************************  Start Om Sound Filters Namespace  *****************************
OM_SOUND_FILTERS_NAMESPACE_START
//******************************************************************************************
//##########################################################################################


const UTF8String FilterGraph:: NAME( "Filter Graph" );
const UTF8String FilterGraph:: MANUFACTURER( "Om Sound" );
const FilterVersion FilterGraph:: VERSION( 1, 0, 0 );


//##########################################################################################
//##########################################################################################
//############		
//############		Filter Node Connection Class Definition
//############		
//##########################################################################################
//##########################################################################################




class FilterGraph:: NodeConnection
{
	public:
		
		OM_INLINE NodeConnection( Node* newNode, Index newIOIndex, Index newConnectionIndex )
			:	node( newNode ),
				ioIndex( newIOIndex ),
				connectionIndex( newConnectionIndex )
		{
		}
		
		
		/// A pointer to the other node that is part of this connection.
		Node* node;
		
		
		/// The index of the input or output on this node that is connected.
		Index ioIndex;
		
		
		/// The index of the output connection on the other node that is connected.
		Index connectionIndex;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Node Class Definition
//############		
//##########################################################################################
//##########################################################################################




class FilterGraph:: Node
{
	public:
		
		/// Create a new filter node object that represents the specified SoundFilter.
		OM_INLINE Node( SoundFilter* newFilter )
			:	filter( newFilter ),
				resultFrame( NULL ),
				numOutputsUsed( 0 )
		{
		}
		
		
		/// Return TRUE if the node has no input or output connections and FALSE otherwise.
		OM_INLINE Bool isOrphaned() const
		{
			return inputs.getSize() == 0 && outputs.getSize() == 0;
		}
		
		
		/// A list of all of the filter input connections for this node.
		ShortArrayList<NodeConnection,2> inputs;
		
		
		/// A list of all of the filter output connections for this node.
		ShortArrayList<NodeConnection,2> outputs;
		
		
		/// A pointer to the filter that is being used to process this node.
		SoundFilter* filter;
		
		
		/// A pointer to a filter frame representing the temporary output of this filter node.
		SoundFrame* resultFrame;
		
		
		/// An object that represents the result of the processing for this node.
		SoundResult result;
		
		
		/// The total number of output connections of the node that have been used so far.
		/**
		  * This value is used to determine when the result frame of a node is no longer
		  * needed and so can be released back into the pool.
		  */
		Size numOutputsUsed;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Buffer Info Class Definition
//############		
//##########################################################################################
//##########################################################################################




class FilterGraph:: BufferInfo
{
	public:
		
		OM_INLINE BufferInfo( const SharedSoundBuffer& newBuffer )
			:	buffer( newBuffer ),
				referenceCount( 0 )
		{
		}
		
		/// The shared sound buffer that is associated with this shared buffer info.
		SharedSoundBuffer buffer;
		
		/// The number of references that there are to this shared buffer info (probably just 1).
		Index referenceCount;
		
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Frame Info Class Definition
//############		
//##########################################################################################
//##########################################################################################




class FilterGraph:: SoundFrameInfo
{
	public:
		
		OM_INLINE SoundFrameInfo()
			:	frame(),
				referenceCount( 0 )
		{
		}
		
		/// The sound filter frame that is associated with this shared filter info.
		SoundFrame frame;
		
		/// The number of references that there are to this shared filter frame (probably just 1).
		Index referenceCount;
		
};




//##########################################################################################
//##########################################################################################
//############		
//############		Constructors
//############		
//##########################################################################################
//##########################################################################################




FilterGraph:: FilterGraph()
	:	SoundFilter( 0, 0 ),
		numBuffersInUse( 0 ),
		numFramesInUse( 0 )
{
}




FilterGraph:: FilterGraph( const FilterGraph& other )
	:	SoundFilter( other ),
		numBuffersInUse( 0 ),
		numFramesInUse( 0 )
{
	other.lockMutex();
	
	// Copy all of the input connections.
	HashMap<const SoundFilter*,Node>::ConstIterator i = other.nodes.getIterator();
	
	for ( ; i; i++ )
	{
		const Node& node = i.getValue();
		const Size numInputs = node.inputs.getSize();
		
		for ( Index j = 0; j < numInputs; j++ )
		{
			const NodeConnection& input = node.inputs[j];
			const NodeConnection& output = input.node->outputs[input.connectionIndex];
			
			this->connect( input.node->filter, output.ioIndex, node.filter, input.ioIndex );
		}
	}
	
	other.unlockMutex();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Destructor
//############		
//##########################################################################################
//##########################################################################################




FilterGraph:: ~FilterGraph()
{
	// Deallocate all of the temporary frames.
	for ( Index i = 0; i < tempFrames.getSize(); i++ )
		om::util::destruct( tempFrames[i] );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Assignment Operator
//############		
//##########################################################################################
//##########################################################################################




FilterGraph& FilterGraph:: operator = ( const FilterGraph& other )
{
	if ( this != &other )
	{
		lockMutex();
		
		this->clear();
		
		other.lockMutex();
		
		// Copy all of the input connections.
		HashMap<const SoundFilter*,Node>::ConstIterator i = other.nodes.getIterator();
		
		for ( ; i; i++ )
		{
			const Node& node = i.getValue();
			const Size numInputs = node.inputs.getSize();
			
			for ( Index j = 0; j < numInputs; j++ )
			{
				const NodeConnection& input = node.inputs[j];
				const NodeConnection& output = input.node->outputs[input.connectionIndex];
				
				this->connect( input.node->filter, output.ioIndex, node.filter, input.ioIndex );
			}
		}
		
		other.unlockMutex();
		unlockMutex();
	}
	
	return *this;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Attribute Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




UTF8String FilterGraph:: getName() const
{
	return NAME;
}




UTF8String FilterGraph:: getManufacturer() const
{
	return MANUFACTURER;
}




FilterVersion FilterGraph:: getVersion() const
{
	return VERSION;
}




FilterCategory FilterGraph:: getCategory() const
{
	return FilterCategory::ROUTING;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Graph Connection Accessor Methods
//############		
//##########################################################################################
//##########################################################################################




Bool FilterGraph:: connect( SoundFilter* output, SoundFilter* input )
{
	return this->connect( output, 0, input, 0 );
}




Bool FilterGraph:: connect( SoundFilter* output, Index outputIndex, SoundFilter* input, Index inputIndex )
{
	//*****************************************************************************
	// Determine if the connection is valid.
	
	// Check to see if the filters have inputs and outputs with the specified indices.
	if ( output != NULL && outputIndex >= output->getOutputCount() ||
		input != NULL && inputIndex >= input->getInputCount() )
	{
		// If not, return that the connection was not made.
		return false;
	}
	
	lockMutex();
	
	// Test to see if the connection is to the global filter input.
	// If this connection enlarges the total number of inputs for the graph, make it so.
	if ( output == NULL && outputIndex >= this->getInputCount() )
		this->setInputCount( outputIndex + 1 );
	
	// Test to see if the connection is to the global filter output.
	// If this connection enlarges the total number of outputs for the graph, make it so.
	if ( input == NULL && inputIndex >= this->getOutputCount() )
		this->setOutputCount( inputIndex + 1 );
	
	//*****************************************************************************
	// Query the node map for the nodes of the specified filters.
	
	Hash outputHash = Hash(PointerInt(output));
	Node* outputNode;
	
	// Find the output filter in the node map. If it is not there, create a new node for it.
	if ( !nodes.find( outputHash, output, outputNode ) )
		outputNode = nodes.add( outputHash, output, Node( output ) );
	
	Hash inputHash = Hash(PointerInt(input));
	Node* inputNode;
	
	// Find the input filter in the node map. If it is not there, create a new node for it.
	if ( !nodes.find( inputHash, input, inputNode ) )
		inputNode = nodes.add( inputHash, input, Node( input ) );
	
	//*****************************************************************************
	// Add the connection for the output filter in the correct sorted location.
	
	const Size outputNumOutputs = outputNode->outputs.getSize();
	Index outputInsertIndex = outputNumOutputs;
	
	for ( Index i = 0; i < outputNumOutputs; i++ )
	{
		if ( outputIndex < outputNode->outputs[i].ioIndex )
		{
			outputInsertIndex = i;
			break;
		}
	}
	
	outputNode->outputs.insert( outputInsertIndex, NodeConnection( inputNode, outputIndex, 0 ) );
	
	// Update the connection indices for all of the outputs after this one.
	for ( Index i = outputInsertIndex + 1; i <= outputNumOutputs; i++ )
	{
		NodeConnection& outputConnection = outputNode->outputs[i];
		outputConnection.node->inputs[outputConnection.connectionIndex].connectionIndex++;
	}
	
	//*****************************************************************************
	// Add the connection for the input filter in the correct sorted location.
	
	const Size inputNumInputs = inputNode->inputs.getSize();
	Index inputInsertIndex = inputNumInputs;
	
	for ( Index i = 0; i < inputNumInputs; i++ )
	{
		if ( inputIndex < inputNode->inputs[i].ioIndex )
		{
			inputInsertIndex = i;
			break;
		}
	}
	
	inputNode->inputs.insert( inputInsertIndex, NodeConnection( outputNode, inputIndex, outputInsertIndex ) );
	outputNode->outputs[outputInsertIndex].connectionIndex = inputInsertIndex;
	
	// Update the connection indices for all of the inputs after this one.
	for ( Index i = inputInsertIndex + 1; i <= inputNumInputs; i++ )
	{
		NodeConnection& inputConnection = inputNode->inputs[i];
		inputConnection.node->outputs[inputConnection.connectionIndex].connectionIndex++;
	}
	
	unlockMutex();
	
	return true;
}




Bool FilterGraph:: disconnect( const SoundFilter* output, const SoundFilter* input )
{
	return this->disconnect( output, 0, input, 0 );
}




Bool FilterGraph:: disconnect( const SoundFilter* output, Index outputIndex, const SoundFilter* input, Index inputIndex )
{
	//*****************************************************************************
	// Determine if the connection is valid.
	
	// Check to see if the filters have inputs and outputs with the specified indices.
	if ( output != NULL && outputIndex >= output->getOutputCount() ||
		input != NULL && inputIndex >= input->getInputCount() )
	{
		// If not, return that the connection was not made.
		return false;
	}
	
	lockMutex();
	
	//*****************************************************************************
	// Query the node map for the nodes of the specified filters.
	
	Hash outputHash = Hash(PointerInt(output));
	Node* outputNode;
	
	Bool removedConnection = false;
	
	// Find the output filter in the node map. If it is not there, return failure.
	if ( nodes.find( outputHash, output, outputNode ) )
	{
		// Find the output connection.
		Size outputNumOutputs = outputNode->outputs.getSize();
		
		for ( Index i = 0; i < outputNumOutputs; i++ )
		{
			NodeConnection& outputConnection = outputNode->outputs[i];
			Node* inputNode = outputConnection.node;
			
			// Is this the right output connection?
			if ( outputIndex == outputConnection.ioIndex && input == inputNode->filter &&
				inputIndex == inputNode->inputs[outputConnection.connectionIndex].ioIndex )
			{
				// Found the connection, now remove it.
				outputNode->outputs.removeAtIndex(i);
				inputNode->inputs.removeAtIndex(outputConnection.connectionIndex);
				removedConnection = true;
				
				const Size inputNumInputs = inputNode->inputs.getSize();
				
				// Update the connection indices for all of the inputs after this one.
				for ( Index j = outputConnection.connectionIndex; j < inputNumInputs; j++ )
				{
					NodeConnection& inputConnection = inputNode->inputs[j];
					inputConnection.node->outputs[inputConnection.connectionIndex].connectionIndex--;
				}
				
				outputNumOutputs--;
				
				// Update the connection indices for all of the outputs after this one.
				for ( Index j = i; j < outputNumOutputs; j++ )
				{
					NodeConnection& outputConnection = outputNode->outputs[j];
					outputConnection.node->inputs[outputConnection.connectionIndex].connectionIndex--;
				}
				
				
				// Test to see if the number of global graph inputs has changed.
				if ( output == NULL && outputIndex + 1 == this->getInputCount() )
				{
					Size numConnections = outputNode->outputs.getSize();
					Size currentNumInputs = this->getInputCount();
					Index numInputs = 0;
					Bool foundMaximalInput = false;
					
					for ( Index i = 0; i < numConnections; i++ )
					{
						if ( outputNode->outputs[i].ioIndex == currentNumInputs )
							foundMaximalInput = true;
						
						numInputs = math::max( numInputs, outputNode->outputs[i].ioIndex + 1 );
					}
					
					if ( foundMaximalInput )
						this->setInputCount( numInputs );
				}
				
				// Test to see if the number of global graph outputs has changed.
				if ( input == NULL && inputIndex + 1 == this->getOutputCount() )
				{
					Size numConnections = inputNode->inputs.getSize();
					Size currentNumOutputs = this->getOutputCount();
					Index numOutputs = 0;
					Bool foundMaximalOutput = false;
					
					for ( Index i = 0; i < numConnections; i++ )
					{
						if ( inputNode->inputs[i].ioIndex == currentNumOutputs )
							foundMaximalOutput = true;
						
						numOutputs = math::max( numOutputs, inputNode->inputs[i].ioIndex + 1 );
					}
					
					if ( foundMaximalOutput )
						this->setOutputCount( numOutputs );
				}
				
				// Remove either the input or output node from the graph if they have no connections.
				if ( outputNode->isOrphaned() )
					nodes.remove( outputHash, output );
				
				if ( inputNode->isOrphaned() )
					nodes.remove( Hash(PointerInt(input)), input );
				
				break;
			}
		}
	}
	
	unlockMutex();
	
	return removedConnection;
}




Bool FilterGraph:: isConnected( const SoundFilter* output, const SoundFilter* input ) const
{
	lockMutex();
	
	Hash outputHash = Hash(PointerInt(output));
	const Node* outputNode;
	
	// Find the output filter in the node map. If it is not there, return false.
	if ( !nodes.find( outputHash, output, outputNode ) )
	{
		unlockMutex();
		return false;
	}
	
	// Look through the output connections for the first node and see if it connects to the second.
	const Size numNodeOutputs = outputNode->outputs.getSize();
	
	for ( Index i = 0; i < numNodeOutputs; i++ )
	{
		const NodeConnection& connection = outputNode->outputs[i];
		
		if ( connection.node->filter == input )
		{
			// Found a connection to that input node, so return that there was a connection.
			unlockMutex();
			return true;
		}
	}
	
	unlockMutex();
	
	return false;
}




Bool FilterGraph:: isConnected( const SoundFilter* output, Index outputIndex,
								const SoundFilter* input, Index inputIndex ) const
{
	lockMutex();
	
	Hash outputHash = Hash(PointerInt(output));
	const Node* outputNode;
	
	// Find the output filter in the node map. If it is not there, return false.
	if ( !nodes.find( outputHash, output, outputNode ) )
	{
		unlockMutex();
		return false;
	}
	
	// Look through the output connections for the first node and see if it connects to the second.
	const Size numNodeOutputs = outputNode->outputs.getSize();
	
	for ( Index i = 0; i < numNodeOutputs; i++ )
	{
		const NodeConnection& connection = outputNode->outputs[i];
		
		if ( connection.ioIndex == outputIndex && 
			connection.node->filter == input && connection.connectionIndex == inputIndex )
		{
			// Found a connection to that input node on the right input/output, so return that there was a connection.
			unlockMutex();
			return true;
		}
	}
	
	unlockMutex();
	
	return false;
}




void FilterGraph:: clear()
{
	nodes.clear();
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Reset Method
//############		
//##########################################################################################
//##########################################################################################




void FilterGraph:: resetStream()
{
	// Reset all of the filters in the graph.
	HashMap<const SoundFilter*,Node>::Iterator i = nodes.getIterator();
	
	while ( i )
	{
		SoundFilter* filter = i->filter;
		
		if ( filter != NULL )
			filter->reset();
	}
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Graph Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FilterGraph:: processFrame( const SoundFrame& inputFrame,
													SoundFrame& outputFrame, Size numSamples )
{
	// If there is no input or output buffer, return that no sound was processed.
	if ( inputFrame.getBufferCount() == 0 || outputFrame.getBufferCount() == 0 )
		return 0;
	
	//******************************************************************
	// Check to see if there are no nodes. If so, copy the input frame directly to the output.
	
	if ( nodes.getSize() == 0 )
	{
		// Determine how many buffers the input and output frames share.
		Size numBuffersToCopy = math::min( inputFrame.getBufferCount(), outputFrame.getBufferCount() );
		
		// Copy the contents of each input buffer to the output buffer.
		for ( Index i = 0; i < numBuffersToCopy; i++ )
		{
			const SoundBuffer* inputBuffer = inputFrame.getBuffer(i);
			SoundBuffer* outputBuffer = outputFrame.getBuffer(i);
			
			// Skip this pair of buffers if the output buffer is NULL.
			if ( outputBuffer == NULL )
				continue;
			
			// If the input buffer is NULL, write zeros to the output buffer.
			if ( inputBuffer == NULL )
				outputBuffer->zero( 0, numSamples );
			else
			{
				// Copy the input buffer's format to this output buffer.
				inputBuffer->copyFormatTo( *outputBuffer, numSamples );
				
				// Copy the input buffer to this output buffer.
				inputBuffer->copyTo( *outputBuffer, numSamples );
			}
		}
		
		return numSamples;
	}
	
	//******************************************************************
	// Get the main input/output node if it exists.
	
	Node* node;
	
	// Find the node corresponding to the main input and output of the filter graph.
	// If it is not found, or if there is no input or output buffer, return that no sound was processed.
	if ( !nodes.find( Hash(NULL), NULL, node ) )
		return 0;
	
	//******************************************************************
	// Process the main output node.
	
	// Set the NULL node (master input/output) to have the main input frame as its result.
	node->resultFrame = const_cast<SoundFrame*>( &inputFrame );
	
	// Process the NULL node and place its output in the main output frame.
	SoundResult result = processMainNode( node, &outputFrame, numSamples );
	
	// Release the filter frame.
	node->resultFrame = NULL;
	node->numOutputsUsed = 0;
	
	//******************************************************************
	
	// Iterate over all of the nodes and release unused output buffers.
	HashMap<const SoundFilter*,Node>::Iterator i = nodes.getIterator();
	
	for ( ; i; i++ )
	{
		if ( i->resultFrame != NULL )
		{
			releaseTempFrame( i->resultFrame );
			i->resultFrame = NULL;
			i->numOutputsUsed = 0;
		}
	}
	
	// Clean up the shared buffer pool.
	tempBuffers.clear();
	
	// Return the result of the filter graph processing.
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Main Filter Graph Node Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FilterGraph:: processMainNode( Node* node, SoundFrame* outputFrame, Size numSamples )
{
	// Gather the inputs to the node and copy them directly to the output.
	const Size numGraphOutputs = this->getOutputCount();
	const Size numInputConnections = node->inputs.getSize();
	Index lastInputIndex = math::max<Index>();
	Size maxNumSamples = 0;
	
	for ( Index i = 0; i < numInputConnections; i++ )
	{
		NodeConnection& input = node->inputs[i];
		Node* inputNode = input.node;
		
		// Keep track of the last input index so that we can determine when a new output buffer is used.
		Index previousInputIndex = lastInputIndex;
		Index inputIndex = lastInputIndex = input.ioIndex;
		
		// Make sure that this input connection is valid. If not, skip it.
		if ( inputIndex >= numGraphOutputs )
			continue;
		
		// Has this input node's output already been computed?
		if ( inputNode->resultFrame == NULL )
			computeNodeOutput( inputNode, numSamples );
		
		//*************************************************************************
		// Input node output is computed, send its output to the current node's input frame.
		
		Index outputIndex = inputNode->outputs[input.connectionIndex].ioIndex;
		const SoundBuffer* inputBuffer = inputNode->resultFrame->getBuffer( outputIndex );
		Size numValidSamples = inputNode->result.getSampleCount();
		
		// Skip NULL buffers or those with no valid samples.
		if ( inputBuffer != NULL && numValidSamples > 0 )
		{
			// Compute the maximum number of samples produced.
			maxNumSamples = math::max( maxNumSamples, numValidSamples );
			
			SoundBuffer* outputBuffer = outputFrame->getBuffer( inputIndex );
			
			// Skip NULL output buffers.
			if ( outputBuffer != NULL )
			{
				// Is this the first usage of this particular input index?
				if ( inputIndex != previousInputIndex )
				{
					// Copy the data to the main buffer.
					inputBuffer->copyFormatTo( *outputBuffer, numValidSamples );
					inputBuffer->copyTo( *outputBuffer, numValidSamples );
					outputBuffer->zero( numValidSamples, numSamples );
				}
				else
				{
					// Mix the buffer contents to the main buffer.
					inputBuffer->mixTo( *outputBuffer, numValidSamples );
				}
			}
		}
		
		//******************************************************************
		// Check to see if the input node's result frame should be released.
		
		inputNode->numOutputsUsed++;
		
		// If all outputs of the node have been computed, we can release the
		// node's frame back to the pool.
		if ( inputNode->filter != NULL &&
			inputNode->numOutputsUsed == inputNode->outputs.getSize() )
		{
			releaseTempFrame( inputNode->resultFrame );
			
			inputNode->resultFrame = NULL;
			inputNode->numOutputsUsed = 0;
		}
	}
	
	return maxNumSamples;
}




//##########################################################################################
//##########################################################################################
//############		
//############		General Filter Graph Node Processing Method
//############		
//##########################################################################################
//##########################################################################################




SoundResult FilterGraph:: processNode( Node* node, SoundFrame* outputFrame, Size numSamples )
{
	// Get the total number of inputs that this node's filter has.
	const Size numFilterInputs = node->filter->getInputCount();
	
	//******************************************************************
	// Build the filter input frame.
	
	// Create an input frame for the current node with the correct number of inputs.
	SoundFrame* inputFrame = getTempFrame( numFilterInputs );
	
	Index lastInputIndex = math::max<Index>();
	
	// Iterate over the input connections and build the filter input frame.
	const Size numInputConnections = node->inputs.getSize();
	
	for ( Index i = 0; i < numInputConnections; i++ )
	{
		NodeConnection& input = node->inputs[i];
		Node* inputNode = input.node;
		
		// Keep track of the last input index so that we can determine when a new output buffer is used.
		Index previousInputIndex = lastInputIndex;
		Index inputIndex = lastInputIndex = input.ioIndex;
		
		// Make sure that this input connection is valid. If not, skip it.
		if ( inputIndex >= numFilterInputs )
			continue;
		
		//*************************************************************************
		// Input node output is computed, send its output to the current node's input frame.
		
		Index outputIndex = inputNode->outputs[input.connectionIndex].ioIndex;
		const SoundBuffer* outputBuffer = inputNode->resultFrame->getBuffer( outputIndex );
		Size numValidSamples = inputNode->result.getSampleCount();
		
		// Skip NULL buffers and those without any valid samples.
		if ( outputBuffer != NULL && numValidSamples > 0 )
		{
			// Is this the first usage of this particular input index?
			if ( inputIndex != previousInputIndex )
			{
				if ( outputIsUnique( inputNode, input.connectionIndex ) )
				{
					// Since both the input and output are unique, we can copy the buffer pointer directly.
					inputFrame->setBuffer( input.ioIndex, (SoundBuffer*)outputBuffer );
					
					// Clean up the other result buffer, setting the buffer pointer to NULL.
					inputNode->resultFrame->setBuffer( outputIndex, NULL );
				}
				else
				{
					// Copy the buffer contents to the input buffer.
					SoundBuffer* inputBuffer;
					
					// Make sure that there is an input buffer.
					if ( (inputBuffer = inputFrame->getBuffer( input.ioIndex )) == NULL )
					{
						// Get a new buffer.
						inputBuffer = getTempBuffer();
						inputFrame->setBuffer( input.ioIndex, inputBuffer );
					}
					
					// Copy the data from the other buffer.
					outputBuffer->copyFormatTo( *inputBuffer, numValidSamples );
					outputBuffer->copyTo( *inputBuffer, numValidSamples );
				}
			}
			else
			{
				// Mix the buffer contents to the input buffer.
				SoundBuffer* inputBuffer;
				
				// Make sure that there is an input buffer.
				if ( (inputBuffer = inputFrame->getBuffer( input.ioIndex )) == NULL )
				{
					// Get a new buffer.
					inputBuffer = getTempBuffer();
					inputFrame->setBuffer( input.ioIndex, inputBuffer );
					
					// Copy the input buffer's format.
					outputBuffer->copyFormatTo( *inputBuffer, numSamples );
					
					// Copy the buffer's contents and zero the extra samples.
					outputBuffer->copyTo( *inputBuffer, numValidSamples );
					inputBuffer->zero( numValidSamples, numSamples );
				}
				else
				{
					// Mix the data from the other buffer.
					outputBuffer->mixTo( *inputBuffer, numValidSamples );
				}
			}
		}
		
		//******************************************************************
		// Check to see if the input node's result frame should be released.
		
		inputNode->numOutputsUsed++;
		
		// If all outputs of the node have been computed, we can release the
		// node's frame back to the pool.
		if ( inputNode->filter != NULL &&
			inputNode->numOutputsUsed == inputNode->outputs.getSize() )
		{
			releaseTempFrame( inputNode->resultFrame );
			
			inputNode->resultFrame = NULL;
			inputNode->numOutputsUsed = 0;
		}
	}
	
	//******************************************************************
	// Do the processing for this node.
	
	SoundResult result = node->filter->process( *inputFrame, *outputFrame, numSamples );
	
	
	//******************************************************************
	// Clean up the input frame.
	
	releaseTempFrame( inputFrame );
	
	
	return result;
}




//##########################################################################################
//##########################################################################################
//############		
//############		Filter Graph Node Output Computation Method
//############		
//##########################################################################################
//##########################################################################################




void FilterGraph:: computeNodeOutput( Node* node, Size numSamples )
{
	//******************************************************************
	// Compute All Node Inputs.
	
	// Get the total number of inputs that this node's filter has.
	const Size numFilterInputs = node->filter->getInputCount();
	
	// Check each input connection of this node to make sure that the inputs to the node
	// are already computed.
	const Size numInputConnections = node->inputs.getSize();
	
	for ( Index i = 0; i < numInputConnections; i++ )
	{
		NodeConnection& input = node->inputs[i];
		Node* inputNode = input.node;
		
		// Make sure that this input connection is valid. If not, skip it.
		if ( input.ioIndex >= numFilterInputs )
			continue;
		
		// Has this input node's output already been computed?
		if ( inputNode->resultFrame == NULL )
			computeNodeOutput( inputNode, numSamples );
	}
	
	//*************************************************************
	// Prepare a filter frame for the input node's output.
	
	const Size numFilterOutputs = node->filter->getOutputCount();
	SoundFrame* resultFrame = getTempFrame( numFilterOutputs );
	
	const Size numNodeOutputs = node->outputs.getSize();
	Index lastOutputIndex = math::max<Index>();
	
	for ( Index i = 0; i < numNodeOutputs; i++ )
	{
		NodeConnection& nodeOutput = node->outputs[i];
		
		Index previousOutputIndex = lastOutputIndex;
		Index outputIndex = nodeOutput.ioIndex;
		lastOutputIndex = outputIndex;
		
		// Skip invalid output indices or repeated indices.
		if ( outputIndex >= numFilterOutputs || outputIndex == previousOutputIndex )
			continue;
		
		// Add a buffer for this output.
		resultFrame->setBuffer( outputIndex, getTempBuffer() );
	}
	
	//*************************************************************
	// Process the input node.
	
	node->resultFrame = resultFrame;
	node->result = processNode( node, resultFrame, numSamples );
}




//##########################################################################################
//##########################################################################################
//############		
//############		Private Helper Methods
//############		
//##########################################################################################
//##########################################################################################




SoundFrame* FilterGraph:: getTempFrame( Size numBuffers )
{
	const Size numTempFrames = tempFrames.getSize();
	
	if ( numFramesInUse < numTempFrames )
	{
		for ( Index i = 0; i < numTempFrames; i++ )
		{
			SoundFrameInfo* info = tempFrames[i];
			
			if ( info->referenceCount == 0 )
			{
				info->referenceCount++;
				info->frame.setBufferCount( numBuffers );
				numFramesInUse++;
				
				return &info->frame;
			}
		}
		
		// Shouldn't happen.
		return NULL;
	}
	else
	{
		// Create a new frame.
		SoundFrameInfo* info = om::util::construct<SoundFrameInfo>();
		info->referenceCount++;
		info->frame.setBufferCount( numBuffers );
		
		// Add the new frame to this list of temporary frames.
		tempFrames.add( info );
		numFramesInUse++;
		
		return &info->frame;
	}
}




void FilterGraph:: releaseTempFrame( SoundFrame* frame )
{
	const Size numTempFrames = tempFrames.getSize();
	
	for ( Index i = 0; i < numTempFrames; i++ )
	{
		SoundFrameInfo* info = tempFrames[i];
		
		if ( frame == &info->frame )
		{
			// Clean up the buffers that the frame was using.
			Size numBuffers = frame->getBufferCount();
			
			for ( Index i = 0; i < numBuffers; i++ )
			{
				SoundBuffer* buffer = frame->getBuffer(i);
				
				if ( buffer != NULL )
					releaseTempBuffer( buffer );
				
				frame->setBuffer( i, NULL );
			}
			
			// Reduce the frame's reference count and return.
			info->referenceCount--;
			numFramesInUse--;
			return;
		}
	}
}




SoundBuffer* FilterGraph:: getTempBuffer()
{
	const Size numTempBuffers = tempBuffers.getSize();
	
	// Are there any free buffers in the pool?
	if ( numBuffersInUse < numTempBuffers )
	{
		for ( Index i = 0; i < numTempBuffers; i++ )
		{
			BufferInfo& info = tempBuffers[i];
			
			if ( info.referenceCount == 0 )
			{
				info.referenceCount++;
				numBuffersInUse++;
				
				return &info.buffer.getBuffer();
			}
		}
		
		// Shouldn't ever happen.
		return NULL;
	}
	else
	{
		// Get a new shared buffer and add it to the buffer pool.
		tempBuffers.add( BufferInfo( SharedBufferPool::getGlobalBuffer() ) );
		BufferInfo& info = tempBuffers.getLast();
		
		info.referenceCount++;
		numBuffersInUse++;
		
		return &info.buffer.getBuffer();
	}
}




void FilterGraph:: releaseTempBuffer( const SoundBuffer* buffer )
{
	const Size numTempBuffers = tempBuffers.getSize();
	
	for ( Index i = 0; i < numTempBuffers; i++ )
	{
		BufferInfo& info = tempBuffers[i];
		
		if ( buffer == &info.buffer.getBuffer() )
		{
			info.referenceCount--;
			numBuffersInUse--;
			return;
		}
	}
}




Bool FilterGraph:: outputIsUnique( Node* node, Index connectionIndex )
{
	Index outputIndex = node->outputs[connectionIndex].ioIndex;
	
	// If the output connection before or after this one has the same output index,
	// they share an output and the output is not unique.
	if ( connectionIndex > 0 &&
		node->outputs[connectionIndex - 1].ioIndex == outputIndex )
	{
		return false;
	}
	else if ( connectionIndex + 1 < node->outputs.getSize() &&
			node->outputs[connectionIndex + 1].ioIndex == outputIndex )
	{
		return false;
	}
	else
		return true;
}




//##########################################################################################
//*************************  End Om Sound Filters Namespace  *******************************
OM_SOUND_FILTERS_NAMESPACE_END
//******************************************************************************************
//##########################################################################################
