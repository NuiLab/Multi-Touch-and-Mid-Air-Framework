#ifndef __DOMAIN__
#define __DOMAIN__


namespace TAM
{

	template <class T>
	class Domain
	{
	public:
		//sample lambda below 
		//[](const type_strength s) { return 1.0*s; }
		
		Domain(T _vmin, T _vmax) : vmin(_vmin), vmax(_vmax)
		{
		}

		void setDomain(T vmin, T vmax)
		{
			setMin(vmin);
			setMax(vmax); 
		}

		T getMin()
		{
			return this->vmin;
		}

		T getMax()
		{
			return this->vmax; 
		}

		void setMin(T vmin)
		{
			this->vmin = vmin;
		}

		void setMax(T vmax)
		{
			this->vmax = vmax;
		}
	private:
		T vmin;
		T vmax; 
	};
}

#endif 