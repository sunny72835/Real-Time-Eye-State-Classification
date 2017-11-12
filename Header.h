#include <stdexcept>
using namespace std;
namespace eye
{
	 extern "C" 
	 {
		 __declspec(dllexport) int eyetrack(int flag);
	 }
}