build:
	mpicc main.c paragraph_list.c master.c worker.c -o main -lpthread
debug:
	mpicc main.c paragraph_list.c master.c worker.c -o main -lpthread -fsanitize=address
run:
	mpirun -np 5 --oversubscribe ./main tests/in/input1.txt
run1:
	mpirun -np 5 --oversubscribe ./main tests/in/input1.txt
run2:
	mpirun -np 5 --oversubscribe ./main tests/in/input2.txt
run3:
	mpirun -np 5 --oversubscribe ./main tests/in/input3.txt
run4:
	mpirun -np 5 --oversubscribe ./main tests/in/input4.txt
run5:
	mpirun -np 5 --oversubscribe ./main tests/in/input5.txt
diff1:
	diff -B -Z tests/in/input1.out tests/ref/input1.ref >> dif1.txt
diff2:
	diff -B -Z tests/in/input2.out tests/ref/input2.ref >> dif2.txt
diff3:
	diff -B -Z tests/in/input3.out tests/ref/input3.ref >> dif3.txt
diff4:
	diff -B -Z tests/in/input4.out tests/ref/input4.ref >> dif4.txt
diff5:
	diff -B -Z tests/in/input5.out tests/ref/input5.ref >> dif5.txt
clean:
	rm main dif1.txt dif2.txt dif3.txt dif4.txt dif5.txt
