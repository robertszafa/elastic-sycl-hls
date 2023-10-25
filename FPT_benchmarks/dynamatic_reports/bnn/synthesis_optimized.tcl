
set_project .
set_top_file bnn.cpp
synthesize -use-lsq=true -verbose
set_period 4
optimize
write_hdl

exit



