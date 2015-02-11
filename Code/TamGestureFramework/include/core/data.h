#ifndef __DATA__
#define __DATA__ 



using namespace std; 

namespace TAM
{

	template <class T>
	class Data
	{
	public:
		//T* but you can share it and it will sdestroy itself 
		//* == raw pointer 
		//shared_ptr == ownership is shared. 
		//unique_ptr == only one owner. 
		//weak_ptr == ??? 

		T getData()
		{
			return  data;
		}
		void setData(T newData)
		{
			this->data = newData;
		}
	private:
		T data;
	};
}
#endif