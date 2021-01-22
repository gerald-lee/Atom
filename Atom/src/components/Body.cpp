#include <Pch.hpp>
#include "Body.hpp"
#include "Transform.hpp"
//#include "../GameObject.h"

Body::Body(Transform* transform) //: Component(TYPE_BODY)
{
	positionX = 0.0f, positionY = 0.0f;
	prevPositionX = 0.0f, prevPositionY = 0.0f;
	velocityX = 0.0f, velocityY = 0.0f;
	accelerationX = 0.0f, accelerationY = 0.0f;
	totalForceX = 0.0f, totalForceY = 0.0f;
	mass = 1.0f, invMass = 1.0f;

	shape = nullptr;

	this->transform = transform;
	//Transform* transform = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
}

Body::~Body()
{
	delete shape;
}

void Body::Update()
{

}

void Body::Integrate(bool Gravity, float DeltaTime) 
{

	if (transform != nullptr)
	{
		positionX = transform->positionX;
		positionY = transform->positionY;
	}

	prevPositionX = positionX;
	prevPositionY = positionY;

	//Compute acceleration
	//mTotalForceY += Gravity;
	accelerationX = totalForceX * mass;
	accelerationY = Gravity ? (totalForceY - 9.81) * mass : 0;

	//Integrate velocity
	velocityX = accelerationX * DeltaTime + velocityX;
	velocityY = Gravity ? accelerationY * DeltaTime + velocityY : 0;

	if (transform != nullptr)
	{
		positionX = positionX + velocityX * DeltaTime;
		positionY = positionY + velocityY * DeltaTime;
		transform->positionX = positionX;
		transform->positionY = positionY;
	}


}
