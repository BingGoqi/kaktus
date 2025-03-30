#pragma once
#include "jolt_pch.h"
#include <cstdarg>
#include <iostream>
namespace jt
{
	static void TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);
		// Print to the TTY
		std::cout << buffer << std::endl;
	}
#ifdef JPH_ENABLE_ASSERTS
	// Callback for asserts, connect this to your own assert handler if you have one
	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine)
	{
		// Print to the TTY
		std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;
		// Breakpoint
		return true;
	};
#endif // JPH_ENABLE_ASSERTS
	// Layer that objects can be in, determines which other objects it can collide with
	// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
	// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
	// but only if you do collision testing).
	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};
	// Class that determines if two object layers can collide
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
		{
			return (inObject1 == inObject2 && 0 != inObject1);
		}
	};
	// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
	// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
	// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
	// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
	// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.

	// BroadPhaseLayerInterface implementation
	// This defines a mapping between object and broadphase layers.
	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			for (int i = 0; i < 16; i++)
			{
				mObjectToBroadPhase[i] = JPH::BroadPhaseLayer(i);
			}
		}
		virtual JPH::uint GetNumBroadPhaseLayers() const override
		{
			return 16;
		}
		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
		{
			JPH_ASSERT(inLayer < 16);
			return JPH::BroadPhaseLayer(inLayer);
		}
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override
		{
			return "0-15";
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED
	private:
		JPH::BroadPhaseLayer mObjectToBroadPhase[16];
	};
	// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
		{
			return (inLayer1 == inLayer2.GetValue() && 0 != inLayer1);
		}
	};
	class PhysicsSystem
	{
	public:
		static JPH::PhysicsSystem* GetJps()
		{
			if (only == nullptr)
				only = create();
			return only;
		}
		static void  free()
		{
			// Unregisters all types with the factory and cleans up the default material
			JPH::UnregisterTypes();
			// Destroy the factory
			delete JPH::Factory::sInstance;
			JPH::Factory::sInstance = nullptr;
		}
	private:
		static JPH::PhysicsSystem* only;
		static JPH::PhysicsSystem* create()
		{
			JPH::RegisterDefaultAllocator();

			// Install trace and assert callbacks
			JPH::Trace = TraceImpl;
			JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

			// Create a factory, this class is responsible for creating instances of classes based on their name or hash and is mainly used for deserialization of saved data.
			// It is not directly used in this example but still required.
				JPH::Factory::sInstance = new JPH::Factory();

			// Register all physics types with the factory and install their collision handlers with the CollisionDispatch class.
			// If you have your own custom shape types you probably need to register their handlers with the CollisionDispatch before calling this function.
			// If you implement your own default material (PhysicsMaterial::sDefault) make sure to initialize it before this function or else this function will create one for you.
			JPH::RegisterTypes();

			// We need a temp allocator for temporary allocations during the physics update. We're
			// pre-allocating 10 MB to avoid having to do allocations during the physics update.
			// B.t.w. 10 MB is way too much for this example but it is a typical value you can use.
			// If you don't want to pre-allocate you can also use TempAllocatorMalloc to fall back to
			// malloc / free.
			JPH::TempAllocatorImpl temp_allocator(10 * 1024 * 1024);

			// We need a job system that will execute physics jobs on multiple threads. Typically
			// you would implement the JobSystem interface yourself and let Jolt Physics run on top
			// of your own job scheduler. JobSystemThreadPool is an example implementation.
			JPH::JobSystemThreadPool job_system(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);

			BPLayerInterfaceImpl broad_phase_layer_interface;

			// Create class that filters object vs broadphase layers
			// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
			// Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
			ObjectVsBroadPhaseLayerFilterImpl object_vs_broadphase_layer_filter;

			// Create class that filters object vs object layers
			// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
			// Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
			ObjectLayerPairFilterImpl object_vs_object_layer_filter;
			JPH::PhysicsSystem physics_system;
			physics_system.Init(1024, 0, 1024, 1024, broad_phase_layer_interface, object_vs_broadphase_layer_filter, object_vs_object_layer_filter);
			return &physics_system;
		}
		~PhysicsSystem() {}
		PhysicsSystem(const PhysicsSystem&) = delete;
		PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	};
}