./compile_to_bc.sh ex1/ex1.cpp
./opt.sh ex1/ex1.cpp.ll 
./opt.sh ex1/ex1.cpp.ll 2> opt_out.txt 
python3 genKernelsAndPipes.py opt_out.txt ex1/ex1.cpp