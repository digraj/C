def sort(f, g):
	fi = open(f, 'r')
	fj = open(g, 'w')
	l = fi.readlines()
	ll = []
	for i in l:
		ll.append(long(l))	
	ll.sort()
	for i in ll:
		fj.write(str(i))
	
	fi.close()
	fj.close()
def main():
	name = input("Filename: ")
	out = input("Output file: ")
	sort(name, out)

main()
