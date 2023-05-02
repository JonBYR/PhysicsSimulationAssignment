#pragma once

#include "BasicActors.h"
#include "VisualDebugger.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cmath>
namespace PhysicsEngine
{
	using namespace std;
	//using namespace VisualDebugger;
	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = {PxVec3(46.f/255.f,9.f/255.f,39.f/255.f),PxVec3(217.f/255.f,0.f/255.f,0.f/255.f),
		PxVec3(255.f/255.f,45.f/255.f,0.f/255.f),PxVec3(255.f/255.f,140.f/255.f,54.f/255.f),PxVec3(4.f/255.f,117.f/255.f,111.f/255.f)};

	//pyramid vertices
	static PxVec3 pyramid_verts[] = {PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1)};
	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = {1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1};
	static PxVec3 wedge_verts[] = { PxVec3(-1, 1, 0), PxVec3(1, 1, 0), PxVec3(1, 0, 0), PxVec3(-1, 0, 0), PxVec3(0, 0, 1), PxVec3(0, 0, -1) };
	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose=PxTransform(PxIdentity), PxReal density=1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), pose, density)
		{
		}
	};
	class Wedge : public ConvexMesh
	{
	public:
		Wedge(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
			ConvexMesh(vector<PxVec3>(begin(wedge_verts), end(wedge_verts)), pose, density)
		{
		}
	};
	class PyramidStatic : public TriangleMesh
	{
	public:
		PyramidStatic(PxTransform pose=PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs),end(pyramid_trigs)), pose)
		{
		}
	};
	class MySimulationEventCallback : public PxSimulationEventCallback //taken from tutorial 3
	{
	public:
		//an example variable that will be checked in the main simulation loop
		bool trigger;
		MySimulationEventCallback() : trigger(false) {}

		///Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);
		///Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
		{
			cerr << "Contact found between " << pairHeader.actors[0]->getName() << " " << pairHeader.actors[1]->getName() << endl;

			//check all pairs
			for (PxU32 i = 0; i < nbPairs; i++)
			{
				//check eNOTIFY_TOUCH_FOUND
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					cerr << "onContact::eNOTIFY_TOUCH_FOUND" << endl;
				}
				//check eNOTIFY_TOUCH_LOST
				if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					cerr << "onContact::eNOTIFY_TOUCH_LOST" << endl;
				}
			}
		}

		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) {}
		virtual void onWake(PxActor** actors, PxU32 count) {}
		virtual void onSleep(PxActor** actors, PxU32 count) {}
#if PX_PHYSICS_VERSION >= 0x304000
		virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}
#endif
	};
	///Custom scene class
	class MyScene : public Scene
	{
		bool iceMat = false;
		Plane* plane;
		Line* fieldLine;
		Box* box;
		Box* boxes[20];
		Sphere* sphere;
		Pyramid* pyr;
		Compound1* comp;
		Wedge* wed;
		RugbyBall* rug;
		FieldGoal* goal;
		Swing* swing;
		Box* brick;
		Box* trigger;
		Pulley* pulley;
		Box* windTrigger;
		RevoluteJoint chain;
		MySimulationEventCallback* my_callback;
		Line* playField;
		int currentRotation = 0; //used to store the value for quat
		int testDistance = 0;
		int finalVelocity = 0; 
		bool left = true;
	public:
		static Cloth* rugbyCloth;
		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_AXES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			srand(static_cast <unsigned> (time(0))); //functions for random float generation is using this Hasen (2009) Random float number generation. StackOverflow. Available from https://stackoverflow.com/questions/686353/random-float-number-generation [accessed 26 March 2023].
			//remember to adapt all the materials etc before they are added to the scene
			SetVisualisation();
			PxMaterial* steelMaterial = CreateMaterial(0.74f, 0.42f, 0);
			PxMaterial* grass = CreateMaterial(0.0f, 0.57f, 0.0f); //KentJacobson10 (2014) Friction coefficient of Mud and grass. PhysicsForum. Available from https://www.physicsforums.com/threads/friction-coefficient-of-mud-and-grass.756418/ [accessed 17 March 2023].
			//dynamic friction of grass
			plane = new Plane();
			plane->Material(grass);
			fieldLine = new Line(PxTransform(PxVec3(20, 0.1f, 25)), PxVec3(70, 0.5f, 0.1f));
			Add(fieldLine); //fieldLine parameters taken from here https://www.harrodsport.com/advice-and-guides/rugby-pitch-dimensions-markings
			PxMaterial* wood = CreateMaterial(0.5, 0.32f, 0.603f); //The Engineering Toolbox (undated) Friction – Friction Coefficients and Calculator. The Engineering Toolbox. Available from https://www.engineeringtoolbox.com/friction-coefficients-d_778.html [accessed 20 March 2023]. friction for wood
			//The Physics Factbook (undated) Coefficients of Restitution. The Physics Factbook. Available from https://hypertextbook.com/facts/2006/restitution.shtml [accessed 20 March 2023]. restitution for wood
			//Wikipedia (undated) Friction. Wikipedia. Available from https://en.wikipedia.org/wiki/Friction#Approximate_coefficients_of_friction [accessed 20 March 2023]. dynamic friction of wood
			PxMaterial* rubber = CreateMaterial(1.15f, 0.0f, 0.828f); //rugby balls are made of a "rubber/polyster compound" Willacy, G. (2020) The life of a rugby ball: from a rubber tree in India to Super League. The Guardian, 17 August. Available from https://www.theguardian.com/sport/no-helmets-required/2020/aug/17/rugby-league-ball-steeden-rubber-tree-india-super-league [accessed 13 March 2023].
			//The Physics Factbook (undated) Coefficients of Restitution. The Physics Factbook. Available from https://hypertextbook.com/facts/2006/restitution.shtml [accessed 20 March 2023]. restitution for rubber
			//Engineers Edge (undated) Coefficient of Friction Equation and Table Chart. Engineers Edge. Available from https://www.engineersedge.com/coeffients_of_friction.htm [accessed 13 March 2023]. friction for rubber
			PxMaterial* brickMat = CreateMaterial(0.6f, 0.5f, 0.0f); //material for a brick from Gregersen, E. (undated) coefficient of friction. Britannica. Available from https://www.britannica.com/science/coefficient-of-friction [accessed 13 March 2023].
			my_callback = new MySimulationEventCallback();
			px_scene->setSimulationEventCallback(my_callback);
			plane->Color(PxVec3(178/255.f,210/255.f,0/255.f));
			Add(plane);
			playField = new Line(PxTransform(PxVec3(20, 0, 25)), PxVec3(70, 0.1, 100)); //Harrod Sport (2019) A Guide to Rugby Pitch Dimensions, Sizes and Markings: Everything you ever needed to know. Harrod Sport. Available from https://www.harrodsport.com/advice-and-guides/rugby-pitch-dimensions-markings [accessed 27 March 2023]. values of a rugby pitch
			playField->Material(grass); 
			playField->Color(PxVec3(0 / 255.f, 210 / 255.f, 0 / 255.f));
			Add(playField);
			goal = new FieldGoal(PxVec3(0.3f, 16.0f, 0.3f), PxTransform(PxVec3(-5.6f, 8.0f, -25.0f))); //dimensions taken from this: Net World Sports (undated) Rugby Post Dimensions. Net World Sports. Available from https://www.networldsports.co.uk/buyers-guides/rugby-post-size-guide [accessed 1 March 2023].
			goal->Material(steelMaterial);
			Add(goal);
			goal->Get()->is<PxRigidStatic>()->setGlobalPose(PxTransform(PxVec3(-5.6f, 16.0f, -25.0f))); //adapted as goal should be 16 metres high
			//density is mass / volume
			rug = new RugbyBall(PxTransform(PxVec3(-3.0f, 0, 0), PxQuat(90, PxVec3(0, 0, 1))), 0.1f, 91.7);
			rug->Material(rubber);
			rug->Name("Ball");
			rug->Color(PxVec3(134/255.0f, 54/255.0f, 0/255.0f));
			rug->Get()->is<PxRigidDynamic>()->setMass(0.41); // Gilbert Rugby (2021) Rugby Balls – Which Ball Do I Need? Gilbert Rugby. Available from https://www.gilbertrugby.com/blogs/news/rugby-balls-which-ball-do-i-need#:~:text=7%20facts%20about%20Rugby%20Balls,and%20made%20of%20four%20panels.&text=It%20weighs%20410%2D460%20grams,or%20a%20suitable%20synthetic%20material [accessed 4 March 2023].
			//assuming units are base SI units of mass (kg) and that all length measurements are in m: NIST (undated) SI Units. NIST. Available from https://www.nist.gov/pml/owm/metric-si/si-units [accessed 1 March 2023].
			Add(rug);
			swing = new Swing(PxVec3(0.5, 4, 0.5), PxTransform(PxVec3(-8.6, 0, 0)));
			swing->Get()->is<PxRigidStatic>()->setGlobalPose(PxTransform(PxVec3(-8.6f, 4.0f, 0.0f)));
			swing->Material(wood);
			Add(swing);
			brick = new Box(PxTransform(PxVec3(-5, 8, 3)), PxVec3(0.215, 0.1025, 0.065), 1424); //volume from Brick Development Association (2018) Designing To Brickwork Dimensions. Brick Development Association. Available from https://www.brick.org.uk/admin/resources/designing-to-brickwork-dimensions.pdf [accessed 18 March 2023]. (p4) (converted from mm) 
			//density calculated using density formulae
			brick->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			brick->Get()->is<PxRigidDynamic>()->setMass(2.04); //James, D. (undated) Average Brick Weights in Different Sizes. SHFT blog. Available from https://www.shtfblog.com/average-brick-weights-in-different-sizes/ [accessed 18 March 2023].
			brick->Color(PxVec3(255.0f / 255.0f, 64.7f / 255.0f, 0 / 255.0f));
			brick->Material(brickMat);
			Add(brick);
			chain.setJoints(swing, PxTransform(PxVec3(0.5, 8, 0.5)), brick, PxTransform(PxVec3(-4, 6, -10), PxQuat(90, PxVec3(1.f, 0.f, 0.f))));
			chain.setBreakLimit(8000, 3000); //first component is force and second component is torque
			//max force of a chain comes from this Zero Friction Cycling (2019) Chain Tensile Strength Test Product Brief. Zero Friction Cycling. Available from https://zerofrictioncycling.com.au/wp-content/uploads/2019/07/Chain-Tensile-Strength-Test-Brief-converted.pdf [accessed 24 March 2023].(p14)
			trigger = new Box(PxTransform(PxVec3(0, 20, -25.0f)), PxVec3(5.8, 13, 0.1));
			trigger->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true); //trigger should not have gravity
			trigger->SetTrigger(true);
			trigger->Color(PxVec3(254/255.0f, 0/255.0f, 0/255.0f));
			trigger->Name("Finish");
			Add(trigger); //Use trigger to get some kind of end condition
			pulley = new Pulley(PxTransform(PxVec3(-12.9f, 0.5f, -4.0f))); //compound shape of spheres and a box
			pulley->Get()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			pulley->Color(PxVec3(134 / 255.0f, 54 / 255.0f, 0 / 255.0f));
			pulley->Material(wood);
			//no need to set mass for a kinematic actor
			Add(pulley);
			windTrigger = new Box(PxTransform(PxVec3(-5.5f, 9.0f, -20.0f)), PxVec3(1.5, 1.5, 1.5));
			windTrigger->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
			windTrigger->Name("Wind");
			windTrigger->SetTrigger(true);
			Add(windTrigger);
			rugbyCloth = new Cloth(PxTransform(PxVec3(-4.2f, 7.0f, -25.0f)), PxVec2(8.4, 5));
			rugbyCloth->Color(PxVec3(255 / 255, 0, 0));
			Add(rugbyCloth);
			//different sized brick values found from a Church created in 1160 Symonds, R (2013) Bricks Through History. Sussex Archaeological Society. Available from https://www.brocross.com/Bricks/Penmorfa/Bricks/Bits/BRICKS%20THROUGH%20HISTORY.pdf [accessed 28 March 2023]. (p10) and converted from inches
			std::vector<Box*> bricks;
			for (int i = 0; i < 10; i++) 
			{
				float horizontalSize = -7.5f + (i - 0.95);
				float vertical = 0.5f;
				for (int j = 0; j < 20; j++) 
				{
					Box* wallBrick = new Box(PxTransform(PxVec3(horizontalSize, vertical, -5.5)), PxVec3(0.3302f, 0.1524f, 0.0508), 1424); //density set to be the same
					wallBrick->Get()->is<PxRigidDynamic>()->setMass(3.64028794522); //set Mass using density x volume
					wallBrick->Material(brickMat);
					wallBrick->Color(PxVec3(255 / 255, 0, 0)); //Brick is described as dark red
					Add(wallBrick);
					wallBrick->Get()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
					bricks.push_back(wallBrick);
					vertical += 0.3f;
				}
				vertical = 0.5f;
			}
			for (int k = 0; k < bricks.size(); k++) 
			{
				bricks[k]->Get()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
			}
		}
		void AddCannonBall(PxVec3 camo, PxVec3 came) 
		{
			float randomRadius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float volume = (4 / 3) * PxPi * (pow(randomRadius, 3)); //volume of a sphere formula
			float density = 5.5 / volume; //density = mass / volume
			Rock* r; //originally called a rock but is now a cannonball
			r = new Rock(PxTransform(came), randomRadius, density); //density comes from link for mass
			PxMaterial* steelMaterial = CreateMaterial(0.74f, 0.42f, 0); //Wikipedia (undated) Friction. Wikipedia. Available from https://en.wikipedia.org/wiki/Friction#Approximate_coefficients_of_friction [accessed 20 March 2023]. for steel
			r->Material(steelMaterial);
			r->Name("Cannon");
			r->Color(PxVec3(60/255.0f, 60/255.0f, 60/255.0f));
			Add(r);
			r->Get()->is<PxRigidDynamic>()->setMass(5.5); //Allain, R. (2011) Cannonballs: Size Matters. WIRED, 9 December. Available from https://www.wired.com/2011/12/cannon-balls-size-matters/#:~:text=The%20ball%20is%20made%20of,the%20cannon%20ball%205.5%20kg. [accessed 4 March 2023].
			r->Get()->is<PxRigidDynamic>()->addForce(PxVec3(camo.x, camo.y, camo.z) * 10000); //simulates shooting from a cannon
		}
		void release() 
		{
			float yForce = brick->Get()->is<PxRigidDynamic>()->getMass() * px_scene->getGravity().y;
			//this formula is equvilant to f = ma or f = mgj (g being acceleration under gravity and j being the normal vector in the j direction (which is 0, 1, 0))
			brick->Get()->is<PxRigidDynamic>()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, false); //apply gravity so it moves
			brick->Get()->is<PxRigidDynamic>()->addForce(PxVec3(0, -yForce, 0)); //needs to go anticlockwise
			chain.DriveVelocity(-yForce * 2); //apply motor
		}
		//Custom udpate function
		virtual void CustomUpdate(PxReal dt) 
		{
			PxVec3 pulleyPosition = pulley->Get()->is<PxRigidDynamic>()->getGlobalPose().p; //pulley works as a moving platform to get in the way of the free kick
			if (left) 
			{
				pulley->Get()->is<PxRigidDynamic>()->setGlobalPose(PxTransform(PxVec3(pulleyPosition.x + 0.1, pulleyPosition.y, pulleyPosition.z)));
				if (pulleyPosition.x >= 7.9f) left = false;
			}
			else if (!left) 
			{
				pulley->Get()->is<PxRigidDynamic>()->setGlobalPose(PxTransform(PxVec3(pulleyPosition.x - 0.1, pulleyPosition.y, pulleyPosition.z)));
				if (pulleyPosition.x <= -12.9f) left = true;
			}
		}
		void changeGround() 
		{
			if (!iceMat) 
			{
				PxMaterial* ice = CreateMaterial(0.05, 0.04, 0); //Mills, A., 2008. The coefficient of friction, particularly of ice. Physics Education, 43(4), p.392.
				plane->Material(ice);
				playField->Material(ice);
				plane->Color(PxVec3(100/255.0f, 100/255.0f, 255 /255.0f));
				playField->Color(PxVec3(100/255, 100/255, 255/255));
				iceMat = true;
			}
			else if (iceMat) 
			{
				PxMaterial* grass = CreateMaterial(0, 0.57f, 0);
				plane->Material(grass);
				playField->Material(grass);
				plane->Color(PxVec3(178 / 255.f, 210 / 255.f, 0 / 255.f));
				playField->Color(PxVec3(0 / 255.f, 210 / 255.f, 0 / 255.f));
				iceMat = false;
			}
		}
		static void ClothForce(PxVec3 force) 
		{
			std::cout << "Cloth Called" << std::endl;
			rugbyCloth->Get()->is<PxCloth>()->setExternalAcceleration(force);
		}
	};
}
