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

#ifndef INCLUDE_OM_RESOURCE_PROTOTYPE_H
#define INCLUDE_OM_RESOURCE_PROTOTYPE_H


#include "omResourcesConfig.h"


#include "omOpaqueResource.h"
#include "omResourcePrototypeFlags.h"


//##########################################################################################
//***************************  Start Om Resources Namespace  *******************************
OM_RESOURCES_NAMESPACE_START
//******************************************************************************************
//##########################################################################################



//********************************************************************************
/// A class that represents a template for a group of resources that can be instantiated as a module.
class Prototype
{
	public:
		
		//********************************************************************************
		//******	Constructor
			
			
			/// A class that stores information about a resource that belongs to a prototype.
			class Component
			{
				public:
					
					/// Create a new component with the specified resource and the default flags.
					OM_INLINE Component( const OpaqueResource& newResource )
						:	resource( newResource ),
							flags( PrototypeFlags::DEFAULT )
					{
					}
					
					/// Create a new component with the specified resource and flags.
					OM_INLINE Component( const OpaqueResource& newResource, const PrototypeFlags& newFlags )
						:	resource( newResource ),
							flags( newFlags )
					{
					}
					
					
					/// An opaque reference to the resource for this component.
					OpaqueResource resource;
					
					
					/// An object containing boolean configuration flags for this component.
					PrototypeFlags flags;
					
					
			};
			
			
		//********************************************************************************
		//******	Constructor
			
			
			/// Create a new empty resource prototype.
			Prototype();
			
			
		//********************************************************************************
		//******	Destructor
			
			
			/// Destroy a resource prototype.
			~Prototype();
			
			
		//********************************************************************************
		//******	Component Accessor Methods
			
			
			/// Return the number of components there are in this resource prototype.
			OM_INLINE Size getComponentCount() const
			{
				return components.getSize();
			}
			
			
			/// Return a reference to the component at the specified index in this resource prototype.
			OM_INLINE Component& getComponent( Index componentIndex )
			{
				return components[componentIndex];
			}
			
			
			/// Return a const reference to the component at the specified index in this resource prototype.
			OM_INLINE const Component& getComponent( Index componentIndex ) const
			{
				return components[componentIndex];
			}
			
			
			/// Return whether or not this prototype contains the specified resource data object.
			Bool hasComponent( const void* data ) const;
			
			
			/// Get the index of the component in this prototype with the specified resource data.
			Bool getComponentIndex( const void* data, Index& componentIndex ) const;
			
			
			/// Add a new component to the end of this resource prototype.
			/**
			  * The method returns whether or not the component was able to be added.
			  */
			void addComponent( const Component& newComponent );
			
			
			/// Insert a new component at the specified index within this resource prototype.
			/**
			  * The method returns whether or not the component was able to be inserted.
			  */
			Bool insertComponent( Index componentIndex, const Component& newComponent );
			
			
			/// Remove the component in this prototype at the specified index.
			void removeComponent( Index componentIndex );
			
			
			/// Remove all components from this resource prototype.
			void clearComponents();
			
			
		//********************************************************************************
		//******	Resource Type Declaration
			
			
			/// The resource type for a resource prototype.
			static const ResourceType RESOURCE_TYPE;
			
			
	private:
		
		//********************************************************************************
		//******	Private Data Members
			
			
			/// A list of the components that are in this resource prototype.
			ArrayList<Component> components;
			
			
			
};




//##########################################################################################
//***************************  End Om Resources Namespace  *********************************
OM_RESOURCES_NAMESPACE_END
//******************************************************************************************
//##########################################################################################


OM_RESOURCE_TYPE( om::resources::Prototype, om::resources::Prototype::RESOURCE_TYPE );


#endif // INCLUDE_OM_RESOURCE_PROTOTYPE_H
