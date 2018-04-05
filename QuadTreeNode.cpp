#include "QuadTreeNode.h"



QuadTreeNode::QuadTreeNode()
{
}


QuadTreeNode::~QuadTreeNode()
{
}

void QuadTreeNode::addGameObjectToTree(GameObject* gameObject)
{
	DirectX::XMVECTOR corners[8];

	for (int i = 0; i < gameObjects.size(); i++)
	{


		OOBB otherOBB = gameObjects[i]->OOBoundingBox;
		DirectX::XMVECTOR otherPosition = gameObjects[i]->transform.getPosition();

		corners[0]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[1]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[2]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[3]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[4]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[5]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[6]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							-1.0f*DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						-1.0f*DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));

		corners[7]=
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorAdd(
						DirectX::XMVectorScale(
							otherOBB.x_hx,
							DirectX::XMVectorGetW(otherOBB.x_hx)),
						DirectX::XMVectorScale(
							otherOBB.y_hy,
							-1.0f*DirectX::XMVectorGetW(otherOBB.y_hy))),
					DirectX::XMVectorScale(
						otherOBB.z_hz,
						DirectX::XMVectorGetW(otherOBB.z_hz))),
				DirectX::XMVectorAdd(
					otherOBB.centre,
					otherPosition));
	}
}
