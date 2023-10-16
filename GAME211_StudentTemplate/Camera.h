#include <MMath.h>

using namespace MATH;

class Camera
{
public:
	Matrix4 projectionMatrix;
	Matrix4 ndc;
	Matrix4 ortho;
	Camera()
	{
	}
	Camera(int w, int h, float xAxis, float yAxis)
	{
		ndc = MMath::viewportNDC(w, h);
		ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
		projectionMatrix = ndc * ortho;
	}
	virtual void OnDestroy() = 0;
	virtual ~Camera() {};
	virtual Matrix4 getProjectionMatrix();
};

