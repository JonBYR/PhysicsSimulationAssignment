#include "MyPhysicsEngine.h"
#include "VisualDebugger.h"
namespace PhysicsEngine 
{
	Cloth* MyScene::rugbyCloth;
	//using namespace VisualDebugger;
	void MySimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		//you can read the trigger information here
		for (PxU32 i = 0; i < count; i++)
		{
			//filter out contact with the planes
			if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
			{
				std::cout << "True" << std::endl;
				//check if eNOTIFY_TOUCH_FOUND trigger
				if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					printf("trigger %s ", pairs[i].otherActor->getName());
					if ((strcmp(pairs[i].otherActor->getName(), "Ball") == 0) && trigger != true && (strcmp(pairs[i].triggerActor->getName(), "Finish") == 0)) //checks if both strings are the same
					{
						VisualDebugger::BallEntered();
						cerr << "onTrigger::eNOTIFY_TOUCH_FOUND" << endl;
						trigger = true;
					}
					if ((strcmp(pairs[i].triggerActor->getName(), "Wind") == 0 && trigger != true)) 
					{
						pairs[i].otherActor->is<PxRigidDynamic>()->addForce(PxVec3(614.023030016f, 0, 0)); //adds a force to replicate wind acting on the ball should thistrigger be hit
						MyScene::ClothForce(PxVec3(614.023030016f, 0, 0)); //apply the force to the cloth
						//Sciencing (2020) How to Convert Wind Speed to Force. Sciencing. Available from https://sciencing.com/convert-wind-speed-force-5985528.html [accessed 25 March 2023].
						//formula is air mass x density of air mass x acceleration. This is taken as 1 x 1.229 x 22.352 ^ 2 ((22.352 being a 50mph wind)
					}
				}
			}
		}
	}
}