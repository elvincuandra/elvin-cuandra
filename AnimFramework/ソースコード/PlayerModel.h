#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include "Object.h"

//! PlayerModel class
/** This class is responsible for managing the player model.
 *  It inherits from Object class.
 */
class PlayerModel : public Object
{
protected:
	virtual void Init() override;
	virtual void Update(uint64_t deltaTime) override;
	virtual void Draw(uint64_t deltaTime) override;
	virtual void Uninit() override;

public:
	//! ShowMeshStatistics function<br>
	/** This function shows the mesh statistics in the ImGui window.
	 */
	void ShowMeshStatistics();

private:
	//! PrintMatrices function<br>
	/** This function prints the matrices. Used for ImGui Debugging
	 *  @param matrix : matrix to be printed
	 */
	void PrintMatrices(DirectX::XMFLOAT4X4& matrix);
};

#endif // PLAYERMODEL_H