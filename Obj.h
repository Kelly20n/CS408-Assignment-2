// Object Class
#include "Keyframe.h"

namespace std
{
	// All values are used to store or help store information from the text file for easy access
	class Obj
	{
	public:
		float xIncrement = 0;	
		float yIncrement = 0;
		float zIncrement = 0;
		float rotationXincrement = 0;
		float rotationYincrement = 0;
		float rotationZincrement = 0;
		float scaleXincrement = 0;
		float scaleYincrement = 0;
		float scaleZincrement = 0;
		int age = NULL;
		int key = 0;
		int counter = 0;	 // Used to keep track of what frame we've done
		int objID = NULL;
		int frameCount = 0;
		string objFile;
		vector<Keyframe> groupFrames;	// Each object has a vector of key frames
	private:

	};
}

