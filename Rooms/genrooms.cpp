#include<fstream>
using namespace std;

int main()
{
  for (int i=0; i<20; i++) {
    for (int j=0; j<5; j++) {
      char fname[] = "room0000.txt";
      fname[4] = (char)((i/10) + 48);
      fname[5] = (char)((i%10) + 48);
      fname[6] = (char)48;
      fname[7] = (char)(j+48);
      ofstream fout(fname);
      fout << "500,50,500,50,GROUND\n";
      fout.close();
    }
  }
}
