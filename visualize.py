import argparse
from os import system
from time import sleep

def prettyp(g):
	print("-----------------------------------------")
	for row in g:
		for i in row:
			print(i, end='\t')
		print()
	print("-----------------------------------------")

parser = argparse.ArgumentParser()
parser.add_argument("--gridfile", type=str)
parser.add_argument("--stepfile", type=str)

args = parser.parse_args()
with open(args.gridfile, 'r') as infile:
	L = infile.readlines()
	n = int(L[0])
	grid = L[1].strip().split()
	zpos = grid.index('0')
	xx = zpos // n
	yy = zpos % n
	grid = [[grid[i*n + j] for j in range(n)] for i in range(n)]

with open(args.stepfile, 'r') as infile:
	steps = infile.readlines()
	if steps == []:
		pass
	else:
		steps = steps[0].strip().split()
		system('clear')
		prettyp(grid)
		sleep(1)
		for s in steps:
			if s == "R":
				grid[xx][yy] = grid[xx][yy+1]
				grid[xx][yy+1] = '0'
				yy += 1
			elif s == "L":
				grid[xx][yy] = grid[xx][yy-1]
				grid[xx][yy-1] = '0'
				yy -= 1
			elif s == "U":
				grid[xx][yy] = grid[xx-1][yy]
				grid[xx-1][yy] = '0'
				xx -= 1
			elif s =="D":
				grid[xx][yy] = grid[xx+1][yy]
				grid[xx+1][yy] = '0'
				xx += 1
			system('clear')
			prettyp(grid)
			sleep(1)

sleep(1)
system('clear')
