#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

const int xnum = 36;
const int ynum = 27;
const int tnum = 72;

int main(int argc, char const* argv[])
{
	unsigned char values[ynum][xnum][tnum];
	for(int t=0;t<tnum;t++)
		for(int x=0;x<xnum;x++)
			for(int y=0;y<ynum;y++)
				values[y][x][t] = 0;

	int index;
	unsigned long value;
	while(1){
		cin >> index >> value;
		if(cin.eof())
			break;

		int y = index/(xnum*tnum);
		int x = (index/tnum)%xnum;
		int t = index%tnum;

		double v = (double)value / 100;
		//max 10100
		if(v <= 100.0)
			values[y][x][t] = (unsigned char)(255.0 - v/100.0*255.0);
	}

	for(int t=0;t<tnum;t++){
		ofstream ofs("./tmp/values" + to_string(t*5) + ".pgm");
		ofs << "P2 " << xnum << ' ' << ynum << ' ' << 255 << endl;

		for(int y=ynum-1;y>=0;y--)
			for(int x=0;x<xnum;x++)
				ofs << (int)values[y][x][t] << endl;

		ofs.close();
	}

	return 0;
}
