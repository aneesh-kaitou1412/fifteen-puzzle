#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <queue>
#include <string>
#include <unordered_map>

using namespace std;

#define row(i,N) (i)/(N)
#define col(i,N) (i)%(N)
#define println(x) cout<<(x)<<"\n";
#define prints(x) cout<<(x)<<" ";

int N = 4;
int N2 = 16;
int D = 2;

// maps a gridstring(state) to tuple of (g, h, zpos, reference to parent gridstring, reference to self gridstring)
unordered_map<string, tuple<int, int, int, string*, string*>> MAP;

// priority queue of pair (f, reference to gridstring)
priority_queue<pair<int, string*>> PRQ;

// Counts number of inversions in the array
// think carefully, only needed once
int inversion_sort(int* arr, int start, int end)
{
	// for(int i=start;i<end;i++)
	// 	cout<<arr[i]<<" ";
	// cout<<"\n";
	if(end - start <= 1)
	{
		// cout<<"SORT DONE\n";
		// for(int i=start;i<end;i++)
		// 	cout<<arr[i]<<" ";
		// cout<<"\nInversions : 0\n";
		return 0;
		
	}
	else
	{
		int mid = (start + end) / 2;
		
		int inv1 = inversion_sort(arr, start, mid);
		int inv2 = inversion_sort(arr, mid, end);

		int inv3 = 0;
		int idx1 = start;
		int idx2 = mid;

		int temp[end-start];
		int i=0;

		while(idx1<mid && idx2<end)
		{
			if(arr[idx1] > arr[idx2])
			{
				inv3 += mid - idx1;
				temp[i++] = arr[idx2++];
			}
			else
			{
				temp[i++] = arr[idx1++];
			}
		}

		while(idx1<mid)
		{
			temp[i++] = arr[idx1++];
		}

		while(idx2<end)
		{
			temp[i++] = arr[idx2++];
		}

		for(int j=start;j<end;j++)
		{
			arr[j] = temp[j-start];
		}

		// cout<<"SORT DONE\n";
		// for(int j=start;j<end;j++)
		// 	cout<<arr[j]<<" ";
		// cout<<"\nInversions : "<<inv3+inv2+inv1;
		// cout<<"\n";
		return inv3+inv2+inv1;
	}
}

// check if current configuration is solvable
// think carefully, only needed once
int is_solvable(int * temparr, int zpos)
{
	int inv = inversion_sort(temparr, 0, N2)-zpos;
	
	// prints("Inversions");
	// println(inv);
	
	// prints("Pos");
	// prints(row(zpos,N));
	// println(col(zpos,N));
	
	if(N%2 == 1 && inv%2 == 0)
	{
		return 1;
	}
	if(N%2 == 0)
	{
		if((N-row(zpos, N))%2 == 0 && inv%2 == 1)
		{
			return 1;
		}
		if((N-row(zpos, N))%2 == 1 && inv%2 == 0)
		{
			return 1;
		}
	}
	return -1;
}

// calculates Manhatten distance for the configuration
int manhatten(string s)
{
	int res = 0;
	for(int i=0, j=0; i<N2*D && j<N2; i+=D, j++)
	{
		int num = stoi(s.substr(i,D));
		if(num!=0)
		{
			res += abs(row(j,N)-row(num-1,N)) + abs(col(j,N)-col(num-1,N));
		}
	}
	// prints("Manhatten distance");
	// println(res);
	return res;
}

// helper to convert int to padded string
string int_to_padded_str(int num) {
  ostringstream out;
  out << std::setfill('0') << std::setw(D) << num;
  return out.str();
}

// convert array to gridstring
string array_to_string(int * arr)
{
	string s = "";
	for(int i=0; i<N2; i++)
	{
		s += int_to_padded_str(arr[i]);
	}
	prints("Grid String");
	println(s);
	return s;
}

// given old pos and new pos find the move direction
string find_move(int oldpos, int newpos)
{
	int UD = row(newpos,N)-row(oldpos,N);
	int LR = col(newpos,N)-col(oldpos,N);
	if(UD==1 && LR==0)
	{
		return "D";
	}
	else if(UD==-1 && LR==0)
	{
		return "U";
	}
	else if(UD==0 && LR==1)
	{
		return "R";
	}
	else if(UD==0 && LR==-1)
	{
		return "L";
	}
	else
	{
		return "incorrect move";
	}
}

// complete a star updation function
void check_direction(string * state, int dir)
{
	// get gval and zpos
	int gval, zpos;
	tie(gval, ignore, zpos, ignore, ignore) = MAP[*state];
	// based on dir, get new zpos, if invalid return
	int xx, yy;
	switch(dir)
	{
		// left
		case 0:
			xx = row(zpos,N);
			yy = col(zpos,N)-1;
			break;
		// right
		case 1:
			xx = row(zpos,N);
			yy = col(zpos,N)+1;
			break;
		// up
		case 2:
			xx = row(zpos,N)-1;
			yy = col(zpos,N);
			break;
		// down
		case 3:
			xx = row(zpos,N)+1;
			yy = col(zpos,N);
			break;
		default:
			return;
	}

	// prints(row(zpos,N));
	// println(col(zpos,N));
	// prints(xx);
	// println(yy);

	if(xx<0 || xx>=N || yy<0 || yy>=N)
	{
		return;
	}
	int newzpos = xx*N + yy;

	// get gridstate of the new zpos
	string s1 = (*state).substr(zpos*D, D);
	string s2 = (*state).substr(newzpos*D, D);
	string newstate = *state;
	newstate.replace(zpos,D,s2);
	newstate.replace(newzpos,D,s1);

	// println(*state);
	// println(newstate);
	
	// if newstate already present in map then 
	// no need to allocate new array for state
	// just check and update
	string * parent = NULL;
	int oldgval;
	tie(oldgval, ignore, ignore, parent, ignore) = MAP[newstate];
	if(parent==NULL)
	{
		string * s = new string;
		*s = newstate;
		MAP[*s] = make_tuple(gval+1, manhatten(*s), newzpos, state, s);
		PRQ.emplace(-get<0>(MAP[*s])-get<1>(MAP[*s]), s);
	}
	else
	{
		if(gval+1 < oldgval)
		{
			get<0>(MAP[newstate]) = gval+1;
			get<3>(MAP[newstate]) = state;
		}
	}
	return;
}



int main()
{
	// initialize size of grid and max digits that could be in a number in the grid
	int n;
	cin>>n;
	N = n;
	N2 = n*n;
	D = 0;
	int k = N2;
	while(k>0)
	{
		k/=10;
		D++;
	}

	// initialize array to take in the grid numbers,
	// temparr for inversion counting, initial position of space
	int * arr = new int[N2];
	int * temparr = new int[N2];
	int zpos;

	// get the input
	for(int i=0;i<N2;i++)
	{
		cin>>arr[i];
		if(arr[i]==0)
		{
			zpos=i;
		}
		temparr[i]=arr[i];
	}

	if(is_solvable(temparr,zpos) == -1)
	{
		cout<<"Not Solvable\n";
		return -1;
	}
	else
	{
		cout<<"Solvable\n";
		
		// SOLVING PROCESS :
		// variable for the end node
		string * finstate = NULL;

		// initialize start node
		string none = "NONE";
		string * start = new string;
		*start = array_to_string(arr);
		MAP[*start] = make_tuple(0, manhatten(*start), zpos, &none, start);

		// initialize priority queue and insert start node
		PRQ.emplace(-get<0>(MAP[*start])-get<1>(MAP[*start]), start);

		// while queue not empty
		while(!PRQ.empty())
		{
			// 	pick min element from queue
			string * curstate = PRQ.top().second;
			PRQ.pop();
			int curhval = get<1>(MAP[*curstate]);

			// if the goal state is reached then break
			if(curhval == 0)
			{
				finstate = curstate;
				break;
			}
			// 	in all directions, check if gval of child decreases by moving through curnode
			for(int i=0; i<4; i++)
			{
				// if it decreases, then update that node
				// insert new pointer and value pair in queue	
				// single function to do so
				check_direction(curstate, i);
			}
		}
		
		// decoding from the final state
		if(finstate == NULL)
		{
			return -1;
		}
		else
		{
			vector<string> moves;
			
			// while parent of current state is not none
			while(*get<3>(MAP[*finstate])!="NONE")
			{
				int curpos = get<2>(MAP[*finstate]);
				finstate = get<3>(MAP[*finstate]);
				int prevpos = get<2>(MAP[*finstate]);
				// add the move from parent to current
				moves.push_back(find_move(prevpos, curpos));
			}

			// print the moves
			prints("Number of Moves");
			println(moves.size());
			for(int i=moves.size()-1;i>=0;i--)
			{
				prints(moves[i]);
			}
		}
	}

	for(auto& x: MAP)
	{
		delete get<4>(x.second);
    }
}