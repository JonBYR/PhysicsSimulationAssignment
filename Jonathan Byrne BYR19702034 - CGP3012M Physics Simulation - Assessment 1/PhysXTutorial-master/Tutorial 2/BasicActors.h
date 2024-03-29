#pragma once

#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	///Plane class
	class Plane : public StaticActor
	{
	public:
		//A plane with default paramters: XZ plane centred at (0,0,0)
		Plane(PxVec3 normal=PxVec3(0.f, 1.f, 0.f), PxReal distance=0.f) 
			: StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance)))
		{
			CreateShape(PxPlaneGeometry());
		}
	};
	class FieldGoal : public StaticActor
	{
	public:
		FieldGoal(PxVec3 dimensions, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f) : StaticActor(pose) 
		{
			for (int i = 0; i <= 2; i++) 
			{
				if (i >= 2) 
				{
					std::cout << "Called";
					CreateShape(PxBoxGeometry(PxVec3(5.6f, 0.3f, 0.3f)), density); //midpoint distance between two poles
					GetShape(2)->setLocalPose(PxTransform(PxVec3(5.6f, -9, pose.p.z + 25))); //middle bar should be 3 meters above ground (-13 in local pose) however this looked too low down in the simulation
					//so 7m down is used instead (-9 in local pose)
				}
				else 
				{
					CreateShape(PxBoxGeometry(dimensions), density);
				}
			}
			GetShape(1)->setLocalPose(PxTransform(PxVec3(pose.p.x + 16.8, 0, pose.p.z + 25)));
		}
	};
	class Swing : public StaticActor 
	{
	public:
		Swing(PxVec3 dimensions, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f) : StaticActor(pose) 
		{
			for (int i = 0; i <= 2; i++)
			{
				if (i >= 2)
				{
					std::cout << "Called";
					CreateShape(PxBoxGeometry(PxVec3(6, 0.5, 0.5)), density); //midpoint distance between two poles
					GetShape(2)->setLocalPose(PxTransform(PxVec3(4.6, 4, 0))); //0 in y is not 0 of plane
					//this feels too small, may have to change later
				}
				else
				{
					CreateShape(PxBoxGeometry(dimensions), density);
				}
			}
			GetShape(1)->setLocalPose(PxTransform(PxVec3(pose.p.x + 18, 0, 0)));
		}
	};
	class StaticBox : public StaticActor 
	{
	public:
		StaticBox(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);
		}
	};
	///Sphere class
	class Sphere : public DynamicActor
	{
	public:
		//a sphere with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m
		// - denisty: 1kg/m^3
		Sphere(const PxTransform& pose=PxTransform(PxIdentity), PxReal radius=1.f, PxReal density=1.f) 
			: DynamicActor(pose)
		{ 
			CreateShape(PxSphereGeometry(radius), density);
		}
	};
	class Rock : public DynamicActor 
	{
	public:
		Rock(const PxTransform& pose = PxTransform(PxIdentity), PxReal radius = 1.f, PxReal density = 1.f)
			: DynamicActor(pose) 
		{
			CreateShape(PxSphereGeometry(radius), density);
		}
	};
	class RugbyBall : public DynamicActor 
	{
	public:
		RugbyBall(const PxTransform& pose = PxTransform(PxIdentity), PxReal radius = 0.1f, PxReal density = 1.f) : DynamicActor(pose)
		{
			PxReal rad = radius; //may have to change density of rugby ball later
			float distance = radius;
			for (int i = 0; i < 5; i++) 
			{
				CreateShape(PxSphereGeometry(rad), density);
				GetShape(i)->setLocalPose(PxTransform(PxVec3((distance), 0, 0)));
				if (i < 2) 
				{
					rad += 0.2f;
				}
				else if (i >= 2) 
				{
					rad -= 0.2f;
				}
				distance += 0.3;
			}
			//Volume of a rugby ball is roughly 0.004471 m ^ 3: IB Maths Resources (2019) Finding the volume of a rugby ball (or American football). IB Maths Resources. Available from https://ibmathsresources.com/2019/10/19/finding-the-volume-of-a-rugby-ball-or-american-football/ [accessed 4 March 2023]. (converted from cm^3)
		}
	};
	///Box class
	class Box : public DynamicActor
	{
	public:
		//a Box with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m x 1m x 1m
		// - denisty: 1kg/m^3
		Box(const PxTransform& pose=PxTransform(PxIdentity), PxVec3 dimensions=PxVec3(.5f,.5f,.5f), PxReal density=1.f)
			: DynamicActor(pose)
		{ 
			CreateShape(PxBoxGeometry(dimensions), density);
		}

	};
	class Line : public StaticActor
	{
	public:
		Line(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
			: StaticActor(pose)
		{
			CreateShape(PxBoxGeometry(dimensions), density);
		}

	};
	class Compound1 : public DynamicActor 
	{
	public:
		Compound1(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
			: DynamicActor(pose)
		{
			for (int i = 0; i < 5; i++) 
			{
				CreateShape(PxBoxGeometry(dimensions), density);
				if (i > 0) 
				{
					GetShape(i)->setLocalPose(PxTransform(PxVec3(i, 0.0f, 0.0f)));
				}
			}
		}
	};
	class Capsule : public DynamicActor
	{
	public:
		Capsule(const PxTransform& pose=PxTransform(PxIdentity), PxVec2 dimensions=PxVec2(1.f,1.f), PxReal density=1.f) 
			: DynamicActor(pose)
		{
			CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
		}
	};

	///The ConvexMesh class
	class ConvexMesh : public DynamicActor
	{
	public:
		//constructor
		ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose=PxTransform(PxIdentity), PxReal density=1.f)
			: DynamicActor(pose)
		{
			PxConvexMeshDesc mesh_desc;
			mesh_desc.points.count = (PxU32)verts.size();
			mesh_desc.points.stride = sizeof(PxVec3);
			mesh_desc.points.data = &verts.front();
			mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
			mesh_desc.vertexLimit = 256;

			CreateShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
		}

		//mesh cooking (preparation)
		PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc)
		{
			PxDefaultMemoryOutputStream stream;

			if(!GetCooking()->cookConvexMesh(mesh_desc, stream))
				throw new Exception("ConvexMesh::CookMesh, cooking failed.");

			PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

			return GetPhysics()->createConvexMesh(input);
		}
	};

	///The TriangleMesh class
	class TriangleMesh : public StaticActor
	{
	public:
		//constructor
		TriangleMesh(const std::vector<PxVec3>& verts, const std::vector<PxU32>& trigs, const PxTransform& pose=PxTransform(PxIdentity))
			: StaticActor(pose)
		{
			PxTriangleMeshDesc mesh_desc;
			mesh_desc.points.count = (PxU32)verts.size();
			mesh_desc.points.stride = sizeof(PxVec3);
			mesh_desc.points.data = &verts.front();
			mesh_desc.triangles.count = (PxU32)trigs.size()/3;
			mesh_desc.triangles.stride = 3*sizeof(PxU32);
			mesh_desc.triangles.data = &trigs.front();

			CreateShape(PxTriangleMeshGeometry(CookMesh(mesh_desc)));
		}

		//mesh cooking (preparation)
		PxTriangleMesh* CookMesh(const PxTriangleMeshDesc& mesh_desc)
		{
			PxDefaultMemoryOutputStream stream;

			if(!GetCooking()->cookTriangleMesh(mesh_desc, stream))
				throw new Exception("TriangleMesh::CookMesh, cooking failed.");

			PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

			return GetPhysics()->createTriangleMesh(input);
		}
	};
	class Pulley : public DynamicActor 
	{
	public:
		Pulley(const PxTransform& pose, PxVec3 dimensions = PxVec3(2.6f, 2.5f, 2.0f), PxReal density = 1.f, PxReal radius = 0.4f) : DynamicActor(pose)
		{
			for (int i = 0; i < 5; i++) 
			{
				if (i != 4) 
				{
					CreateShape(PxSphereGeometry(radius), density);
				}
				else 
				{
					CreateShape(PxBoxGeometry(dimensions), density);
				}
			}
			GetShape(1)->setLocalPose(PxTransform(PxVec3(0, 0, 2)));
			GetShape(2)->setLocalPose(PxTransform(PxVec3(2.6, 0, 0)));
			GetShape(3)->setLocalPose(PxTransform(PxVec3(2.6, 0, 2)));
			GetShape(4)->setLocalPose(PxTransform(PxVec3(1.3f, 3, 1)));
		}
	};
	class RevoluteJoint : public Joint
	{
	public:
		RevoluteJoint() //have default constructor 
		{

		}
		RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
		{
			PxRigidActor* px_actor0 = 0;
			if (actor0)
				px_actor0 = (PxRigidActor*)actor0->Get();

			joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}
		void setJoints(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1) //have a setter 
		{
			PxRigidActor* px_actor0 = 0;
			if (actor0)
				px_actor0 = (PxRigidActor*)actor0->Get();

			joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
			joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		}
		void DriveVelocity(PxReal value)
		{
			//wake up the attached actors
			PxRigidStatic* actor_0;
			PxRigidDynamic* actor_1;
			((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor_0, (PxRigidActor*&)actor_1);
			if (actor_1)
			{
				if (actor_1->isSleeping())
					actor_1->wakeUp();
			}
			((PxRevoluteJoint*)joint)->setDriveVelocity(value);
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
		}

		PxReal DriveVelocity()
		{
			return ((PxRevoluteJoint*)joint)->getDriveVelocity();
		}

		void SetLimits(PxReal lower, PxReal upper)
		{
			((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
			((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
		}
		void setBreakLimit(PxReal breakagex, PxReal breakagey) 
		{
			((PxRevoluteJoint*)joint)->setBreakForce(breakagex, breakagey);
		}
	};
	class Cloth : public Actor //taken from tutorial 4
	{
		PxClothMeshDesc mesh_desc;

	public:
		//constructor
		Cloth(PxTransform pose = PxTransform(PxIdentity), const PxVec2& size = PxVec2(1.f, 1.f), PxU32 width = 1, PxU32 height = 1, bool fix_top = true)
		{
			//prepare vertices
			PxReal w_step = size.x / width;
			PxReal h_step = size.y / height;

			PxClothParticle* vertices = new PxClothParticle[(width + 1) * (height + 1) * 4];
			PxU32* quads = new PxU32[width * height * 4];

			for (PxU32 j = 0; j < (height + 1); j++)
			{
				for (PxU32 i = 0; i < (width + 1); i++)
				{
					PxU32 offset = i + j * (width + 1);
					vertices[offset].pos = PxVec3(w_step * i, 0.f, h_step * j);
					if (fix_top && (j == 0)) //fix the top row of vertices
						vertices[offset].invWeight = 0.f;
					else
						vertices[offset].invWeight = 1.f;
				}

				for (PxU32 j = 0; j < height; j++)
				{
					for (PxU32 i = 0; i < width; i++)
					{
						PxU32 offset = (i + j * width) * 4;
						quads[offset + 0] = (i + 0) + (j + 0) * (width + 1);
						quads[offset + 1] = (i + 1) + (j + 0) * (width + 1);
						quads[offset + 2] = (i + 1) + (j + 1) * (width + 1);
						quads[offset + 3] = (i + 0) + (j + 1) * (width + 1);
					}
				}
			}
			//init cloth mesh description
			mesh_desc.points.data = vertices;
			mesh_desc.points.count = (width + 1) * (height + 1);
			mesh_desc.points.stride = sizeof(PxClothParticle);

			mesh_desc.invMasses.data = &vertices->invWeight;
			mesh_desc.invMasses.count = (width + 1) * (height + 1);
			mesh_desc.invMasses.stride = sizeof(PxClothParticle);

			mesh_desc.quads.data = quads;
			mesh_desc.quads.count = width * height;
			mesh_desc.quads.stride = sizeof(PxU32) * 4;

			//create cloth fabric (cooking)
			PxClothFabric* fabric = PxClothFabricCreate(*GetPhysics(), mesh_desc, PxVec3(0, -1, 0));

			//create cloth
			actor = (PxActor*)GetPhysics()->createCloth(pose, *fabric, vertices, PxClothFlags());
			//collisions with the scene objects
			((PxCloth*)actor)->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);

			colors.push_back(default_color);
			actor->userData = new UserData(&colors.back(), &mesh_desc);
		}
		~Cloth()
		{
			delete (UserData*)actor->userData;
		}
	};
}