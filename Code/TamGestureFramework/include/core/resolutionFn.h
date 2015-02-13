#ifndef __RESOLUTIONFN__
#define __RESOLUTIONFN__


using namespace std;

namespace TAM
{

	template <class FN>
	class ResolutionFunction
	{
	public:
		//sample lambda below 
		//[](const type_strength s) { return 1.0*s; }
		ResolutionFunction(FN _fn) : fn(_fn)
		{
		}

		void setFunction(FN fn)
		{
			this->fn = fn;
		}

		FN getFunction()
		{
			return fn; 
		}
	private:
		FN fn;
	};
}

#endif 