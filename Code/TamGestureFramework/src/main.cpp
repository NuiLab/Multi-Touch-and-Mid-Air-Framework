#include <iostream>
#include <data.h>
#include <vector.h> 
#include <memory>
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

	cout << "TAM GESTURE FRAMEWORK!\n";
	return 0;
}