#include <iostream>
#include <ctime>
#include <unistd.h>
using namespace std;

int main(){
	time_t st;
	time_t ct;
	time(&st);
	while(true){
		if(difftime(ct,st) >= 5 && difftime(ct,st) < 6){
			cout<<"shutting down after: "<<difftime(ct,st)<<endl;
			break;
		} else {
			cout<<"not yet "<<difftime(ct,st)<<endl;
		}
	time(&ct);
	}
	return 0;
}
