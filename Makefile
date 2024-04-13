test:
	g++ -Wall -Wextra ./src/main.cpp ./src/SerialMotorCtrl.cpp -I./include -Og -o test.out
clean:
	rm -rf test.out