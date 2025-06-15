#include <iostream>


using namespace std;

class animal{};
class dog : public animal{
	public:
		int weight;
};

int main(){
	animal *a = new animal();
	dog *d = (dog*)(a);
	d->weight = 20;
	cout << d->weight << endl;
}