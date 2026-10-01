.PHONY: clean program

program: checker

checker: emailer

emailer: ChatGPT.o
	g++ ChatGPT.o -o emailer

ChatGPT.o: ChatGPT.cpp
	g++ -c ChatGPT.cpp

clean:
	rm -f ChatGPT.o emailer
