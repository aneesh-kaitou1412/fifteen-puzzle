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

struct node
{
	string* config;
	int g;
	int h;
	string* parent;
	int zpos;
	short move; // move that led to this state
};

// maps grid string to its node structure
unordered_map<string, node*> MAP;

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
string int_to_padded_str(int num) 
{
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
	// prints("Grid String");
	// println(s);
	return s;
}

// Print move letter given number
void find_move(int move)
{
	switch(move)
	{
		case 0:
			cout<<"L ";
			return;
		case 3:
			cout<<"R ";
			return;
		case 2:
			cout<<"U ";
			return;
		case 1:
			cout<<"D ";
			return;
		default:
			return;
	}
}

// complete A* updation function
void check_direction(string * state, int dir)
{
	// get the node of current state
	node * curnode = MAP[*state];
	int zpos = curnode->zpos;

	if(curnode->move + dir == 3)
	{
		return;
	}

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
		case 3:
			xx = row(zpos,N);
			yy = col(zpos,N)+1;
			break;
		// up
		case 2:
			xx = row(zpos,N)-1;
			yy = col(zpos,N);
			break;
		// down
		case 1:
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
	string newstate = *state;
	for(int i=0;i<D;i++)
	{
		char temp = newstate[zpos*D + i];
		newstate[zpos*D + i] = newstate[newzpos*D + i];
		newstate[newzpos*D + i] = temp;
	}

	// println("-----------------------------------------------------");
	// println(*state);
	// println(newstate);
	// println("-----------------------------------------------------");

	// if newstate already present in map then 
	// no need to allocate new array for state
	// just check and update
	node * checknode = MAP[newstate];
	if(checknode==NULL)
	{
		string * s = new string;
		*s = newstate;
		node * n = new node;
		n->config = s;
		n->g = curnode->g + 1;
		n->h = manhatten(*s);
		n->parent = state;
		n->move = dir;
		n->zpos = newzpos;
		MAP[*s] = n;
		PRQ.emplace(- n->g - n->h, s);
	}
	else
	{
		if(curnode->g + 1 < checknode->g + 1)
		{
			MAP[newstate]->g = curnode->g + 1;
			MAP[newstate]->parent = state;
			MAP[newstate]->move = dir;
			PRQ.emplace(- MAP[newstate]->g - MAP[newstate]->h, MAP[newstate]->config);
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
		// cout<<"Not Solvable\n";
		return -1;
	}
	else
	{
		// cout<<"Solvable\n";
		
		// SOLVING PROCESS :
		// variable for the end node
		string * finstate = NULL;

		// initialize start node
		string none = "NONE";
		string * start = new string;
		*start = array_to_string(arr);
		node * fnode = new node;
		fnode->g = 0;
		fnode->h = manhatten(*start);
		fnode->parent = &none;
		fnode->move = -1;
		fnode->zpos = zpos;
		fnode->config = start;
		MAP[*start] = fnode;

		// initialize priority queue and insert start node
		PRQ.emplace(-MAP[*start]->g - MAP[*start]->h, start);

		// while queue not empty
		while(!PRQ.empty())
		{
			// 	pick min element from queue
			string * curstate = PRQ.top().second;
			PRQ.pop();
			int curhval = MAP[*curstate]->h;

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
			vector<int> moves;
			// while parent of current state is not none
			while(MAP[*finstate]->move!=-1)
			{
				// add the move from parent to current
				moves.push_back(MAP[*finstate]->move);
				// backtrace pointer to parent
				finstate = MAP[*finstate]->parent;
			}
			
			// print the moves
			// prints("Number of Moves");
			// println(moves.size());
			for(int i=moves.size()-1;i>=0;i--)
			{
				find_move(moves[i]);
			}
			println(" ");
		}
	}

	// for(auto& x: MAP)
	// {
	// 	delete x.second->config;
	// 	delete x.second;
 	// }
}