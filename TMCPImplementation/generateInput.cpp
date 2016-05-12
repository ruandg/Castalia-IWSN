#include <iostream>
#include <cstdlib>

using namespace std;

int main() {
	srand(time(NULL));
	int N;
	int X,Y;
	//cout << "Digite a quantidade de nós" <<endl;
	cin >> N;
	//cout << "Digite o tamanho do terreno (X e Y):" << endl;
	cin >> X >> Y;
	cout << N << endl;
	cout << X/2 << " " <<  Y/2 << " " << 1 << endl; //sink node no meio
	
	for(int i = 1; i < N; i++) {
		cout << rand()%X << " " << rand()%Y << " " << 1 << endl;
	}
	return 0;
}