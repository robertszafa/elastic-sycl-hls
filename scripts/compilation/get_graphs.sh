# $1 - bc/ll filename

# Generate PDFs with the CFG and DDG for the supplied LLVM IR.

LLVM_BIN_DIR=$ELASTIC_SYCL_HLS_DIR/llvm/build/bin

BC_FILE=$1

$LLVM_BIN_DIR/opt -passes=strip $BC_FILE -o $BC_FILE.strip

BC_FILE_BASENAME_WITH_EXT=`basename "$BC_FILE"`
BASENAME=${BC_FILE_BASENAME_WITH_EXT%.*}


# Standard DDG
$LLVM_BIN_DIR/opt -passes=dot-ddg $BC_FILE.strip -o /dev/null > /dev/null 
dot -Tpdf *.dot -o $BASENAME.DDG.pdf > /dev/null 2>&1 
echo $BASENAME.DDG.pdf
rm *.dot 

# DDG of only Strongly Connected Componennts
$LLVM_BIN_DIR/opt -load-pass-plugin $ELASTIC_SYCL_HLS_DIR/build/lib/libDDGDotPrinter.so \
                                   -passes=dot-ddg-sccs $BC_FILE.strip > /dev/null 2>&1 

j=0
for fname in /tmp/DDG_function*.dot; do
  dot -Tpdf $fname -o $BASENAME.DDG_function_$j.pdf
  echo $BASENAME.DDG_function_$j.pdf
  (( j++ ))
  rm $fname
done

i=0
for fname in /tmp/DDG_loop_*.dot; do
  dot -Tpdf $fname -o $BASENAME.DDG_loop_$i.pdf
  echo $BASENAME.DDG_loop_$i.pdf
  (( i++ ))
  rm $fname
done


# Multiple standard CFGs
$LLVM_BIN_DIR/opt -passes=dot-cfg $BC_FILE.strip > /dev/null 2>&1 
i=0
for fname in .*.dot; do
  dot -Tpdf $fname -o ${BASENAME}_CFG_$i.pdf
  echo ${BASENAME}_CFG_$i.pdf
  (( i++ ))
  rm $fname
done

rm $BC_FILE.strip
