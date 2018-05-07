#include "MeshFilter.h"



MeshFilter::MeshFilter()
{
}

MeshFilter::MeshFilter(Mesh * mesh):Component(-1, "Mesh Filter")
{
	setMesh(mesh);

}


MeshFilter::~MeshFilter()
{
}

void MeshFilter::setMesh(Mesh * in_setMesh)
{
	this->mesh = in_setMesh;

	std::vector<DirectX::XMVECTOR>* temp = mesh->getVertexPositions();
	boundingBox.isActive = true;

	float maxX = DirectX::XMVectorGetX(temp[0][0]);
	float minX = DirectX::XMVectorGetX(temp[0][0]);
	float maxY = DirectX::XMVectorGetY(temp[0][0]);
	float minY = DirectX::XMVectorGetY(temp[0][0]);
	float maxZ = DirectX::XMVectorGetZ(temp[0][0]);
	float minZ = DirectX::XMVectorGetZ(temp[0][0]);


	for (int i = 1; i < temp->size(); i++)
	{
		float tempX = DirectX::XMVectorGetX(temp[0][i]);
		if (tempX < minX)
			minX = tempX;
		if (tempX > maxX)
			maxX = tempX;

		float tempY = DirectX::XMVectorGetY(temp[0][i]);
		if (tempY < minY)
			minY = tempY;
		if (tempY > maxY)
			maxY = tempY;

		float tempZ = DirectX::XMVectorGetZ(temp[0][i]);
		if (tempZ < minZ)
			minZ = tempZ;
		if (tempZ > maxZ)
			maxZ = tempZ;
	}

	float centerX = (maxX + minX) / 2;
	float centerY = (maxY + minY) / 2;
	float centerZ = (maxZ + minZ) / 2;

	boundingBox.centre = DirectX::XMVectorSet(centerX, centerY, centerZ, 0);

	float hx = (maxX - minX) / 2;
	float hy = (maxY - minY) / 2;
	float hz = (maxZ - minZ) / 2;

	boundingBox.x_hx = DirectX::XMVectorSet(1, 0, 0, hx);
	boundingBox.y_hy = DirectX::XMVectorSet(0, 1, 0, hy);
	boundingBox.z_hz = DirectX::XMVectorSet(0, 0, 1, hz);



	boundingBox.corners[0] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre
		);

	boundingBox.corners[1] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						-1.0f*DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						-1.0f*DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					-1.0f*DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre
		);
	boundingBox.corners[2] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					-1.0f*DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre);
	boundingBox.corners[3] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						-1.0f*DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						-1.0f*DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre);
	boundingBox.corners[4] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						-1.0f*DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre);
	boundingBox.corners[5] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						-1.0f*DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					-1.0f*DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre);
	boundingBox.corners[6] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						-1.0f*DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					-1.0f*DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre);
	boundingBox.corners[7] =
		DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorAdd(
					DirectX::XMVectorScale(
						boundingBox.x_hx,
						DirectX::XMVectorGetW(boundingBox.x_hx)),
					DirectX::XMVectorScale(
						boundingBox.y_hy,
						-1.0f*DirectX::XMVectorGetW(boundingBox.y_hy))),
				DirectX::XMVectorScale(
					boundingBox.z_hz,
					DirectX::XMVectorGetW(boundingBox.z_hz))),
			boundingBox.centre);

}

void MeshFilter::update()
{
}

