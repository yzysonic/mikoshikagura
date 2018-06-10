#include "Polygon.h"
#include "Renderer.h"
#include "Lerp.h"


RectPolygon2D::RectPolygon2D(std::string texture_name, Layer layer, std::string render_space_name) : RectPolygon2D(Texture::Get(texture_name), layer, render_space_name){}

RectPolygon2D::RectPolygon2D(Texture * texture, Layer layer, std::string render_space_name) : Drawable(layer, render_space_name)
{
	this->Component::type = ComponentType::RectPolygon2D;
	this->rendType = RendererType::UI;
	this->pattern = 0;

	// rhw�̐ݒ�
	this->vertex[0].rhw =
	this->vertex[1].rhw =
	this->vertex[2].rhw =
	this->vertex[3].rhw = 1.0f;

	SetTexture(texture);
}

void RectPolygon2D::Draw(void)
{
	// ���_���W�̍X�V
	transformVertex();

	Direct3D::GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Direct3D::GetDevice()->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	Direct3D::GetDevice()->SetTexture(0, this->pTexture->pDXTex);

	// �|���S���̕`��
	Direct3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, RECT_NUM_POLYGON, this->vertex, sizeof(Vertex2D));

	Direct3D::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}


Vector2 RectPolygon2D::GetSize(void)
{
	return this->size;
}


void RectPolygon2D::SetSize(float x, float y)
{
	this->size = Vector2(x, y);
	this->radius = this->size.length()/2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
}


void RectPolygon2D::SetColor(Color color)
{
	this->vertex[0].diffuse = color;
	this->vertex[1].diffuse = color;
	this->vertex[2].diffuse = color;
	this->vertex[3].diffuse = color;
	this->color = color;
}

void RectPolygon2D::setPattern(unsigned int pattern)
{
	this->pattern = pattern;

	int x = pattern % this->pTexture->divideX;
	int y = pattern / this->pTexture->divideX;
	Vector2 size = Vector2(1.0f / this->pTexture->divideX, 1.0f / this->pTexture->divideY);

	this->vertex[0].uv = Vector2(x*size.x, y*size.y);
	this->vertex[1].uv = Vector2(x*size.x + size.x, y*size.y);
	this->vertex[2].uv = Vector2(x*size.x, y*size.y + size.y);
	this->vertex[3].uv = Vector2(x*size.x + size.x, y*size.y + size.y);
}

void RectPolygon2D::SetTexture(std::string texture_name)
{
	SetTexture(Texture::Get(texture_name));
}

void RectPolygon2D::SetTexture(Texture * texture)
{
	if (texture == nullptr)
		texture = Texture::none;

	this->pTexture = texture;
	this->size = this->pTexture->size;

	this->radius = this->size.length() / 2;
	this->baseAngle = atan2f(this->size.y, this->size.x);
	if (this->pTexture != Texture::none && this->pTexture->pDXTex == nullptr)
		SetColor(Color(255, 0, 255, 255));
	else
		SetColor(Color::white);

	// �e�N�X�`�����W�̐ݒ�
	this->vertex[0].uv = Vector2(0.0f, 0.0f);
	this->vertex[1].uv = Vector2(1.0f / this->pTexture->divideX, 0.0f);
	this->vertex[2].uv = Vector2(0.0f, 1.0f / this->pTexture->divideY);
	this->vertex[3].uv = Vector2(1.0f / this->pTexture->divideX, 1.0f / this->pTexture->divideY);

}

//=============================================================================
// ���_�̍��W�ϊ�
//=============================================================================
void RectPolygon2D::transformVertex(void)
{
	// ���[���h�ϊ�
	Vector3 pos = this->object->transform.position;
	Vector3 rot = this->object->transform.getRotation();
	Vector3 radius = this->radius * this->object->transform.scale;

	this->vertex[0].pos.x = pos.x - cosf(this->baseAngle - rot.z) * radius.x;
	this->vertex[0].pos.y = pos.y + sinf(this->baseAngle - rot.z) * radius.y;
	this->vertex[0].pos.z = pos.z;

	this->vertex[1].pos.x = pos.x + cosf(this->baseAngle + rot.z) * radius.x;
	this->vertex[1].pos.y = pos.y + sinf(this->baseAngle + rot.z) * radius.y;
	this->vertex[1].pos.z = pos.z;

	this->vertex[2].pos.x = pos.x - cosf(this->baseAngle + rot.z) * radius.x;
	this->vertex[2].pos.y = pos.y - sinf(this->baseAngle + rot.z) * radius.y;
	this->vertex[2].pos.z = pos.z;

	this->vertex[3].pos.x = pos.x + cosf(this->baseAngle - rot.z) * radius.x;
	this->vertex[3].pos.y = pos.y - sinf(this->baseAngle - rot.z) * radius.y;
	this->vertex[3].pos.z = pos.z;

	Camera *camera = Renderer::GetInstance()->getCamera();

	if (this->rendType == RendererType::Classic2D)
	{

		// �J�����ϊ�
		this->vertex[0].pos.x -= camera->transform.position.x;
		this->vertex[0].pos.y -= camera->transform.position.y;
		this->vertex[1].pos.x -= camera->transform.position.x;
		this->vertex[1].pos.y -= camera->transform.position.y;
		this->vertex[2].pos.x -= camera->transform.position.x;
		this->vertex[2].pos.y -= camera->transform.position.y;
		this->vertex[3].pos.x -= camera->transform.position.x;
		this->vertex[3].pos.y -= camera->transform.position.y;

		// ���e�ϊ�
		//float fov = Lerpf(this->vertex[0].pos.z, 1.0f, camera->fov);
		//this->vertex[0].pos.x /= this->vertex[0].pos.z / fov;
		//this->vertex[0].pos.y /= this->vertex[0].pos.z / fov;
		//this->vertex[0].pos.z = (this->vertex[0].pos.z - camera->near_z) / (camera->far_z - camera->near_z);

		//this->vertex[1].pos.x /= this->vertex[1].pos.z / fov;
		//this->vertex[1].pos.y /= this->vertex[1].pos.z / fov;
		//this->vertex[1].pos.z = (this->vertex[1].pos.z - camera->near_z) / (camera->far_z - camera->near_z);

		//this->vertex[2].pos.x /= this->vertex[2].pos.z / fov;
		//this->vertex[2].pos.y /= this->vertex[2].pos.z / fov;
		//this->vertex[2].pos.z = (this->vertex[2].pos.z - camera->near_z) / (camera->far_z - camera->near_z);

		//this->vertex[3].pos.x /= this->vertex[3].pos.z / fov;
		//this->vertex[3].pos.y /= this->vertex[3].pos.z / fov;
		//this->vertex[3].pos.z = (this->vertex[3].pos.z - camera->near_z) / (camera->far_z - camera->near_z);

	}

	// �X�N���[���ϊ�
	this->vertex[0].pos.y = -this->vertex[0].pos.y;
	this->vertex[1].pos.y = -this->vertex[1].pos.y;
	this->vertex[2].pos.y = -this->vertex[2].pos.y;
	this->vertex[3].pos.y = -this->vertex[3].pos.y;

	this->vertex[0].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);
	this->vertex[1].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);
	this->vertex[2].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);
	this->vertex[3].pos += Vector3(camera->render_target->size.x / 2.f, camera->render_target->size.y / 2.f, 0.0f);

}

RectPolygon::RectPolygon(std::string texture_name, Layer layer, std::string render_space_name) : RectPolygon(Texture::Get(texture_name), layer, render_space_name) {}

RectPolygon::RectPolygon(Texture * texture, Layer layer, std::string render_space_name) : Drawable(layer, render_space_name)
{
	this->Component::type = ComponentType::RectPolygon;
	this->layer = layer;
	this->rendType = RendererType::Default;

	InitBuffer();
	SetTexture(texture);
	SetSize(Vector2::one);
}


RectPolygon::~RectPolygon(void)
{
	SafeRelease(this->pVtxBuff);
}

HRESULT RectPolygon::OnLostDevice(void)
{
	SafeRelease(this->pVtxBuff);
	return S_OK;
}

HRESULT RectPolygon::OnResetDevice(void)
{
	return InitBuffer();
}

void RectPolygon::Draw(void)
{
	D3DXMATRIX mtxWorld, mtxScl, mtxRot, mtxTranslate, mtxView;
	auto pDevice = Direct3D::GetDevice();
	auto camera = Renderer::GetInstance()->getCamera();

	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, this->object->transform.scale.x, this->object->transform.scale.y, this->object->transform.scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, this->object->transform.getRotation().y, this->object->transform.getRotation().x, this->object->transform.getRotation().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		
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

}

void RectPolygon::SetColor(Color color)
{
	Vertex3D *pVtx;

	this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = color;

	this->pVtxBuff->Unlock();

	this->color = color;
}


Vector2 RectPolygon::GetSize(void)
{
	return this->size;
}

void RectPolygon::SetSize(Vector2 size)
{
	this->size = size;

	Vertex3D *pVtx;

	this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = Vector3(-0.5f*size.x, 0.5f*size.y, 0.0f);
	pVtx[1].pos = Vector3(0.5f*size.x, 0.5f*size.y, 0.0f);
	pVtx[2].pos = Vector3(-0.5f*size.x, -0.5f*size.y, 0.0f);
	pVtx[3].pos = Vector3(0.5f*size.x, -0.5f*size.y, 0.0f);

	this->pVtxBuff->Unlock();
}

void RectPolygon::SetTexture(std::string texture_name)
{
	SetTexture(Texture::Get(texture_name));
}

void RectPolygon::SetTexture(Texture * texture)
{

	if (texture != Texture::none && texture->pDXTex == nullptr)
		SetColor(Color(255, 0, 255, 255));
	else
		SetColor(Color::white);

	this->pTexture = texture;
}

void RectPolygon::LockBuff(Vertex3D ** pVtx)
{
	this->pVtxBuff->Lock(0, 0, (void**)pVtx, 0);
}

void RectPolygon::UnlockBuff(void)
{
	this->pVtxBuff->Unlock();
}

HRESULT RectPolygon::InitBuffer(void)
{
	HRESULT hr;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	hr = Direct3D::GetDevice()->CreateVertexBuffer(sizeof(Vertex3D) * RECT_NUM_VERTEX,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
		D3DUSAGE_DYNAMIC,			// ���_�o�b�t�@�̎g�p�@�@
		FVF_VERTEX_3D,				// �g�p���钸�_�t�H�[�}�b�g
		D3DPOOL_DEFAULT,			// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
		&this->pVtxBuff,			// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
		NULL);

	if (FAILED(hr))
		return hr;

	Vertex3D *pVtx;

	hr = this->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (FAILED(hr))
		return hr;

	// ���_���W�̐ݒ�(���[�J�����W)
	pVtx[0].pos = Vector3(-0.5f*size.x, 0.5f*size.y, 0.0f);
	pVtx[1].pos = Vector3(0.5f*size.x, 0.5f*size.y, 0.0f);
	pVtx[2].pos = Vector3(-0.5f*size.x, -0.5f*size.y, 0.0f);
	pVtx[3].pos = Vector3(0.5f*size.x, -0.5f*size.y, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = Vector3(0.0f, 0.0f, -1.0f);

	// ���ˌ��̐ݒ�
	pVtx[0].diffuse =
	pVtx[1].diffuse =
	pVtx[2].diffuse =
	pVtx[3].diffuse = this->color;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].uv = Vector2(0.0f, 0.0f);
	pVtx[1].uv = Vector2(1.0f, 0.0f);
	pVtx[2].uv = Vector2(0.0f, 1.0f);
	pVtx[3].uv = Vector2(1.0f, 1.0f);

	this->pVtxBuff->Unlock();

	return hr;
}
