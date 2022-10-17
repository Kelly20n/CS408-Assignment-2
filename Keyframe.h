// Header file used to store key frame information
#include <vector>
#include <string>


namespace std
{
	class Keyframe
	{
	public:
		int objID, frameNum;
		float x, y, z, rotationX, rotationY, rotationZ, scaleX, scaleY, scaleZ;
	private:

	};
}
