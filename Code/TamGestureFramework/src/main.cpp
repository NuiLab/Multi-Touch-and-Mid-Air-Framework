#include <iostream>
#include <vector.h> 
#include <memory>
#include <functional> 

//tam
#include <data.h>
#include <domain.h> 
#include <resolutionFn.h>
using namespace std;
int main(int argc, const char * argv[]) {
	// insert code here...
	using PVector3 = shared_ptr<TAM::Vector<3, int>>;

	PVector3 myVector; 
	TAM::Data<PVector3> dd;
	dd.setData(myVector);
	TAM::Data<float> d; 
	//shared_ptr<float> df = make_shared<float>(10);
	//float * f2 = new float(10); //in c will malloc 
	d.setData(10.0f);

	//TAM::Domain<int> mydomain(-1, 1); 

	auto mylambda = [](TAM::Data<int> val) { return 2*val.getData(); };

	using MYFUNC = function < int(TAM::Data<int>)>;
	TAM::ResolutionFunction<MYFUNC> ResFn(mylambda); 

	auto f = ResFn.getFunction();
	TAM::Data<int> dx; 
	dx.setData(1); 
	int x = f(dx);
	cout << "X is equal to " << x << endl; 
	cout << "TAM GESTURE FRAMEWORK!\n";
	cout << "Press Ctrl + c to exit" << endl;
	while(true);

		
	
	return 0;
}