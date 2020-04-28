all:
		make scheduler.o 
		make process.o 
		make main.o
		make main
main: main.o scheduler.o process.o
		gcc main.o scheduler.o process.o -o main
main.o: main.c Makefile
		gcc main.c -c
scheduler.o: scheduler.c scheduler.h Makefile
		gcc -c scheduler.c 
process.o: process.c process.h Makefile
		gcc -c process.c
clean:
		rm -rf *o
run:
		sudo ./main < test.in
demo:
	make
	sudo dmesg -c
	sudo ./main < OS_PJ1_Test/TIME_MEASUREMENT.txt
	dmesg |grep Project1
	sudo dmesg -c
	sudo ./main < OS_PJ1_Test/FIFO_1.txt
	dmesg |grep Project1
	sudo dmesg -c
	sudo ./main < OS_PJ1_Test/PSJF_2.txt
	dmesg |grep Project1
	sudo dmesg -c
	sudo ./main < OS_PJ1_Test/RR_3.txt
	dmesg |grep Project1
	sudo dmesg -c
	sudo ./main < OS_PJ1_Test/SJF_4.txt
	dmesg |grep Project1
