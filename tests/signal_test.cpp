#include <iostream>
#include <csignal>
#include <unistd.h>
using namespace std;

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

//unInitApp(); //clear your app
  exit(signum);  //if no exit, must do :   killall signal_test   
}

int main () {
   // register signal SIGINT and signal handler  
   signal(SIGINT, signalHandler);  

   while(1) {
      cout << "Going to sleep...." << endl;
      sleep(1);
   }

   return 0;
}