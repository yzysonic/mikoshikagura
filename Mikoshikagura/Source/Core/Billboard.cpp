#include "Billboard.h"
#include "Renderer.h"

void Billboard::Draw(void)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate, mtxView;
	auto pDevice = Direct3D::GetDevice();
	auto camera = Renderer::GetInstance()->getCamera();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	mtxView = camera->getViewMatrix(false);
	mtxView._41 =
	mtxView._42 =
	mtxView._43 = 0.0f;

	D3DXMatrixInverse(&mtxView, NULL, &mtxView);


	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, this->object->transform.scale.x, this->object->transform.scale.y, this->object->transform.scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	// ��]�𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, this->object->transform.getRotation().y, this->object->transform.getRotation().x, this->object->transform.getRotation().z);
	//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �J�����̉�]�̋t����
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxView);

	// ���s�ړ��𔽉f(�n�`��z�u���Ă���)
	D3DXMatrixTranslation(&mtxTranslate, this->object->transform.position.x, this->object->transform.position.y, this->object->transform.position.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	// ���[���h�}�g���N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, this->pVtxBuff, 0, sizeof(Vertex3D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, this->pTexture->pDXTex);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, RECT_NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
