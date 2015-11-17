#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

const int xnum = 36;
const int ynum = 27;
const int tnum = 72;

int main(int argc, char const* argv[])
{
	unsigned char policy[ynum][xnum][tnum];
	for(int t=0;t<tnum;t++)
		for(int x=0;x<xnum;x++)
			for(int y=0;y<ynum;y++)
				policy[y][x][t] = 0;

	int index;
	string action;
	while(1){
		cin >> index >> action;
		if(cin.eof())
			break;

		int y = index/(xnum*tnum);
		int x = (index/tnum)%xnum;
		int t = index%tnum;

		if(action == "ccw")
			policy[y][x][t] = 255;
		else if(action == "fw")
			policy[y][x][t] = 155;
		else if(action == "cw")
			policy[y][x][t] = 55;
	}

	for(int t=0;t<tnum;t++){
		ofstream ofs("./tmp/policy" + to_string(t*5) + ".pgm");
		ofs << "P2 " << xnum << ' ' << ynum << ' ' << 255 << endl;

		for(int y=ynum-1;y>=0;y--)
			for(int x=0;x<xnum;x++)
				ofs << (int)policy[y][x][t] << endl;

		ofs.close();
	}

	return 0;
}
