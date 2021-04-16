xs: rw_fbstring.c
	gcc -o xs rw_fbstring.c -Wall -Wextra -Wshadow -g -fsanitize=address,undefined

testbench_new: teststringc.c
	gcc -o teststringc teststringc.c -Wall -Wextra -Wshadow -g -fsanitize=address,undefined
#need testbench1.txt

testbench_cow: testcow.c
	gcc -o testcow testcow.c -Wall -Wextra -Wshadow -g -fsanitize=address,undefined
#need testbench1.txt to testbench3.txt

testbench_cpp_new: tetstringcpp.cpp
	g++ tetstringcpp.cpp -o teststringcpp
#need testbench1.txt

testbench_cpp_cow: testcppcow.cpp
	g++ testcppcow.cpp -o testcppcow
#need testbench1.txt to testbench3.txt

clean:
	rm xs testbench_new testbench_cow testcppcow teststringcpp *.o
