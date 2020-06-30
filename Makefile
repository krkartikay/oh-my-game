run: main
	./build/main

main:
	sh -c 'mkdir -p build/; cd build/ && cmake .. && make'
